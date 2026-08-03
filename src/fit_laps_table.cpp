#include "include/fit_laps_table.hpp"
#include "include/fit_table_base.hpp"

namespace duckdb {

unique_ptr<FunctionData> FitLapsBind(ClientContext &context, TableFunctionBindInput &input,
                                     vector<LogicalType> &return_types, vector<string> &names) {
	auto file_path = input.inputs[0].GetValue<string>();

	// Define lap columns based on FitLap structure
	names = {"lap_id",
	         "session_id",
	         "activity_id",
	         "timestamp",
	         "start_time",
	         "total_elapsed_time",
	         "total_timer_time",
	         "total_distance",
	         "total_calories",
	         "avg_speed",
	         "max_speed",
	         "avg_heart_rate",
	         "max_heart_rate",
	         "min_heart_rate",
	         "avg_cadence",
	         "max_cadence",
	         "avg_power",
	         "max_power",
	         "total_ascent",
	         "total_descent",
	         "lap_trigger",
	         "event",
	         "event_type",
	         "start_position_lat",
	         "start_position_long",
	         "end_position_lat",
	         "end_position_long",
	         "file_source"};

	return_types = {LogicalType::UINTEGER,     LogicalType::UINTEGER,  LogicalType::UBIGINT,  LogicalType::TIMESTAMP_TZ,
	                LogicalType::TIMESTAMP_TZ, LogicalType::DOUBLE,    LogicalType::DOUBLE,   LogicalType::DOUBLE,
	                LogicalType::UINTEGER,     LogicalType::DOUBLE,    LogicalType::DOUBLE,   LogicalType::UTINYINT,
	                LogicalType::UTINYINT,     LogicalType::UTINYINT,  LogicalType::UTINYINT, LogicalType::UTINYINT,
	                LogicalType::USMALLINT,    LogicalType::USMALLINT, LogicalType::DOUBLE,   LogicalType::DOUBLE,
	                LogicalType::VARCHAR,      LogicalType::VARCHAR,   LogicalType::VARCHAR,  LogicalType::DOUBLE,
	                LogicalType::DOUBLE,       LogicalType::DOUBLE,    LogicalType::DOUBLE,   LogicalType::VARCHAR};

	return make_uniq<FitTableFunctionData>(file_path, "laps", &context);
}

void FitLapsFunction(ClientContext &context, TableFunctionInput &data_p, DataChunk &output) {
	auto &data = (FitTableFunctionData &)*data_p.bind_data;

	idx_t remaining_rows = data.fit_laps.size() - data.current_row;
	idx_t rows_to_output = MinValue<idx_t>(remaining_rows, STANDARD_VECTOR_SIZE);

	if (rows_to_output == 0) {
		output.SetCardinality(0);
		return;
	}

	// Fill the output chunk with lap data
	for (idx_t row = 0; row < rows_to_output; row++) {
		const auto &fit_lap = data.fit_laps[data.current_row + row];
		idx_t col = 0;

		output.SetValue(col++, row, Value::UINTEGER(fit_lap.lap_id));
		output.SetValue(col++, row, Value::UINTEGER(fit_lap.session_id));
		output.SetValue(col++, row, Value::UBIGINT(fit_lap.activity_id));
		output.SetValue(col++, row, Value::TIMESTAMPTZ(fit_lap.timestamp));
		output.SetValue(col++, row, Value::TIMESTAMPTZ(fit_lap.start_time));
		output.SetValue(col++, row,
		                fit_lap.total_elapsed_time > 0.0 ? Value::DOUBLE(fit_lap.total_elapsed_time) : Value());
		output.SetValue(col++, row, fit_lap.total_timer_time > 0.0 ? Value::DOUBLE(fit_lap.total_timer_time) : Value());
		output.SetValue(col++, row, fit_lap.total_distance > 0.0 ? Value::DOUBLE(fit_lap.total_distance) : Value());
		output.SetValue(col++, row, fit_lap.total_calories > 0 ? Value::UINTEGER(fit_lap.total_calories) : Value());
		output.SetValue(col++, row, fit_lap.avg_speed > 0.0 ? Value::DOUBLE(fit_lap.avg_speed) : Value());
		output.SetValue(col++, row, fit_lap.max_speed > 0.0 ? Value::DOUBLE(fit_lap.max_speed) : Value());
		output.SetValue(col++, row, fit_lap.avg_heart_rate > 0 ? Value::UTINYINT(fit_lap.avg_heart_rate) : Value());
		output.SetValue(col++, row, fit_lap.max_heart_rate > 0 ? Value::UTINYINT(fit_lap.max_heart_rate) : Value());
		output.SetValue(col++, row, fit_lap.min_heart_rate > 0 ? Value::UTINYINT(fit_lap.min_heart_rate) : Value());
		output.SetValue(col++, row, fit_lap.avg_cadence > 0 ? Value::UTINYINT(fit_lap.avg_cadence) : Value());
		output.SetValue(col++, row, fit_lap.max_cadence > 0 ? Value::UTINYINT(fit_lap.max_cadence) : Value());
		output.SetValue(col++, row, fit_lap.avg_power > 0 ? Value::USMALLINT(fit_lap.avg_power) : Value());
		output.SetValue(col++, row, fit_lap.max_power > 0 ? Value::USMALLINT(fit_lap.max_power) : Value());
		output.SetValue(col++, row, fit_lap.total_ascent > 0.0 ? Value::DOUBLE(fit_lap.total_ascent) : Value());
		output.SetValue(col++, row, fit_lap.total_descent > 0.0 ? Value::DOUBLE(fit_lap.total_descent) : Value());
		output.SetValue(col++, row, Value(fit_lap.lap_trigger));
		output.SetValue(col++, row, Value(fit_lap.event));
		output.SetValue(col++, row, Value(fit_lap.event_type));
		output.SetValue(col++, row,
		                fit_lap.start_position_lat != 0.0 ? Value::DOUBLE(fit_lap.start_position_lat) : Value());
		output.SetValue(col++, row,
		                fit_lap.start_position_long != 0.0 ? Value::DOUBLE(fit_lap.start_position_long) : Value());
		output.SetValue(col++, row,
		                fit_lap.end_position_lat != 0.0 ? Value::DOUBLE(fit_lap.end_position_lat) : Value());
		output.SetValue(col++, row,
		                fit_lap.end_position_long != 0.0 ? Value::DOUBLE(fit_lap.end_position_long) : Value());
		output.SetValue(col++, row, !fit_lap.file_source.empty() ? Value(fit_lap.file_source) : Value());
	}

	output.SetCardinality(rows_to_output);
	data.current_row += rows_to_output;
}

} // namespace duckdb
