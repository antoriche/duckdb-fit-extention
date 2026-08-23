#pragma once

#include "duckdb.hpp"
#include "duckdb/common/enums/file_compression_type.hpp"
#include "duckdb/function/table_function.hpp"
#include "fit_types.hpp"
#include <vector>

namespace duckdb {

struct FitTableFunctionData : public TableFunctionData {
	string input_name;
	idx_t current_row;
	std::vector<FitRecord> fit_records;
	std::vector<FitActivity> fit_activities;
	std::vector<FitSession> fit_sessions;
	std::vector<FitLap> fit_laps;
	std::vector<FitDevice> fit_devices;
	std::vector<FitEvent> fit_events;
	std::vector<FitUser> fit_users;
	string user_timezone;
	string table_type; // To distinguish which table this data is for
	FileCompressionType compression;

	FitTableFunctionData(string name, string type = "records", ClientContext *context = nullptr,
	                     FileCompressionType compression = FileCompressionType::AUTO_DETECT);

private:
	ClientContext *context;
	void LoadFitFile();
};

// Reads the optional `compression` named parameter shared by every FIT table function.
FileCompressionType FitCompressionParameter(TableFunctionBindInput &input);

} // namespace duckdb
