#include "include/fit_glob.hpp"
#include <algorithm>
#include <fnmatch.h>
#include <dirent.h>
#include <sys/stat.h>
#include <cstring>

namespace duckdb {

// Helper function to expand glob patterns
vector<string> ExpandGlobPattern(const string &pattern) {
	vector<string> files;

	// Check if pattern contains wildcards
	if (pattern.find('*') == string::npos && pattern.find('?') == string::npos && pattern.find('[') == string::npos) {
		// No wildcards, just return the single file
		files.push_back(pattern);
		return files;
	}

	// Extract directory and filename pattern
	size_t last_slash = pattern.find_last_of('/');
	string dir_path;
	string filename_pattern;

	if (last_slash != string::npos) {
		dir_path = pattern.substr(0, last_slash);
		filename_pattern = pattern.substr(last_slash + 1);
	} else {
		dir_path = ".";
		filename_pattern = pattern;
	}

	// Open directory
	DIR *dir = opendir(dir_path.c_str());
	if (!dir) {
		return files; // Return empty vector if directory doesn't exist
	}

	// Read directory entries
	struct dirent *entry;
	while ((entry = readdir(dir)) != nullptr) {
		// Skip . and .. entries
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
			continue;
		}

		// Check if it matches the pattern
		if (fnmatch(filename_pattern.c_str(), entry->d_name, 0) == 0) {
			// Construct full path
			string full_path;
			if (dir_path == ".") {
				full_path = entry->d_name;
			} else {
				full_path = dir_path + "/" + entry->d_name;
			}

			// Check if it's a regular file
			struct stat statbuf;
			if (stat(full_path.c_str(), &statbuf) == 0 && S_ISREG(statbuf.st_mode)) {
				files.push_back(full_path);
			}
		}
	}

	closedir(dir);

	// Sort files for consistent ordering
	std::sort(files.begin(), files.end());

	return files;
}

} // namespace duckdb
