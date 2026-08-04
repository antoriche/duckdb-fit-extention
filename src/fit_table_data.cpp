#include "include/fit_table_base.hpp"
#include "include/fit_collector.hpp"

#include "duckdb/common/file_system.hpp"

#include "fit_decode.hpp"
#include "fit_mesg_broadcaster.hpp"

#include <map>
#include <sstream>
#include <stdexcept>

namespace duckdb {

FitTableFunctionData::FitTableFunctionData(string name, string type, ClientContext *context)
    : input_name(name), current_row(0), user_timezone("UTC"), table_type(type), context(context) {
	// Get user's timezone setting if context is available
	if (context) {
		Value timezone_value;
		if (context->TryGetCurrentSetting("TimeZone", timezone_value)) {
			user_timezone = timezone_value.ToString();
		}
	}
	// Load and parse FIT file
	LoadFitFile();
}

void FitTableFunctionData::LoadFitFile() {
	try {
		// Check for empty or null input
		if (input_name.empty()) {
			throw std::runtime_error("File path cannot be empty");
		}

		// Resolve DuckDB's virtual filesystem so that reads work across every backend
		// DuckDB knows about: local files, s3://, https://, registered in-memory files,
		// and the WASM (Emscripten) filesystem. Reading through this layer instead of
		// std::fstream is what makes the extension usable in the DuckDB-WASM build.
		unique_ptr<FileSystem> local_fs;
		FileSystem *fs_ptr;
		if (context) {
			fs_ptr = &FileSystem::GetFileSystem(*context);
		} else {
			// No client context (e.g. direct construction in a unit test): fall back to
			// the local filesystem so behaviour matches a native single-file read.
			local_fs = FileSystem::CreateLocal();
			fs_ptr = local_fs.get();
		}
		FileSystem &fs = *fs_ptr;

		// Expand glob pattern to get list of files. HasGlob keeps the wildcard detection
		// consistent with DuckDB, and fs.Glob honours the active filesystem backend.
		bool has_wildcards = FileSystem::HasGlob(input_name);

		vector<string> files;
		if (has_wildcards) {
			for (auto &info : fs.Glob(input_name)) {
				files.push_back(info.path);
			}
		} else {
			files.push_back(input_name);
		}

		if (files.empty()) {
			// Only reachable for wildcard patterns that matched nothing: return an
			// empty result (valid schema, 0 rows).
			fit_records.clear();
			fit_activities.clear();
			fit_sessions.clear();
			fit_laps.clear();
			fit_devices.clear();
			fit_events.clear();
			fit_users.clear();
			return;
		}

		// Create shared collector that will accumulate data from all files
		FitDataCollector collector;

		// Process each file
		for (const auto &file_path : files) {
			try {
				// Read the whole file through DuckDB's filesystem into memory, then wrap
				// the bytes in an istringstream for the FIT SDK decoder (which only knows
				// how to consume a std::istream).
				auto handle = fs.OpenFile(file_path, FileFlags::FILE_FLAGS_READ);
				idx_t file_size = handle->GetFileSize();

				std::string buffer;
				buffer.resize(file_size);
				idx_t bytes_read = 0;
				while (bytes_read < file_size) {
					int64_t n = handle->Read((void *)(buffer.data() + bytes_read), file_size - bytes_read);
					if (n <= 0) {
						break; // Reached EOF early; decode whatever we managed to read
					}
					bytes_read += (idx_t)n;
				}
				buffer.resize(bytes_read);

				std::istringstream file(buffer, std::ios::in | std::ios::binary);

				// Set current file in collector
				collector.SetCurrentFile(file_path);

				fit::Decode decode;
				fit::MesgBroadcaster mesgBroadcaster;

				// Check file integrity
				if (!decode.CheckIntegrity(file)) {
					// Continue anyway, might still be readable
				}

				// Add listeners for all message types
				mesgBroadcaster.AddListener((fit::RecordMesgListener &)collector);
				mesgBroadcaster.AddListener((fit::FileIdMesgListener &)collector);
				mesgBroadcaster.AddListener((fit::ActivityMesgListener &)collector);
				mesgBroadcaster.AddListener((fit::SessionMesgListener &)collector);
				mesgBroadcaster.AddListener((fit::LapMesgListener &)collector);
				mesgBroadcaster.AddListener((fit::DeviceInfoMesgListener &)collector);
				mesgBroadcaster.AddListener((fit::EventMesgListener &)collector);
				mesgBroadcaster.AddListener((fit::UserProfileMesgListener &)collector);

				// Decode the file
				decode.Read(&file, &mesgBroadcaster, &mesgBroadcaster, nullptr);

			} catch (const std::exception &e) {
				if (!has_wildcards) {
					// For single files, propagate the error
					throw std::runtime_error("Error reading FIT file '" + file_path + "': " + string(e.what()));
				}
				// For wildcard patterns, continue with next file if one fails
				continue;
			}
		}

		// Copy collected data from all files
		fit_records = std::move(collector.records);
		fit_activities = std::move(collector.activities);
		fit_sessions = std::move(collector.sessions);
		fit_laps = std::move(collector.laps);
		fit_devices = std::move(collector.devices);
		fit_events = std::move(collector.events);
		fit_users = std::move(collector.users);

		// Post-process: populate activity_type from session data
		// This is now done per file, but we keep this for backward compatibility
		if (!fit_sessions.empty() && !fit_records.empty()) {
			// Group records by file source and apply activity types accordingly
			std::map<string, string> file_activity_types;

			// Build mapping of file -> activity type from sessions
			for (const auto &session : fit_sessions) {
				if (!session.sport.empty() && !session.file_source.empty()) {
					file_activity_types[session.file_source] = session.sport;
				}
			}

			// Apply activity types to records based on their file source
			for (auto &record : fit_records) {
				if (file_activity_types.find(record.file_source) != file_activity_types.end()) {
					record.activity_type = file_activity_types[record.file_source];
				}
			}
		}

		// Post-process: populate activities with session data
		if (!fit_sessions.empty() && !fit_activities.empty()) {
			// Group by file source and match sessions to activities
			std::map<string, std::vector<FitSession *>> file_sessions;
			std::map<string, std::vector<FitActivity *>> file_activities;

			for (auto &session : fit_sessions) {
				file_sessions[session.file_source].push_back(&session);
			}
			for (auto &activity : fit_activities) {
				file_activities[activity.file_source].push_back(&activity);
			}

			// Match sessions to activities within each file
			for (const auto &file_pair : file_activities) {
				const string &file_source = file_pair.first;
				auto &activities = file_pair.second;

				if (file_sessions.find(file_source) != file_sessions.end() && !file_sessions[file_source].empty() &&
				    !activities.empty()) {
					auto &session = *file_sessions[file_source][0]; // Use first session
					auto &activity = *activities[0];                // Use first activity

					// Copy sport information
					activity.sport = session.sport;
					activity.sub_sport = session.sub_sport;

					// Copy performance data
					activity.total_distance = session.total_distance;
					activity.total_elapsed_time = session.total_elapsed_time;
					activity.total_calories = session.total_calories;
					activity.avg_heart_rate = session.avg_heart_rate;
					activity.max_heart_rate = session.max_heart_rate;
					activity.avg_speed = session.avg_speed;
					activity.max_speed = session.max_speed;
					activity.avg_power = session.avg_power;
					activity.max_power = session.max_power;
					activity.avg_cadence = session.avg_cadence;
					activity.max_cadence = session.max_cadence;
					activity.total_ascent = session.total_ascent;
					activity.total_descent = session.total_descent;

					// Copy start time if not already set
					if (activity.start_time.value == 0 && session.start_time.value != 0) {
						activity.start_time = session.start_time;
					}
				}
			}
		}

	} catch (const std::exception &e) {
		throw std::runtime_error("Error reading FIT files: " + string(e.what()));
	}
}

} // namespace duckdb
