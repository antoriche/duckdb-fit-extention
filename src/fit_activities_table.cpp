#include "include/fit_activities_table.hpp"
#include "include/fit_table_base.hpp"

namespace duckdb {

// FIT Activities table function - activity metadata
unique_ptr<FunctionData> FitActivitiesBind(ClientContext &context, TableFunctionBindInput &input,
                                           vector<LogicalType> &return_types, vector<string> &names) {
	auto file_path = input.inputs[0].GetValue<string>();

	// Define schema for activities metadata table
	vector<pair<string, LogicalType>> columns = {{"activity_id", LogicalType::UBIGINT},
	                                             {"file_id", LogicalType::VARCHAR},
	                                             {"timestamp", LogicalType::TIMESTAMP_TZ},
	                                             {"local_timestamp", LogicalType::TIMESTAMP_TZ},
	                                             {"start_time", LogicalType::TIMESTAMP_TZ},
	                                             {"total_timer_time", LogicalType::DOUBLE},
	                                             {"total_elapsed_time", LogicalType::DOUBLE},
	                                             {"total_distance", LogicalType::DOUBLE},
	                                             {"sport", LogicalType::VARCHAR},
	                                             {"sub_sport", LogicalType::VARCHAR},
	                                             {"manufacturer", LogicalType::VARCHAR},
	                                             {"product", LogicalType::VARCHAR},
	                                             {"device_serial_number", LogicalType::UBIGINT},
	                                             {"software_version", LogicalType::VARCHAR},
	                                             {"total_calories", LogicalType::UINTEGER},
	                                             {"total_ascent", LogicalType::DOUBLE},
	                                             {"total_descent", LogicalType::DOUBLE},
	                                             {"avg_heart_rate", LogicalType::UTINYINT},
	                                             {"max_heart_rate", LogicalType::UTINYINT},
	                                             {"avg_speed", LogicalType::DOUBLE},
	                                             {"max_speed", LogicalType::DOUBLE},
	                                             {"avg_power", LogicalType::USMALLINT},
	                                             {"max_power", LogicalType::USMALLINT},
	                                             {"avg_cadence", LogicalType::UTINYINT},
	                                             {"max_cadence", LogicalType::UTINYINT},
	                                             {"start_position_lat", LogicalType::DOUBLE},
	                                             {"start_position_long", LogicalType::DOUBLE},
	                                             {"end_position_lat", LogicalType::DOUBLE},
	                                             {"end_position_long", LogicalType::DOUBLE},
	                                             {"file_source", LogicalType::VARCHAR}};

	for (const auto &col : columns) {
		names.push_back(col.first);
		return_types.push_back(col.second);
	}

	return make_uniq<FitTableFunctionData>(file_path, "activities", &context, FitCompressionParameter(input));
}

void FitActivitiesFunction(ClientContext &context, TableFunctionInput &data_p, DataChunk &output) {
	auto &data = (FitTableFunctionData &)*data_p.bind_data;

	// Defensive check for empty activities vector
	if (data.fit_activities.empty() || data.current_row >= data.fit_activities.size()) {
		output.SetCardinality(0);
		return;
	}

	idx_t remaining_rows = data.fit_activities.size() - data.current_row;
	idx_t rows_to_output = MinValue<idx_t>(remaining_rows, STANDARD_VECTOR_SIZE);

	if (rows_to_output == 0) {
		output.SetCardinality(0);
		return;
	}

	for (idx_t row = 0; row < rows_to_output; row++) {
		const auto &activity = data.fit_activities[data.current_row + row];
		idx_t col = 0;

		output.SetValue(col++, row, Value::UBIGINT(activity.activity_id));
		output.SetValue(col++, row, Value(activity.file_id));
		output.SetValue(col++, row, Value::TIMESTAMPTZ(activity.timestamp));
		output.SetValue(col++, row, Value::TIMESTAMPTZ(activity.local_timestamp));
		output.SetValue(col++, row, Value::TIMESTAMPTZ(activity.start_time));
		output.SetValue(col++, row, Value::DOUBLE(activity.total_timer_time));
		output.SetValue(col++, row, Value::DOUBLE(activity.total_elapsed_time));
		output.SetValue(col++, row, Value::DOUBLE(activity.total_distance));
		output.SetValue(col++, row, Value(activity.sport));
		output.SetValue(col++, row, Value(activity.sub_sport));
		output.SetValue(col++, row, Value(activity.manufacturer));
		output.SetValue(col++, row, Value(activity.product));
		output.SetValue(col++, row, Value::UBIGINT(activity.device_serial_number));
		output.SetValue(col++, row, Value(activity.software_version));
		output.SetValue(col++, row, Value::UINTEGER(activity.total_calories));
		output.SetValue(col++, row, Value::DOUBLE(activity.total_ascent));
		output.SetValue(col++, row, Value::DOUBLE(activity.total_descent));
		output.SetValue(col++, row, activity.avg_heart_rate > 0 ? Value::UTINYINT(activity.avg_heart_rate) : Value());
		output.SetValue(col++, row, activity.max_heart_rate > 0 ? Value::UTINYINT(activity.max_heart_rate) : Value());
		output.SetValue(col++, row, Value::DOUBLE(activity.avg_speed));
		output.SetValue(col++, row, Value::DOUBLE(activity.max_speed));
		output.SetValue(col++, row, activity.avg_power > 0 ? Value::USMALLINT(activity.avg_power) : Value());
		output.SetValue(col++, row, activity.max_power > 0 ? Value::USMALLINT(activity.max_power) : Value());
		output.SetValue(col++, row, activity.avg_cadence > 0 ? Value::UTINYINT(activity.avg_cadence) : Value());
		output.SetValue(col++, row, activity.max_cadence > 0 ? Value::UTINYINT(activity.max_cadence) : Value());
		output.SetValue(col++, row,
		                activity.start_position_lat != 0.0 ? Value::DOUBLE(activity.start_position_lat) : Value());
		output.SetValue(col++, row,
		                activity.start_position_long != 0.0 ? Value::DOUBLE(activity.start_position_long) : Value());
		output.SetValue(col++, row,
		                activity.end_position_lat != 0.0 ? Value::DOUBLE(activity.end_position_lat) : Value());
		output.SetValue(col++, row,
		                activity.end_position_long != 0.0 ? Value::DOUBLE(activity.end_position_long) : Value());
		output.SetValue(col++, row, !activity.file_source.empty() ? Value(activity.file_source) : Value());
	}

	output.SetCardinality(rows_to_output);
	data.current_row += rows_to_output;
}

} // namespace duckdb
