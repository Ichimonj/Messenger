#pragma once
#include <filesystem>
#include "file_names.hpp"
#include <string>
using namespace std;
class WorkWithFiles {
public:
	static inline void initAllDir() noexcept {
		filesystem::create_directories(main_dir);
	}
	static inline void createUserDir(const uint64_t user_ID) noexcept {
		filesystem::create_directories(main_dir"/" + to_string(user_ID));
	}
};