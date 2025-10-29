#define DUCKDB_EXTENSION_MAIN

#include "fit_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/function/scalar_function.hpp"
#include "duckdb/function/table_function.hpp"
#include <duckdb/parser/parsed_data/create_scalar_function_info.hpp>

// OpenSSL linked through vcpkg
#include <openssl/opensslv.h>

// Include all table function headers
#include "include/fit_records_table.hpp"
#include "include/fit_activities_table.hpp"
#include "include/fit_sessions_table.hpp"
#include "include/fit_laps_table.hpp"
#include "include/fit_devices_table.hpp"
#include "include/fit_events_table.hpp"
#include "include/fit_users_table.hpp"

namespace duckdb {

// Scalar function for OpenSSL version
inline void FitOpenSSLVersionScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
	auto &name_vector = args.data[0];
	UnaryExecutor::Execute<string_t, string_t>(name_vector, result, args.size(), [&](string_t name) {
		return StringVector::AddString(result, "OpenSSL " + string(OPENSSL_VERSION_TEXT));
	});
}

static void LoadInternal(ExtensionLoader &loader) {
	// Register the 7 FIT table functions corresponding to the 7 tables in documentation

	// 1. Time-series records table (original 'fit' function)
	TableFunction fit_records_function("fit_records", {LogicalType::VARCHAR}, FitRecordsFunction, FitRecordsBind);
	loader.RegisterFunction(fit_records_function);

	// Keep original 'fit' function name for backward compatibility
	TableFunction fit_table_function("fit", {LogicalType::VARCHAR}, FitRecordsFunction, FitRecordsBind);
	loader.RegisterFunction(fit_table_function);

	// 2. Activities metadata table
	TableFunction fit_activities_function("fit_activities", {LogicalType::VARCHAR}, FitActivitiesFunction,
	                                      FitActivitiesBind);
	loader.RegisterFunction(fit_activities_function);

	// 3. Sessions table
	TableFunction fit_sessions_function("fit_sessions", {LogicalType::VARCHAR}, FitSessionsFunction, FitSessionsBind);
	loader.RegisterFunction(fit_sessions_function);

	// 4. Laps table
	TableFunction fit_laps_function("fit_laps", {LogicalType::VARCHAR}, FitLapsFunction, FitLapsBind);
	loader.RegisterFunction(fit_laps_function);

	// 5. Device information table
	TableFunction fit_devices_function("fit_devices", {LogicalType::VARCHAR}, FitDevicesFunction, FitDevicesBind);
	loader.RegisterFunction(fit_devices_function);

	// 6. Events table
	TableFunction fit_events_function("fit_events", {LogicalType::VARCHAR}, FitEventsFunction, FitEventsBind);
	loader.RegisterFunction(fit_events_function);

	// 7. User profile table
	TableFunction fit_users_function("fit_users", {LogicalType::VARCHAR}, FitUsersFunction, FitUsersBind);
	loader.RegisterFunction(fit_users_function);

	// Register scalar function
	auto fit_openssl_version_scalar_function =
	    ScalarFunction("fit_openssl_version", {LogicalType::VARCHAR}, LogicalType::VARCHAR, FitOpenSSLVersionScalarFun);
	loader.RegisterFunction(fit_openssl_version_scalar_function);
}

void FitExtension::Load(ExtensionLoader &loader) {
	LoadInternal(loader);
}

std::string FitExtension::Name() {
	return "fit";
}

std::string FitExtension::Version() const {
#ifdef EXT_VERSION_FIT
	return EXT_VERSION_FIT;
#else
	return "";
#endif
}

} // namespace duckdb

extern "C" {

DUCKDB_CPP_EXTENSION_ENTRY(fit, loader) {
	duckdb::LoadInternal(loader);
}
}
