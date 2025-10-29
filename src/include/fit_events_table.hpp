#pragma once

#include "duckdb.hpp"
#include "duckdb/function/table_function.hpp"

namespace duckdb {

unique_ptr<FunctionData> FitEventsBind(ClientContext &context, TableFunctionBindInput &input,
                                       vector<LogicalType> &return_types, vector<string> &names);

void FitEventsFunction(ClientContext &context, TableFunctionInput &data_p, DataChunk &output);

} // namespace duckdb
