#define DUCKDB_EXTENSION_MAIN

#include "fit_extension.hpp"
#include "duckdb.hpp"
#include "duckdb/common/exception.hpp"
#include "duckdb/function/scalar_function.hpp"
#include <duckdb/parser/parsed_data/create_scalar_function_info.hpp>

// OpenSSL linked through vcpkg
#include <openssl/opensslv.h>

namespace duckdb {

inline void FitScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
	auto &name_vector = args.data[0];
	UnaryExecutor::Execute<string_t, string_t>(name_vector, result, args.size(), [&](string_t name) {
		return StringVector::AddString(result, "Fit " + name.GetString() + " 🐥");
	});
}

inline void FitOpenSSLVersionScalarFun(DataChunk &args, ExpressionState &state, Vector &result) {
	auto &name_vector = args.data[0];
	UnaryExecutor::Execute<string_t, string_t>(name_vector, result, args.size(), [&](string_t name) {
		return StringVector::AddString(result, "Fit " + name.GetString() + ", my linked OpenSSL version is " +
		                                           OPENSSL_VERSION_TEXT);
	});
}

static void LoadInternal(ExtensionLoader &loader) {
	// Register a scalar function
	auto fit_scalar_function = ScalarFunction("fit", {LogicalType::VARCHAR}, LogicalType::VARCHAR, FitScalarFun);
	loader.RegisterFunction(fit_scalar_function);

	// Register another scalar function
	auto fit_openssl_version_scalar_function = ScalarFunction("fit_openssl_version", {LogicalType::VARCHAR},
	                                                            LogicalType::VARCHAR, FitOpenSSLVersionScalarFun);
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
