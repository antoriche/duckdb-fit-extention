#pragma once

#include "duckdb.hpp"
#include "duckdb/function/table_function.hpp"

namespace duckdb {

// FIT Activities table function - activity metadata
unique_ptr<FunctionData> FitActivitiesBind(ClientContext &context, TableFunctionBindInput &input,
                                           vector<LogicalType> &return_types, vector<string> &names);

void FitActivitiesFunction(ClientContext &context, TableFunctionInput &data_p, DataChunk &output);

} // namespace duckdb
