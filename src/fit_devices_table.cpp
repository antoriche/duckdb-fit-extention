#include "include/fit_devices_table.hpp"
#include "include/fit_table_base.hpp"

namespace duckdb {

unique_ptr<FunctionData> FitDevicesBind(ClientContext &context, TableFunctionBindInput &input,
                                        vector<LogicalType> &return_types, vector<string> &names) {
	auto file_path = input.inputs[0].GetValue<string>();

	// Define device columns based on FitDevice structure
	names = {"device_id",      "activity_id",     "device_index",     "device_type",           "manufacturer",
	         "product",        "serial_number",   "software_version", "hardware_version",      "cum_operating_time",
	         "battery_status", "sensor_position", "descriptor",       "ant_transmission_type", "ant_device_number",
	         "ant_network",    "source_type",     "product_name",     "battery_voltage",       "file_source"};

	return_types = {LogicalType::UINTEGER, LogicalType::UBIGINT,  LogicalType::UTINYINT,  LogicalType::VARCHAR,
	                LogicalType::VARCHAR,  LogicalType::VARCHAR,  LogicalType::UBIGINT,   LogicalType::VARCHAR,
	                LogicalType::VARCHAR,  LogicalType::UINTEGER, LogicalType::VARCHAR,   LogicalType::VARCHAR,
	                LogicalType::VARCHAR,  LogicalType::UTINYINT, LogicalType::USMALLINT, LogicalType::VARCHAR,
	                LogicalType::VARCHAR,  LogicalType::VARCHAR,  LogicalType::DOUBLE,    LogicalType::VARCHAR};

	return make_uniq<FitTableFunctionData>(file_path, "devices", &context);
}

void FitDevicesFunction(ClientContext &context, TableFunctionInput &data_p, DataChunk &output) {
	auto &data = (FitTableFunctionData &)*data_p.bind_data;

	idx_t remaining_rows = data.fit_devices.size() - data.current_row;
	idx_t rows_to_output = MinValue<idx_t>(remaining_rows, STANDARD_VECTOR_SIZE);

	if (rows_to_output == 0) {
		output.SetCardinality(0);
		return;
	}

	// Fill the output chunk with device data
	for (idx_t row = 0; row < rows_to_output; row++) {
		const auto &fit_device = data.fit_devices[data.current_row + row];
		idx_t col = 0;

		output.SetValue(col++, row, Value::UINTEGER(fit_device.device_id));
		output.SetValue(col++, row, Value::UBIGINT(fit_device.activity_id));
		output.SetValue(col++, row, Value::UTINYINT(fit_device.device_index));
		output.SetValue(col++, row, Value(fit_device.device_type));
		output.SetValue(col++, row, Value(fit_device.manufacturer));
		output.SetValue(col++, row, Value(fit_device.product));
		output.SetValue(col++, row, fit_device.serial_number > 0 ? Value::UBIGINT(fit_device.serial_number) : Value());
		output.SetValue(col++, row, Value(fit_device.software_version));
		output.SetValue(col++, row, Value(fit_device.hardware_version));
		output.SetValue(col++, row,
		                fit_device.cum_operating_time > 0 ? Value::UINTEGER(fit_device.cum_operating_time) : Value());
		output.SetValue(col++, row, Value(fit_device.battery_status));
		output.SetValue(col++, row, Value(fit_device.sensor_position));
		output.SetValue(col++, row, Value(fit_device.descriptor));
		output.SetValue(col++, row,
		                fit_device.ant_transmission_type > 0 ? Value::UTINYINT(fit_device.ant_transmission_type)
		                                                     : Value());
		output.SetValue(col++, row,
		                fit_device.ant_device_number > 0 ? Value::USMALLINT(fit_device.ant_device_number) : Value());
		output.SetValue(col++, row, Value(fit_device.ant_network));
		output.SetValue(col++, row, Value(fit_device.source_type));
		output.SetValue(col++, row, Value(fit_device.product_name));
		output.SetValue(col++, row,
		                fit_device.battery_voltage > 0.0 ? Value::DOUBLE(fit_device.battery_voltage) : Value());
		output.SetValue(col++, row, !fit_device.file_source.empty() ? Value(fit_device.file_source) : Value());
	}

	output.SetCardinality(rows_to_output);
	data.current_row += rows_to_output;
}

} // namespace duckdb
