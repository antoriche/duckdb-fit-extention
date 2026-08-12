#include "include/fit_users_table.hpp"
#include "include/fit_table_base.hpp"

namespace duckdb {

unique_ptr<FunctionData> FitUsersBind(ClientContext &context, TableFunctionBindInput &input,
                                      vector<LogicalType> &return_types, vector<string> &names) {
	auto file_path = input.inputs[0].GetValue<string>();

	// Define user columns based on FitUser structure
	names = {"user_id",
	         "gender",
	         "age",
	         "height",
	         "weight",
	         "language",
	         "time_zone",
	         "activity_class",
	         "running_lactate_threshold_hr",
	         "cycling_lactate_threshold_hr",
	         "swimming_lactate_threshold_hr",
	         "default_max_running_hr",
	         "default_max_biking_hr",
	         "default_max_hr",
	         "hr_setting",
	         "speed_setting",
	         "dist_setting",
	         "power_setting",
	         "position_setting",
	         "temperature_setting",
	         "local_id",
	         "global_id",
	         "wake_time",
	         "sleep_time",
	         "height_setting",
	         "weight_setting",
	         "resting_heart_rate",
	         "default_max_swimming_hr",
	         "file_source"};

	return_types = {LogicalType::UINTEGER, LogicalType::VARCHAR,  LogicalType::UTINYINT, LogicalType::DOUBLE,
	                LogicalType::DOUBLE,   LogicalType::VARCHAR,  LogicalType::TINYINT,  LogicalType::DOUBLE,
	                LogicalType::UTINYINT, LogicalType::UTINYINT, LogicalType::UTINYINT, LogicalType::UTINYINT,
	                LogicalType::UTINYINT, LogicalType::UTINYINT, LogicalType::VARCHAR,  LogicalType::VARCHAR,
	                LogicalType::VARCHAR,  LogicalType::VARCHAR,  LogicalType::VARCHAR,  LogicalType::VARCHAR,
	                LogicalType::UINTEGER, LogicalType::UBIGINT,  LogicalType::UINTEGER, LogicalType::UINTEGER,
	                LogicalType::VARCHAR,  LogicalType::VARCHAR,  LogicalType::UTINYINT, LogicalType::UTINYINT,
	                LogicalType::VARCHAR};

	return make_uniq<FitTableFunctionData>(file_path, "users", &context, FitCompressionParameter(input));
}

void FitUsersFunction(ClientContext &context, TableFunctionInput &data_p, DataChunk &output) {
	auto &data = (FitTableFunctionData &)*data_p.bind_data;

	idx_t remaining_rows = data.fit_users.size() - data.current_row;
	idx_t rows_to_output = MinValue<idx_t>(remaining_rows, STANDARD_VECTOR_SIZE);

	if (rows_to_output == 0) {
		output.SetCardinality(0);
		return;
	}

	// Fill the output chunk with user data
	for (idx_t row = 0; row < rows_to_output; row++) {
		const auto &fit_user = data.fit_users[data.current_row + row];
		idx_t col = 0;

		output.SetValue(col++, row, Value::UINTEGER(fit_user.user_id));
		output.SetValue(col++, row, Value(fit_user.gender));
		output.SetValue(col++, row, fit_user.age > 0 ? Value::UTINYINT(fit_user.age) : Value());
		output.SetValue(col++, row, fit_user.height > 0.0 ? Value::DOUBLE(fit_user.height) : Value());
		output.SetValue(col++, row, fit_user.weight > 0.0 ? Value::DOUBLE(fit_user.weight) : Value());
		output.SetValue(col++, row, Value(fit_user.language));
		output.SetValue(col++, row, Value::TINYINT(fit_user.time_zone));
		output.SetValue(col++, row, fit_user.activity_class > 0.0 ? Value::DOUBLE(fit_user.activity_class) : Value());
		output.SetValue(col++, row,
		                fit_user.running_lactate_threshold_hr > 0
		                    ? Value::UTINYINT(fit_user.running_lactate_threshold_hr)
		                    : Value());
		output.SetValue(col++, row,
		                fit_user.cycling_lactate_threshold_hr > 0
		                    ? Value::UTINYINT(fit_user.cycling_lactate_threshold_hr)
		                    : Value());
		output.SetValue(col++, row,
		                fit_user.swimming_lactate_threshold_hr > 0
		                    ? Value::UTINYINT(fit_user.swimming_lactate_threshold_hr)
		                    : Value());
		output.SetValue(col++, row,
		                fit_user.default_max_running_hr > 0 ? Value::UTINYINT(fit_user.default_max_running_hr)
		                                                    : Value());
		output.SetValue(col++, row,
		                fit_user.default_max_biking_hr > 0 ? Value::UTINYINT(fit_user.default_max_biking_hr) : Value());
		output.SetValue(col++, row, fit_user.default_max_hr > 0 ? Value::UTINYINT(fit_user.default_max_hr) : Value());
		output.SetValue(col++, row, Value(fit_user.hr_setting));
		output.SetValue(col++, row, Value(fit_user.speed_setting));
		output.SetValue(col++, row, Value(fit_user.dist_setting));
		output.SetValue(col++, row, Value(fit_user.power_setting));
		output.SetValue(col++, row, Value(fit_user.position_setting));
		output.SetValue(col++, row, Value(fit_user.temperature_setting));
		output.SetValue(col++, row, fit_user.local_id > 0 ? Value::UINTEGER(fit_user.local_id) : Value());
		output.SetValue(col++, row, fit_user.global_id > 0 ? Value::UBIGINT(fit_user.global_id) : Value());
		output.SetValue(col++, row, fit_user.wake_time > 0 ? Value::UINTEGER(fit_user.wake_time) : Value());
		output.SetValue(col++, row, fit_user.sleep_time > 0 ? Value::UINTEGER(fit_user.sleep_time) : Value());
		output.SetValue(col++, row, Value(fit_user.height_setting));
		output.SetValue(col++, row, Value(fit_user.weight_setting));
		output.SetValue(col++, row,
		                fit_user.resting_heart_rate > 0 ? Value::UTINYINT(fit_user.resting_heart_rate) : Value());
		output.SetValue(col++, row,
		                fit_user.default_max_swimming_hr > 0 ? Value::UTINYINT(fit_user.default_max_swimming_hr)
		                                                     : Value());
		output.SetValue(col++, row, !fit_user.file_source.empty() ? Value(fit_user.file_source) : Value());
	}

	output.SetCardinality(rows_to_output);
	data.current_row += rows_to_output;
}

} // namespace duckdb
