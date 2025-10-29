#pragma once

#include "duckdb.hpp"
#include <vector>

namespace duckdb {

// Helper function to expand glob patterns
vector<string> ExpandGlobPattern(const string &pattern);

} // namespace duckdb
