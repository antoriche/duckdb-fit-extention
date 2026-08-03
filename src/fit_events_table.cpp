#include "include/fit_events_table.hpp"
#include "include/fit_table_base.hpp"

namespace duckdb {

unique_ptr<FunctionData> FitEventsBind(ClientContext &context, TableFunctionBindInput &input,
                                       vector<LogicalType> &return_types, vector<string> &names) {
	auto file_path = input.inputs[0].GetValue<string>();

	// Define event columns based on FitEvent structure
	names = {"event_id",  "activity_id",  "timestamp",      "event",           "event_type", "data",
	         "data16",    "score",        "opponent_score", "front_gear_num",  "front_gear", "rear_gear_num",
	         "rear_gear", "device_index", "activity_type",  "start_timestamp", "file_source"};

	return_types = {LogicalType::UINTEGER,  LogicalType::UBIGINT,  LogicalType::TIMESTAMP_TZ, LogicalType::VARCHAR,
	                LogicalType::VARCHAR,   LogicalType::UINTEGER, LogicalType::USMALLINT,    LogicalType::USMALLINT,
	                LogicalType::USMALLINT, LogicalType::UTINYINT, LogicalType::UTINYINT,     LogicalType::UTINYINT,
	                LogicalType::UTINYINT,  LogicalType::UTINYINT, LogicalType::VARCHAR,      LogicalType::TIMESTAMP_TZ,
	                LogicalType::VARCHAR};

	return make_uniq<FitTableFunctionData>(file_path, "events", &context);
}

void FitEventsFunction(ClientContext &context, TableFunctionInput &data_p, DataChunk &output) {
	auto &data = (FitTableFunctionData &)*data_p.bind_data;

	idx_t remaining_rows = data.fit_events.size() - data.current_row;
	idx_t rows_to_output = MinValue<idx_t>(remaining_rows, STANDARD_VECTOR_SIZE);

	if (rows_to_output == 0) {
		output.SetCardinality(0);
		return;
	}

	// Fill the output chunk with event data
	for (idx_t row = 0; row < rows_to_output; row++) {
		const auto &fit_event = data.fit_events[data.current_row + row];
		idx_t col = 0;

		output.SetValue(col++, row, Value::UINTEGER(fit_event.event_id));
		output.SetValue(col++, row, Value::UBIGINT(fit_event.activity_id));
		output.SetValue(col++, row, Value::TIMESTAMPTZ(fit_event.timestamp));
		output.SetValue(col++, row, Value(fit_event.event));
		output.SetValue(col++, row, Value(fit_event.event_type));
		output.SetValue(col++, row, fit_event.data > 0 ? Value::UINTEGER(fit_event.data) : Value());
		output.SetValue(col++, row, fit_event.data16 > 0 ? Value::USMALLINT(fit_event.data16) : Value());
		output.SetValue(col++, row, fit_event.score > 0 ? Value::USMALLINT(fit_event.score) : Value());
		output.SetValue(col++, row,
		                fit_event.opponent_score > 0 ? Value::USMALLINT(fit_event.opponent_score) : Value());
		output.SetValue(col++, row, fit_event.front_gear_num > 0 ? Value::UTINYINT(fit_event.front_gear_num) : Value());
		output.SetValue(col++, row, fit_event.front_gear > 0 ? Value::UTINYINT(fit_event.front_gear) : Value());
		output.SetValue(col++, row, fit_event.rear_gear_num > 0 ? Value::UTINYINT(fit_event.rear_gear_num) : Value());
		output.SetValue(col++, row, fit_event.rear_gear > 0 ? Value::UTINYINT(fit_event.rear_gear) : Value());
		output.SetValue(col++, row, Value::UTINYINT(fit_event.device_index));
		output.SetValue(col++, row, Value(fit_event.activity_type));
		output.SetValue(col++, row, Value::TIMESTAMPTZ(fit_event.start_timestamp));
		output.SetValue(col++, row, !fit_event.file_source.empty() ? Value(fit_event.file_source) : Value());
	}

	output.SetCardinality(rows_to_output);
	data.current_row += rows_to_output;
}

} // namespace duckdb
