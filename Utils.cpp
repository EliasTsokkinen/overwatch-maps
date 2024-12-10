#include "utils.h"

void Utils::init(std::ostream& output, const std::string& main_file_name)
{
	// Tarkistaa onko tiedostoa olemassa
	if (fileExists(main_file_name)) {
		if (!mapverUpToDate(main_file_name, getMapver())) {
			output << "Saved map version out of date!" << std::endl;
			output << "TODO: Update file." << std::endl;
		}
		return;
	}
	// tiedostoa ei löytynyt
	output << "No save file found. Initializing...\n" << std::endl;
	output << "If you think you should have a save file, look in the previous "
		"directory this .exe was in for a file called \".data.owmd\" and move "
		"it here. If this is your first time using this program, welcome!\n"
		"To get started, type in \"help\" and press Enter.\n"<< std::endl;
	File::createMainFile(output, main_file_name);
	output << "File initialized." << std::endl;
}


bool Utils::fileExists(const std::string& file_name)
{
	std::ifstream dataFile(file_name);
	if (dataFile) return true;
	else return false;
}

bool Utils::mapverUpToDate(const std::string& file_name,
	const std::string& mapver)
{
	std::ifstream dataFile(file_name);

	std::string row = "";
	bool outdated = true;
	while (std::getline(dataFile, row) && row.at(0) == '#') {
		if (row == "#mpv%" + getMapver()) {
			return true;
		}
	}
	return false;
}



int Utils::getNumberOfMapTypes(const std::vector<std::pair<std::string,
	std::string>>&meta)
{
	for (auto meta : meta) {
		if (meta.first == "#mpt") {
			return stoi(meta.second);
		}
	}
	return 0;
}

Map* Utils::getMapType(const std::string& maptype_name,
	const std::vector<std::shared_ptr<Map>>& maptypes)
{
	for (std::shared_ptr<Map> mapt : maptypes) {
		if (mapt->getName() == maptype_name) {
			return &(*mapt);
		}
	}
	return nullptr;
}


std::vector<std::string> Utils::split(const std::string& to_split,
	const char splitter,
	const char compound)
{
	std::vector<std::string> to_return = { "" };
	bool in_compound = false;
	for (char c : to_split) {
		if (c == compound) {
			in_compound = not in_compound;
		}
		else if (c == splitter && not in_compound) {
			to_return.push_back("");
		}
		else {
			to_return.back().push_back(c);
		}
	}
	return to_return;
}

std::string Utils::in_lower(const std::string& str)
{
	std::string in_lower = str;
	for (char& c : in_lower) {
		c = std::tolower(c);
	}
	return in_lower;
}


std::map<std::string, std::vector<std::string>> Utils::getMaps()
{
	return {
	{"Clash",
		{"Hanaoka",
		"Throne of Anubis"}},
	{"Control",
		{"Antarctic Peninsula",
		"Busan",
		"Ilios",
		"Lijiang Tower",
		"Nepal",
		"Oasis",
		"Samoa"}},
	{"Escort",
		{"Circuit Royal",
		"Dorado",
		"Havana",
		"Junkertown",
		"Rialto",
		"Route 66",
		"Shambali Monastery",
		"Watchpoint: Gibraltar"}},
	{"Flashpoint",
		{"New Junk City",
		"Suravasa"}},
	{"Hybrid",
		{"Blizzard World",
		"Eichenwalde",
		"Hollywood",
		"King's Row",
		"Midtown",
		"Numbani",
		"Paraiso"}},
	{"Push",
		{"Colosseo",
		"Esperanca",
		"New Queen Street",
		"Runasapi"}}
	};
}

std::string Utils::getMapver()
{
	return "13.0";
}

std::time_t Utils::getTime()
{
	auto now = std::chrono::system_clock::now();
	return std::chrono::system_clock::to_time_t(now);
}

std::string::size_type Utils::lengthOfLongestMapName(
	const std::vector<std::set<std::string>>& input)
{
	std::string::size_type longest = 0;
	std::string::size_type pos;
	std::string map;
	for (std::set<std::string> set : input) {
		for (std::string str : set) {
			pos = str.find("/");
			while (str.at(pos) != ' ' and pos >= 0) {
				--pos;
			}
			map = str.substr(0, pos);
			if (map.length() > longest) {
				longest = map.length();
			}
		}
	}
	return longest;
}

std::string Utils::removeSpecials(const std::string& input)
{
	const std::vector<char> TO_REMOVE = { ' ', '\'', '\\', '/', '"', ':' };

	std::string output = input;

	for (const char c : TO_REMOVE) {
		output.erase(remove(output.begin(), output.end(), c), output.end());
	}

	return output;
}

std::string Utils::format(const std::string& input)
{
	return removeSpecials(in_lower(input));
}

std::string Utils::lookForAlias(const std::string& input)
{
	std::map<std::string, std::vector<std::string>> aliases = {
		{"Hanaoka", {"Hana", "Hanamura"}},
		{"Throne of Anubis", {"Anubis", "ToA", "TA"}},
		{"Antarctic Peninsula", {"Antarctic", "Antarctica", "AP"}},
		{"Busan", {}},
		{"Ilios", {}},
		{"Lijiang Tower", {"Lijiang", "Lijang", "LT", "Lijiang"}},
		{"Nepal", {"Nipple"}},
		{"Oasis", {}},
		{"Samoa", {}},
		{"Circuit Royal", {"Circuit Royale", "Circuit", "Circ", "CR"}},
		{"Dorado", {}},
		{"Havana", {}},
		{"Junkertown", {"Junker", "JT"}},
		{"Rialto", {}},
		{"Route 66", {"Route", "R66", "Route 6", "R6"}},
		{"Shambali Monastery", {"Shambali", "SM"}},
		{"Watchpoint: Gibraltar", {"Gibraltar", "Gib", "Watchpoint", "WG"}},
		{"New Junk City", {"NJC", "New Junk", "Junk City", "New Junk Shitty"}},
		{"Suravasa", {}},
		{"Blizzard World", {"Blizz World", "Blizz", "Blizzard", "BW"}},
		{"Eichenwalde", {"Eich"}},
		{"Hollywood", {"HW"}},
		{"King's Row", {"Kings", "KR"}},
		{"Midtown", {}},
		{"Numbani", {}},
		{"Paraiso", {}},
		{"Colosseo", {}},
		{"Esperanca", {"Esparanca", "Esperansa", "Esparansa"}},
		{"New Queen Street", {"NQS", "Queens", "Queen Street", "New Queens"}},
		{"Runasapi", {}}
	};

	std::string user_input = removeSpecials(in_lower(input));
	std::string kas_alias;

	for (auto map : aliases) {
		for (auto alias : map.second) {
			kas_alias = removeSpecials(in_lower(alias));
			if (user_input == kas_alias) {
				return map.first;
			}
		}
	}
	return input;
}
