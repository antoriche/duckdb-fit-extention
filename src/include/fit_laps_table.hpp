#pragma once

#include "duckdb.hpp"
#include "duckdb/function/table_function.hpp"

namespace duckdb {

unique_ptr<FunctionData> FitLapsBind(ClientContext &context, TableFunctionBindInput &input,
                                     vector<LogicalType> &return_types, vector<string> &names);

void FitLapsFunction(ClientContext &context, TableFunctionInput &data_p, DataChunk &output);

} // namespace duckdb
