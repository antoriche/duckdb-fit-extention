#pragma once

#include "duckdb.hpp"
#include "duckdb/function/table_function.hpp"
#include "include/fit_types.hpp"
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

	FitTableFunctionData(string name, string type = "records", ClientContext *context = nullptr);

private:
	void LoadFitFile();
};

} // namespace duckdb
