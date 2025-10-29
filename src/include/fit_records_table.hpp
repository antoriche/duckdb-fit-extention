#pragma once

#include "duckdb.hpp"
#include "duckdb/function/table_function.hpp"

namespace duckdb {

// FIT Records table function - main time-series data
unique_ptr<FunctionData> FitRecordsBind(ClientContext &context, TableFunctionBindInput &input,
                                        vector<LogicalType> &return_types, vector<string> &names);

void FitRecordsFunction(ClientContext &context, TableFunctionInput &data_p, DataChunk &output);

} // namespace duckdb
