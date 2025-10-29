#pragma once

#include "include/fit_types.hpp"
#include "fit_decode.hpp"
#include "fit_mesg_broadcaster.hpp"
#include "fit_record_mesg.hpp"
#include "fit_file_id_mesg.hpp"
#include "fit_activity_mesg.hpp"
#include "fit_session_mesg.hpp"
#include "fit_lap_mesg.hpp"
#include "fit_device_info_mesg.hpp"
#include "fit_event_mesg.hpp"
#include "fit_user_profile_mesg.hpp"

#include <vector>

namespace duckdb {

// FIT message listener to collect all types of data
class FitDataCollector : public fit::RecordMesgListener,
                         public fit::FileIdMesgListener,
                         public fit::ActivityMesgListener,
                         public fit::SessionMesgListener,
                         public fit::LapMesgListener,
                         public fit::DeviceInfoMesgListener,
                         public fit::EventMesgListener,
                         public fit::UserProfileMesgListener {
public:
	std::vector<FitRecord> records;
	std::vector<FitActivity> activities;
	std::vector<FitSession> sessions;
	std::vector<FitLap> laps;
	std::vector<FitDevice> devices;
	std::vector<FitEvent> events;
	std::vector<FitUser> users;
	string file_type;
	string manufacturer;
	string activity_name;
	string current_activity_type; // Track current activity type for records
	string current_file_source;   // Track current file being processed

	FitDataCollector();

	// Method to set the current file being processed
	void SetCurrentFile(const string &file_path);

	// Override message listeners
	void OnMesg(fit::RecordMesg &record) override;
	void OnMesg(fit::FileIdMesg &file_id) override;
	void OnMesg(fit::ActivityMesg &activity) override;
	void OnMesg(fit::SessionMesg &session) override;
	void OnMesg(fit::LapMesg &lap) override;
	void OnMesg(fit::DeviceInfoMesg &device_info) override;
	void OnMesg(fit::EventMesg &event) override;
	void OnMesg(fit::UserProfileMesg &user_profile) override;
};

} // namespace duckdb
