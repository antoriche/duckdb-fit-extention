#include "include/fit_records_table.hpp"
#include "include/fit_table_base.hpp"

namespace duckdb {

// FIT Records table function - main time-series data
unique_ptr<FunctionData> FitRecordsBind(ClientContext &context, TableFunctionBindInput &input,
                                        vector<LogicalType> &return_types, vector<string> &names) {
	// Get the input parameter (file path)
	auto file_path = input.inputs[0].GetValue<string>();

	// Define the FIT data schema with grouped names and types
	struct ColumnDef {
		string name;
		LogicalType type;
	};

	vector<ColumnDef> columns = {// Basic timestamp and location
	                             {"timestamp", LogicalType::TIMESTAMP_TZ},
	                             {"latitude", LogicalType::DOUBLE},
	                             {"longitude", LogicalType::DOUBLE},
	                             {"altitude", LogicalType::DOUBLE},
	                             {"enhanced_altitude", LogicalType::DOUBLE},

	                             // Speed and distance
	                             {"distance", LogicalType::DOUBLE},
	                             {"speed", LogicalType::DOUBLE},
	                             {"enhanced_speed", LogicalType::DOUBLE},
	                             {"vertical_speed", LogicalType::DOUBLE},

	                             // Power metrics
	                             {"power", LogicalType::USMALLINT},                        // uint16_t -> USMALLINT
	                             {"motor_power", LogicalType::USMALLINT},                  // uint16_t -> USMALLINT
	                             {"accumulated_power", LogicalType::UINTEGER},             // uint32_t -> UINTEGER
	                             {"compressed_accumulated_power", LogicalType::USMALLINT}, // uint16_t -> USMALLINT

	                             // Heart rate and physiological data
	                             {"heart_rate", LogicalType::UTINYINT}, // uint8_t -> UTINYINT (corrected)
	                             {"total_hemoglobin_conc", LogicalType::DOUBLE},
	                             {"total_hemoglobin_conc_min", LogicalType::DOUBLE},
	                             {"total_hemoglobin_conc_max", LogicalType::DOUBLE},
	                             {"saturated_hemoglobin_percent", LogicalType::DOUBLE},
	                             {"saturated_hemoglobin_percent_min", LogicalType::DOUBLE},
	                             {"saturated_hemoglobin_percent_max", LogicalType::DOUBLE},

	                             // Cadence metrics
	                             {"cadence", LogicalType::UTINYINT}, // uint8_t -> UTINYINT (corrected)
	                             {"cadence256", LogicalType::DOUBLE},
	                             {"fractional_cadence", LogicalType::DOUBLE},

	                             // Temperature
	                             {"temperature", LogicalType::TINYINT}, // int8_t -> TINYINT (corrected)
	                             {"core_temperature", LogicalType::DOUBLE},

	                             // Cycling metrics
	                             {"grade", LogicalType::DOUBLE},
	                             {"resistance", LogicalType::USMALLINT},        // uint16_t -> USMALLINT
	                             {"left_right_balance", LogicalType::UTINYINT}, // uint8_t -> UTINYINT (corrected)
	                             {"left_torque_effectiveness", LogicalType::DOUBLE},
	                             {"right_torque_effectiveness", LogicalType::DOUBLE},
	                             {"left_pedal_smoothness", LogicalType::DOUBLE},
	                             {"right_pedal_smoothness", LogicalType::DOUBLE},
	                             {"combined_pedal_smoothness", LogicalType::DOUBLE},
	                             {"left_pco", LogicalType::TINYINT},  // int8_t -> TINYINT
	                             {"right_pco", LogicalType::TINYINT}, // int8_t -> TINYINT

	                             // Running metrics
	                             {"vertical_oscillation", LogicalType::DOUBLE},
	                             {"stance_time_percent", LogicalType::DOUBLE},
	                             {"stance_time", LogicalType::DOUBLE},
	                             {"stance_time_balance", LogicalType::DOUBLE},
	                             {"step_length", LogicalType::DOUBLE},
	                             {"vertical_ratio", LogicalType::DOUBLE},

	                             // Cycling/Swimming specific
	                             {"cycle_length", LogicalType::DOUBLE},
	                             {"cycle_length16", LogicalType::DOUBLE},
	                             {"cycles", LogicalType::UTINYINT},
	                             {"total_cycles", LogicalType::UINTEGER}, // uint32_t -> UINTEGER

	                             // Navigation and course
	                             {"time_from_course", LogicalType::DOUBLE},
	                             {"gps_accuracy", LogicalType::UTINYINT},

	                             // Energy and calories
	                             {"calories", LogicalType::USMALLINT}, // uint16_t -> USMALLINT (corrected)

	                             // Zones and training
	                             {"zone", LogicalType::UTINYINT},
	                             {"activity_type", LogicalType::VARCHAR},
	                             {"stroke_type", LogicalType::VARCHAR},

	                             // Advanced metrics
	                             {"time128", LogicalType::DOUBLE},
	                             {"grit", LogicalType::DOUBLE},
	                             {"flow", LogicalType::DOUBLE},
	                             {"current_stress", LogicalType::DOUBLE},

	                             // E-bike specific
	                             {"ebike_travel_range", LogicalType::USMALLINT}, // uint16_t -> USMALLINT
	                             {"ebike_battery_level", LogicalType::UTINYINT},
	                             {"ebike_assist_mode", LogicalType::UTINYINT},
	                             {"ebike_assist_level_percent", LogicalType::UTINYINT},
	                             {"battery_soc", LogicalType::DOUBLE},

	                             // Sports specific
	                             {"ball_speed", LogicalType::DOUBLE},

	                             // Diving/Swimming specific
	                             {"absolute_pressure", LogicalType::UINTEGER}, // uint32_t -> UINTEGER
	                             {"depth", LogicalType::DOUBLE},
	                             {"next_stop_depth", LogicalType::DOUBLE},
	                             {"next_stop_time", LogicalType::UINTEGER},  // uint32_t -> UINTEGER
	                             {"time_to_surface", LogicalType::UINTEGER}, // uint32_t -> UINTEGER
	                             {"ndl_time", LogicalType::UINTEGER},        // uint32_t -> UINTEGER
	                             {"cns_load", LogicalType::UTINYINT},
	                             {"n2_load", LogicalType::USMALLINT},           // uint16_t -> USMALLINT
	                             {"air_time_remaining", LogicalType::UINTEGER}, // uint32_t -> UINTEGER
	                             {"pressure_sac", LogicalType::DOUBLE},
	                             {"volume_sac", LogicalType::DOUBLE},
	                             {"rmv", LogicalType::DOUBLE},
	                             {"ascent_rate", LogicalType::DOUBLE},
	                             {"po2", LogicalType::DOUBLE},

	                             // Respiratory
	                             {"respiration_rate", LogicalType::UTINYINT},
	                             {"enhanced_respiration_rate", LogicalType::DOUBLE},

	                             // Device info
	                             {"device_index", LogicalType::UTINYINT},

	                             // File source
	                             {"file_source", LogicalType::VARCHAR}};

	// Extract names and types from the column definitions
	for (const auto &col : columns) {
		names.push_back(col.name);
		return_types.push_back(col.type);
	}

	return make_uniq<FitTableFunctionData>(file_path, "records", &context, FitCompressionParameter(input));
}

void FitRecordsFunction(ClientContext &context, TableFunctionInput &data_p, DataChunk &output) {
	auto &data = (FitTableFunctionData &)*data_p.bind_data;

	idx_t remaining_rows = data.fit_records.size() - data.current_row;
	idx_t rows_to_output = MinValue<idx_t>(remaining_rows, STANDARD_VECTOR_SIZE);

	if (rows_to_output == 0) {
		output.SetCardinality(0);
		return;
	}

	// Fill the output chunk with FIT data
	for (idx_t row = 0; row < rows_to_output; row++) {
		const auto &fit_record = data.fit_records[data.current_row + row];
		idx_t col = 0;

		// Basic timestamp and location
		output.SetValue(col++, row, Value::TIMESTAMPTZ(fit_record.timestamp));

		output.SetValue(col++, row, fit_record.latitude != 0.0 ? Value::DOUBLE(fit_record.latitude) : Value());
		output.SetValue(col++, row, fit_record.longitude != 0.0 ? Value::DOUBLE(fit_record.longitude) : Value());
		output.SetValue(col++, row, fit_record.altitude != 0.0 ? Value::DOUBLE(fit_record.altitude) : Value());
		output.SetValue(col++, row,
		                fit_record.enhanced_altitude != 0.0 ? Value::DOUBLE(fit_record.enhanced_altitude) : Value());

		// Speed and distance
		output.SetValue(col++, row, fit_record.distance > 0.0 ? Value::DOUBLE(fit_record.distance) : Value());
		output.SetValue(col++, row, fit_record.speed > 0.0 ? Value::DOUBLE(fit_record.speed) : Value());
		output.SetValue(col++, row,
		                fit_record.enhanced_speed > 0.0 ? Value::DOUBLE(fit_record.enhanced_speed) : Value());
		output.SetValue(col++, row,
		                fit_record.vertical_speed != 0.0 ? Value::DOUBLE(fit_record.vertical_speed) : Value());

		// Power metrics - corrected to use proper unsigned types
		output.SetValue(col++, row, fit_record.power > 0 ? Value::USMALLINT(fit_record.power) : Value());
		output.SetValue(col++, row, fit_record.motor_power > 0 ? Value::USMALLINT(fit_record.motor_power) : Value());
		output.SetValue(col++, row,
		                fit_record.accumulated_power > 0 ? Value::UINTEGER(fit_record.accumulated_power) : Value());
		output.SetValue(col++, row,
		                fit_record.compressed_accumulated_power > 0
		                    ? Value::USMALLINT(fit_record.compressed_accumulated_power)
		                    : Value());

		// Heart rate and physiological data - corrected to use UTINYINT for heart_rate
		output.SetValue(col++, row, fit_record.heart_rate > 0 ? Value::UTINYINT(fit_record.heart_rate) : Value());
		output.SetValue(col++, row,
		                fit_record.total_hemoglobin_conc > 0.0 ? Value::DOUBLE(fit_record.total_hemoglobin_conc)
		                                                       : Value());
		output.SetValue(col++, row,
		                fit_record.total_hemoglobin_conc_min > 0.0 ? Value::DOUBLE(fit_record.total_hemoglobin_conc_min)
		                                                           : Value());
		output.SetValue(col++, row,
		                fit_record.total_hemoglobin_conc_max > 0.0 ? Value::DOUBLE(fit_record.total_hemoglobin_conc_max)
		                                                           : Value());
		output.SetValue(col++, row,
		                fit_record.saturated_hemoglobin_percent > 0.0
		                    ? Value::DOUBLE(fit_record.saturated_hemoglobin_percent)
		                    : Value());
		output.SetValue(col++, row,
		                fit_record.saturated_hemoglobin_percent_min > 0.0
		                    ? Value::DOUBLE(fit_record.saturated_hemoglobin_percent_min)
		                    : Value());
		output.SetValue(col++, row,
		                fit_record.saturated_hemoglobin_percent_max > 0.0
		                    ? Value::DOUBLE(fit_record.saturated_hemoglobin_percent_max)
		                    : Value());

		// Cadence metrics - corrected to use UTINYINT for cadence
		output.SetValue(col++, row, fit_record.cadence > 0 ? Value::UTINYINT(fit_record.cadence) : Value());
		output.SetValue(col++, row, fit_record.cadence256 > 0.0 ? Value::DOUBLE(fit_record.cadence256) : Value());
		output.SetValue(col++, row,
		                fit_record.fractional_cadence > 0.0 ? Value::DOUBLE(fit_record.fractional_cadence) : Value());

		// Temperature - corrected to use TINYINT for temperature
		output.SetValue(col++, row, fit_record.temperature != 0 ? Value::TINYINT(fit_record.temperature) : Value());
		output.SetValue(col++, row,
		                fit_record.core_temperature != 0.0 ? Value::DOUBLE(fit_record.core_temperature) : Value());

		// Cycling metrics - corrected types
		output.SetValue(col++, row, fit_record.grade != 0.0 ? Value::DOUBLE(fit_record.grade) : Value());
		output.SetValue(col++, row, fit_record.resistance > 0 ? Value::USMALLINT(fit_record.resistance) : Value());
		output.SetValue(col++, row,
		                fit_record.left_right_balance > 0 ? Value::UTINYINT(fit_record.left_right_balance) : Value());
		output.SetValue(col++, row,
		                fit_record.left_torque_effectiveness != 0.0
		                    ? Value::DOUBLE(fit_record.left_torque_effectiveness)
		                    : Value());
		output.SetValue(col++, row,
		                fit_record.right_torque_effectiveness != 0.0
		                    ? Value::DOUBLE(fit_record.right_torque_effectiveness)
		                    : Value());
		output.SetValue(col++, row,
		                fit_record.left_pedal_smoothness != 0.0 ? Value::DOUBLE(fit_record.left_pedal_smoothness)
		                                                        : Value());
		output.SetValue(col++, row,
		                fit_record.right_pedal_smoothness != 0.0 ? Value::DOUBLE(fit_record.right_pedal_smoothness)
		                                                         : Value());
		output.SetValue(col++, row,
		                fit_record.combined_pedal_smoothness != 0.0
		                    ? Value::DOUBLE(fit_record.combined_pedal_smoothness)
		                    : Value());
		output.SetValue(col++, row, fit_record.left_pco != 0 ? Value::TINYINT(fit_record.left_pco) : Value());
		output.SetValue(col++, row, fit_record.right_pco != 0 ? Value::TINYINT(fit_record.right_pco) : Value());

		// Running metrics
		output.SetValue(col++, row,
		                fit_record.vertical_oscillation > 0.0 ? Value::DOUBLE(fit_record.vertical_oscillation)
		                                                      : Value());
		output.SetValue(col++, row,
		                fit_record.stance_time_percent > 0.0 ? Value::DOUBLE(fit_record.stance_time_percent) : Value());
		output.SetValue(col++, row, fit_record.stance_time > 0.0 ? Value::DOUBLE(fit_record.stance_time) : Value());
		output.SetValue(col++, row,
		                fit_record.stance_time_balance > 0.0 ? Value::DOUBLE(fit_record.stance_time_balance) : Value());
		output.SetValue(col++, row, fit_record.step_length > 0.0 ? Value::DOUBLE(fit_record.step_length) : Value());
		output.SetValue(col++, row,
		                fit_record.vertical_ratio > 0.0 ? Value::DOUBLE(fit_record.vertical_ratio) : Value());

		// Cycling/Swimming specific
		output.SetValue(col++, row, fit_record.cycle_length > 0.0 ? Value::DOUBLE(fit_record.cycle_length) : Value());
		output.SetValue(col++, row,
		                fit_record.cycle_length16 > 0.0 ? Value::DOUBLE(fit_record.cycle_length16) : Value());
		output.SetValue(col++, row, fit_record.cycles > 0 ? Value::UTINYINT(fit_record.cycles) : Value());
		output.SetValue(col++, row, fit_record.total_cycles > 0 ? Value::UINTEGER(fit_record.total_cycles) : Value());

		// Navigation and course
		output.SetValue(col++, row,
		                fit_record.time_from_course != 0.0 ? Value::DOUBLE(fit_record.time_from_course) : Value());
		output.SetValue(col++, row, fit_record.gps_accuracy > 0 ? Value::UTINYINT(fit_record.gps_accuracy) : Value());

		// Energy and calories - corrected to use USMALLINT
		output.SetValue(col++, row, fit_record.calories > 0 ? Value::USMALLINT(fit_record.calories) : Value());

		// Zones and training
		output.SetValue(col++, row, fit_record.zone > 0 ? Value::UTINYINT(fit_record.zone) : Value());
		output.SetValue(col++, row, Value(fit_record.activity_type));
		output.SetValue(col++, row, Value(fit_record.stroke_type));

		// Advanced metrics
		output.SetValue(col++, row, fit_record.time128 != 0.0 ? Value::DOUBLE(fit_record.time128) : Value());
		output.SetValue(col++, row, fit_record.grit != 0.0 ? Value::DOUBLE(fit_record.grit) : Value());
		output.SetValue(col++, row, fit_record.flow != 0.0 ? Value::DOUBLE(fit_record.flow) : Value());
		output.SetValue(col++, row,
		                fit_record.current_stress != 0.0 ? Value::DOUBLE(fit_record.current_stress) : Value());

		// E-bike specific - corrected to use USMALLINT for ebike_travel_range
		output.SetValue(col++, row,
		                fit_record.ebike_travel_range > 0 ? Value::USMALLINT(fit_record.ebike_travel_range) : Value());
		output.SetValue(col++, row,
		                fit_record.ebike_battery_level > 0 ? Value::UTINYINT(fit_record.ebike_battery_level) : Value());
		output.SetValue(col++, row,
		                fit_record.ebike_assist_mode > 0 ? Value::UTINYINT(fit_record.ebike_assist_mode) : Value());
		output.SetValue(col++, row,
		                fit_record.ebike_assist_level_percent > 0
		                    ? Value::UTINYINT(fit_record.ebike_assist_level_percent)
		                    : Value());
		output.SetValue(col++, row, fit_record.battery_soc > 0.0 ? Value::DOUBLE(fit_record.battery_soc) : Value());

		// Sports specific
		output.SetValue(col++, row, fit_record.ball_speed > 0.0 ? Value::DOUBLE(fit_record.ball_speed) : Value());

		// Diving/Swimming specific - corrected to use proper unsigned types
		output.SetValue(col++, row,
		                fit_record.absolute_pressure > 0 ? Value::UINTEGER(fit_record.absolute_pressure) : Value());
		output.SetValue(col++, row, fit_record.depth > 0.0 ? Value::DOUBLE(fit_record.depth) : Value());
		output.SetValue(col++, row,
		                fit_record.next_stop_depth > 0.0 ? Value::DOUBLE(fit_record.next_stop_depth) : Value());
		output.SetValue(col++, row,
		                fit_record.next_stop_time > 0 ? Value::UINTEGER(fit_record.next_stop_time) : Value());
		output.SetValue(col++, row,
		                fit_record.time_to_surface > 0 ? Value::UINTEGER(fit_record.time_to_surface) : Value());
		output.SetValue(col++, row, fit_record.ndl_time > 0 ? Value::UINTEGER(fit_record.ndl_time) : Value());
		output.SetValue(col++, row, fit_record.cns_load > 0 ? Value::UTINYINT(fit_record.cns_load) : Value());
		output.SetValue(col++, row, fit_record.n2_load > 0 ? Value::USMALLINT(fit_record.n2_load) : Value());
		output.SetValue(col++, row,
		                fit_record.air_time_remaining > 0 ? Value::UINTEGER(fit_record.air_time_remaining) : Value());
		output.SetValue(col++, row, fit_record.pressure_sac > 0.0 ? Value::DOUBLE(fit_record.pressure_sac) : Value());
		output.SetValue(col++, row, fit_record.volume_sac > 0.0 ? Value::DOUBLE(fit_record.volume_sac) : Value());
		output.SetValue(col++, row, fit_record.rmv > 0.0 ? Value::DOUBLE(fit_record.rmv) : Value());
		output.SetValue(col++, row, fit_record.ascent_rate != 0.0 ? Value::DOUBLE(fit_record.ascent_rate) : Value());
		output.SetValue(col++, row, fit_record.po2 > 0.0 ? Value::DOUBLE(fit_record.po2) : Value());

		// Respiratory
		output.SetValue(col++, row,
		                fit_record.respiration_rate > 0 ? Value::UTINYINT(fit_record.respiration_rate) : Value());
		output.SetValue(col++, row,
		                fit_record.enhanced_respiration_rate > 0.0 ? Value::DOUBLE(fit_record.enhanced_respiration_rate)
		                                                           : Value());

		// Device info
		output.SetValue(col++, row, fit_record.device_index > 0 ? Value::UTINYINT(fit_record.device_index) : Value());

		// File source
		output.SetValue(col++, row, !fit_record.file_source.empty() ? Value(fit_record.file_source) : Value());
	}

	output.SetCardinality(rows_to_output);
	data.current_row += rows_to_output;
}

} // namespace duckdb
