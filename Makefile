PROJ_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# Configuration of extension
EXT_NAME=fit
EXT_CONFIG=${PROJ_DIR}extension_config.cmake

# Include the Makefile from extension-ci-tools
include extension-ci-tools/makefiles/duckdb_extension.Makefile

# Override tidy-check to exclude fit-sdk directory (third-party code)
tidy-check:
	mkdir -p ./build/tidy
	cmake $(GENERATOR) $(BUILD_FLAGS) $(EXT_DEBUG_FLAGS) -DDISABLE_UNITY=1 -DCLANG_TIDY=1 -S $(DUCKDB_SRCDIR) -B build/tidy
	cp duckdb/.clang-tidy build/tidy/.clang-tidy
	cd build/tidy && python3 ../../duckdb/scripts/run-clang-tidy.py '$(PROJ_DIR)src/(?!fit-sdk).*/' -header-filter '$(PROJ_DIR)src/(?!fit-sdk).*/' -quiet ${TIDY_THREAD_PARAMETER} ${TIDY_BINARY_PARAMETER} ${TIDY_PERFORM_CHECKS}
