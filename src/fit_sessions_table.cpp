#include "include/fit_sessions_table.hpp"
#include "include/fit_table_base.hpp"

namespace duckdb {

unique_ptr<FunctionData> FitSessionsBind(ClientContext &context, TableFunctionBindInput &input,
                                         vector<LogicalType> &return_types, vector<string> &names) {
	auto file_path = input.inputs[0].GetValue<string>();

	vector<pair<string, LogicalType>> columns = {
	    {"session_id", LogicalType::UINTEGER},       {"activity_id", LogicalType::UBIGINT},
	    {"timestamp", LogicalType::TIMESTAMP_TZ},    {"start_time", LogicalType::TIMESTAMP_TZ},
	    {"total_elapsed_time", LogicalType::DOUBLE}, {"total_timer_time", LogicalType::DOUBLE},
	    {"total_distance", LogicalType::DOUBLE},     {"sport", LogicalType::VARCHAR},
	    {"sub_sport", LogicalType::VARCHAR},         {"total_calories", LogicalType::UINTEGER},
	    {"avg_speed", LogicalType::DOUBLE},          {"max_speed", LogicalType::DOUBLE},
	    {"avg_heart_rate", LogicalType::UTINYINT},   {"max_heart_rate", LogicalType::UTINYINT},
	    {"min_heart_rate", LogicalType::UTINYINT},   {"avg_cadence", LogicalType::UTINYINT},
	    {"max_cadence", LogicalType::UTINYINT},      {"avg_power", LogicalType::USMALLINT},
	    {"max_power", LogicalType::USMALLINT},       {"normalized_power", LogicalType::USMALLINT},
	    {"intensity_factor", LogicalType::DOUBLE},   {"training_stress_score", LogicalType::DOUBLE},
	    {"total_work", LogicalType::UINTEGER},       {"total_ascent", LogicalType::DOUBLE},
	    {"total_descent", LogicalType::DOUBLE},      {"first_lap_index", LogicalType::UTINYINT},
	    {"num_laps", LogicalType::UTINYINT},         {"event", LogicalType::VARCHAR},
	    {"event_type", LogicalType::VARCHAR},        {"trigger", LogicalType::VARCHAR},
	    {"file_source", LogicalType::VARCHAR}};

	for (const auto &col : columns) {
		names.push_back(col.first);
		return_types.push_back(col.second);
	}

	return make_uniq<FitTableFunctionData>(file_path, "sessions", &context);
}

void FitSessionsFunction(ClientContext &context, TableFunctionInput &data_p, DataChunk &output) {
	auto &data = (FitTableFunctionData &)*data_p.bind_data;

	idx_t remaining_rows = data.fit_sessions.size() - data.current_row;
	idx_t rows_to_output = MinValue<idx_t>(remaining_rows, STANDARD_VECTOR_SIZE);

	if (rows_to_output == 0) {
		output.SetCardinality(0);
		return;
	}

	for (idx_t row = 0; row < rows_to_output; row++) {
		const auto &session = data.fit_sessions[data.current_row + row];
		idx_t col = 0;

		output.SetValue(col++, row, Value::UINTEGER(session.session_id));
		output.SetValue(col++, row, Value::UBIGINT(session.activity_id));
		output.SetValue(col++, row, Value::TIMESTAMPTZ(session.timestamp));
		output.SetValue(col++, row, Value::TIMESTAMPTZ(session.start_time));
		output.SetValue(col++, row, Value::DOUBLE(session.total_elapsed_time));
		output.SetValue(col++, row, Value::DOUBLE(session.total_timer_time));
		output.SetValue(col++, row, Value::DOUBLE(session.total_distance));
		output.SetValue(col++, row, Value(session.sport));
		output.SetValue(col++, row, Value(session.sub_sport));
		output.SetValue(col++, row, Value::UINTEGER(session.total_calories));
		output.SetValue(col++, row, Value::DOUBLE(session.avg_speed));
		output.SetValue(col++, row, Value::DOUBLE(session.max_speed));
		output.SetValue(col++, row, session.avg_heart_rate > 0 ? Value::UTINYINT(session.avg_heart_rate) : Value());
		output.SetValue(col++, row, session.max_heart_rate > 0 ? Value::UTINYINT(session.max_heart_rate) : Value());
		output.SetValue(col++, row, session.min_heart_rate > 0 ? Value::UTINYINT(session.min_heart_rate) : Value());
		output.SetValue(col++, row, session.avg_cadence > 0 ? Value::UTINYINT(session.avg_cadence) : Value());
		output.SetValue(col++, row, session.max_cadence > 0 ? Value::UTINYINT(session.max_cadence) : Value());
		output.SetValue(col++, row, session.avg_power > 0 ? Value::USMALLINT(session.avg_power) : Value());
		output.SetValue(col++, row, session.max_power > 0 ? Value::USMALLINT(session.max_power) : Value());
		output.SetValue(col++, row,
		                session.normalized_power > 0 ? Value::USMALLINT(session.normalized_power) : Value());
		output.SetValue(col++, row, Value::DOUBLE(session.intensity_factor));
		output.SetValue(col++, row, Value::DOUBLE(session.training_stress_score));
		output.SetValue(col++, row, Value::UINTEGER(session.total_work));
		output.SetValue(col++, row, Value::DOUBLE(session.total_ascent));
		output.SetValue(col++, row, Value::DOUBLE(session.total_descent));
		output.SetValue(col++, row, Value::UTINYINT(session.first_lap_index));
		output.SetValue(col++, row, Value::UTINYINT(session.num_laps));
		output.SetValue(col++, row, Value(session.event));
		output.SetValue(col++, row, Value(session.event_type));
		output.SetValue(col++, row, Value(session.trigger));
		output.SetValue(col++, row, !session.file_source.empty() ? Value(session.file_source) : Value());
	}

	output.SetCardinality(rows_to_output);
	data.current_row += rows_to_output;
}

} // namespace duckdb
