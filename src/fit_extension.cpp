#define DUCKDB_EXTENSION_MAIN

#include "fit_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/function/scalar_function.hpp"
#include "duckdb/function/table_function.hpp"

// Per-table function declarations
#include "include/fit_records_table.hpp"
#include "include/fit_activities_table.hpp"
#include "include/fit_sessions_table.hpp"
#include "include/fit_laps_table.hpp"
#include "include/fit_devices_table.hpp"
#include "include/fit_events_table.hpp"
#include "include/fit_users_table.hpp"

// OpenSSL linked through vcpkg
#include <openssl/opensslv.h>

namespace duckdb {

inline void FitOpenSSLVersionScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
	auto &name_vector = args.data[0];
	UnaryExecutor::Execute<string_t, string_t>(name_vector, result, args.size(), [&](string_t name) {
		return StringVector::AddString(result, "Fit " + name.GetString() + ", my linked OpenSSL version is " +
		                                           OPENSSL_VERSION_TEXT);
	});
}

// Every FIT table function takes a single path and the same optional `compression` override.
static void RegisterFitTableFunction(ExtensionLoader &loader, const string &name, table_function_t function,
                                     table_function_bind_t bind) {
	TableFunction table_function(name, {LogicalType::VARCHAR}, function, bind);
	table_function.named_parameters["compression"] = LogicalType::VARCHAR;
	loader.RegisterFunction(table_function);
}

static void LoadInternal(ExtensionLoader &loader) {
	// Register the 7 FIT table functions corresponding to the 7 tables in documentation
	RegisterFitTableFunction(loader, "fit_records", FitRecordsFunction, FitRecordsBind);
	// Keep original 'fit' function name for backward compatibility
	RegisterFitTableFunction(loader, "fit", FitRecordsFunction, FitRecordsBind);
	RegisterFitTableFunction(loader, "fit_activities", FitActivitiesFunction, FitActivitiesBind);
	RegisterFitTableFunction(loader, "fit_sessions", FitSessionsFunction, FitSessionsBind);
	RegisterFitTableFunction(loader, "fit_laps", FitLapsFunction, FitLapsBind);
	RegisterFitTableFunction(loader, "fit_devices", FitDevicesFunction, FitDevicesBind);
	RegisterFitTableFunction(loader, "fit_events", FitEventsFunction, FitEventsBind);
	RegisterFitTableFunction(loader, "fit_users", FitUsersFunction, FitUsersBind);

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
