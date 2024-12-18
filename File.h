#pragma once
#include "Map.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <memory>

struct fileData {
	std::shared_ptr<Map> total_;
	std::vector<std::shared_ptr<Map>> maptypes_;
	std::vector<std::shared_ptr<Map>> maps_;
	std::vector<std::pair<std::string, std::string>> meta_;
	std::vector<std::pair<std::string, std::string>> pref_;
	std::vector<std::string> history_;
};

namespace File {
// Luo päätiedoston
// overwrite luoko tyhjän päätiedoston, vaikka sellainen löytyisi
void createMainFile(std::ostream& output,
	const std::string& main_file_name,
	const bool overwrite = false);

// Lukee tiedoston
fileData readFile(const std::string& file_name);

bool save(const std::string& file_name, const fileData& data);

bool exportToCsv(const std::string& file_name, const fileData& data);

}