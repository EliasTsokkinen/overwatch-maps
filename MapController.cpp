#include "MapController.h"

MapController::MapController(std::string file_name): file_name_(file_name)
{
	readFileMC();
}

void MapController::readFileMC()
{
	fileData data = File::readFile(file_name_);
	total_ = data.total_;
	maptypes_ = data.maptypes_;
	maps_ = data.maps_;
	meta_ = data.meta_;
	pref_ = data.pref_;
	history_ = data.history_;
	updatePref();
}

bool MapController::didFileRead()
{
	for (std::pair<std::string, std::string> meta : meta_) {
		if (meta.first == "#red" && meta.second == "fail") {
			return false;
		}
	}
	return true;
}

void MapController::addWin(const std::vector<std::string>& params,
	std::ostream& output)
{
	Map* target = findMap(params.at(0));
	if (target == nullptr) {
		printMapNotFound(params.at(0), output);
		return;
	}
	target->addWin(output);
	addToHistory("addWin \"" + target->getName() + "\"");
	checkAndAutoSave();
}

void MapController::addLoss(const std::vector<std::string>& params,
	std::ostream& output)
{
	Map* target = findMap(params.at(0));
	if (target == nullptr) {
		printMapNotFound(params.at(0), output);
		return;
	}
	target->addLoss(output);
	addToHistory("addLoss \"" + target->getName() + "\"");
	checkAndAutoSave();
}

void MapController::addDraw(const std::vector<std::string>& params,
	std::ostream& output)
{
	Map* target = findMap(params.at(0));
	if (target == nullptr) {
		printMapNotFound(params.at(0), output);
		return;
	}
	target->addDraw(output);
	addToHistory("addDraw \"" + target->getName() + "\"");
	checkAndAutoSave();
}

void MapController::delWin(const std::vector<std::string>& params, std::ostream& output)
{
	Map* target = findMap(params.at(0));
	if (target == nullptr) {
		printMapNotFound(params.at(0), output);
		return;
	}
	target->delWin(output);
	addToHistory("delWin \"" + target->getName() + "\"");
	checkAndAutoSave();
}

void MapController::delLoss(const std::vector<std::string>& params, std::ostream& output)
{
	Map* target = findMap(params.at(0));
	if (target == nullptr) {
		printMapNotFound(params.at(0), output);
		return;
	}
	target->delLoss(output);
	addToHistory("delLoss \"" + target->getName() + "\"");
	checkAndAutoSave();
}

void MapController::delDraw(const std::vector<std::string>& params, std::ostream& output)
{
	Map* target = findMap(params.at(0));
	if (target == nullptr) {
		printMapNotFound(params.at(0), output);
		return;
	}
	target->delDraw(output);
	addToHistory("delDraw \"" + target->getName() + "\"");
	checkAndAutoSave();
}

void MapController::printAll(const std::vector<std::string>& params,
	std::ostream& output)
{
	int longest = longestMapName();
	int longest_wld = longestWLD();

	std::vector<std::set<std::string>> alloutputs =
	{ outputTotal(longest, longest_wld),
		outputMapTypes(longest, longest_wld),
		outputMaps(longest, longest_wld) };

	// total
	for (std::string line : alloutputs.at(0)) {
		output << line << std::endl;
	}

	output << "\n";

	// maptypes
	for (std::string line : alloutputs.at(1)) {
		output << line << std::endl;
	}

	output << "\n";

	// maps
	for (std::string line : alloutputs.at(2)) {
		output << line << std::endl;
	}

	addToHistory("printAll");
}

void MapController::printTotal(const std::vector<std::string>& params,
	std::ostream& output)
{
	for (auto op : outputTotal(longestMapName("total"),
							   longestWLD("total"))) {
		output << op << std::endl;
	}
	addToHistory("printTotal");
}

void MapController::printMap(const std::vector<std::string>& params,
	std::ostream& output)
{
	Map* target = findMap(params.at(0));

	if (target == nullptr) {
		printMapNotFound(params.at(0), output);
		return;
	}

	for (auto op : outputMap(target,
				   longestMapName(params.at(0)),
				   longestWLD(params.at(0)))) {
		output << op << std::endl;
	}
	addToHistory("printMap \"" + target->getName() + "\"");
}

void MapController::printMapType(const std::vector<std::string>& params,
	std::ostream& output)
{
	Map* target = findMap(params.at(0), true);
	if (target == nullptr) {
		printMapTypeNotFound(params.at(0), output);
		return;
	}


	std::string::size_type longest =
		longestMapName(params.at(0));

	std::string::size_type longest_wld =
		longestWLD(params.at(0));

	std::vector<std::set<std::string>> alloutputs 
		= { outputMap(target, longest, longest_wld),
		outputChildren(target, longest, longest_wld) };

	// itse maptype
	for (std::string line : alloutputs.at(0)) {
		output << line << std::endl;
	}

	output << "\n";

	// lapset
	for (std::string line : alloutputs.at(1)) {
		output << line << std::endl;
	}

	addToHistory("printMapType \"" + target->getName() + "\"");
}

void MapController::printMapTypes(const std::vector<std::string>& params,
	std::ostream& output)
{
	std::string::size_type longest =
		longestMapName("maptypes");

	std::string::size_type longest_wld =
		longestWLD("maptypes");

	for (std::string line : outputMapTypes(longest, longest_wld)) {
		output << line << std::endl;
	}

	addToHistory("printMapTypes");
}

void MapController::manualSave(const std::vector<std::string>& params, std::ostream& output)
{
	save(params, output, false);
	addToHistory("save");
}

void MapController::save(const std::vector<std::string>& params,
	std::ostream& output, const bool autosave)
{
	if (File::save(file_name_, generateFileData())) {
		if (not autosave) {
			output << "Saved!" << std::endl;
		}
	}
	else {
		output << "Error in saving :(\n"
			"PLEASE message me with what you did so I can fix it before it "
			"corrupts someone else's save. Sorry." << std::endl;
	}
	removeUnsavedTag();
}

void MapController::exportToCsv(const std::vector<std::string>& params,
	std::ostream& output)
{
	if (File::exportToCsv(params.at(0), generateFileData())) {
		output << "Exported succesfully as "
			<< params.at(0) << ".csv and saved in this directory" << std::endl;
		addToHistory("export \"" + params.at(0) + "\"");
	}
	else {
		output << "Error in exporting :(\n"
			"Please make sure you do not already have a file with the same name "
			"in this directory. For safety reason will not overwrite existing "
			"files!\nIf that is not the case PLEASE message me with what you "
			"did so I can fix it before it corrupts someone's save :)"<< std::endl;
	}
}

void MapController::history(const std::vector<std::string>& params, std::ostream& output)
{
	int i = 1;

	int chars_in_longest_number = 0;
	for (char c : std::to_string(history_.size())) {
		chars_in_longest_number++;
	}

	int chars_in_longest_action = 0;
	for (std::string line : history_) {
		std::vector<std::string> split = Utils::split(line, '?');
		if (chars_in_longest_action < split.at(0).size()) {
			chars_in_longest_action = split.at(0).size();
		}
	}

	for (std::string line : history_) {

		std::vector<std::string> split = Utils::split(line, '?');
		std::string action = split.at(0);
		std::string time = split.at(1);

		std::string start;

		// Numero
		start.append(std::to_string(i) + ".");

		// Välilyöntejä, jotta komennot jäsentyvät samalle kohtaa riviä
		while (start.size() < chars_in_longest_number 
			+ 1 // koska piste
			+ paddingInHistoryAfterNumber()) {
			start.append(" ");
		}

		// Komento
		start.append(action);

		// Välilyöntejä, jotta timestamp osuu samalle kohtaa riviä
		while (start.size() < chars_in_longest_number
			+ chars_in_longest_action
			+ 1 //koska piste
			+ paddingInHistoryAfterNumber()
			+ paddingInHistoryAfterCommand()) {
			start.append(" ");
		}

		output << start << "(" << time << ")" << std::endl;
		++i;
	}
	addToHistory("history");
}

std::string MapController::previousCommand(const int& n) const
{
	if (n > history_.size()) {
		return "Command not found.";
	}

	return Utils::split(history_.at(n - 1), '?', '"', true).at(0);
}

bool MapController::hasUnsavedChanges() const
{
	return unsavedChanges_;
}

std::vector<std::pair<std::string, std::string>> MapController::getPreferences() const
{
	return pref_;
}

void MapController::updatePreferences(std::vector<std::pair<std::string, std::string>> newprefs)
{
	pref_ = newprefs;
	if (stateSaveOnPrefEdit()) {
		save({}, std::cout);
		removeUnsavedTag();
		return;
	}
	addUnsavedTag();
}

Map* MapController::findMap(const std::string& mapname, const bool maptype) const
{
	std::string formal_name = Utils::format(Utils::lookForAlias(mapname));

	if (not maptype) {
		for (std::shared_ptr<Map> map : maps_) {
			if (Utils::format(map->getName()) == formal_name) {
				return &(*map);
			}
		}
		return nullptr;
	}

	for (std::shared_ptr<Map> mapt : maptypes_) {
		if (Utils::format(mapt->getName()) == formal_name) {
			return &(*mapt);
		}
	}

	return nullptr;
}

void MapController::printMapNotFound(const std::string& mapname,
	std::ostream& output) const
{
	output << "Error. Map \"" << mapname << "\" not found. Try \"help\"."
		<< std::endl;
}

void MapController::printMapTypeNotFound(const std::string& mapname,
	std::ostream& output) const
{
	output << "Error. Maptype \"" << mapname << "\" not found. Try \"help\"."
		<< std::endl;
}

std::set<std::string> MapController::outputMaps(
	const std::string::size_type& maplen,
	const std::string::size_type& wldlen) const
{
	std::set<std::string> set;
	for (std::shared_ptr<Map> map : maps_) {
		set.insert(map->getOutput(maplen + paddingAfterMapName(), wldlen + paddingAfterWLD()));
	}
	return set;
}

std::set<std::string> MapController::outputMapTypes(
	const std::string::size_type& maplen,
	const std::string::size_type& wldlen) const
{
	std::set<std::string> set;
	for (std::shared_ptr<Map> maptype : maptypes_) {
		set.insert(maptype->getOutput(maplen + paddingAfterMapName(), wldlen + paddingAfterWLD()));
	}
	return set;
}

std::set<std::string> MapController::outputTotal(
	const std::string::size_type& maplen,
	const std::string::size_type& wldlen) const
{
	return { total_->getOutput(maplen + paddingAfterMapName(), wldlen + paddingAfterWLD()) };
}

std::set<std::string> MapController::outputMap(const Map* map,
	const std::string::size_type& maplen,
	const std::string::size_type& wldlen) const
{
	return { map->getOutput(maplen + paddingAfterMapName(), wldlen + paddingAfterWLD()) };
}

std::set<std::string> MapController::outputChildren(const Map* map, 
	const std::string::size_type& maplen,
	const std::string::size_type& wldlen) const
{
	std::set<std::string> to_return;
	for (Map* child : map->getChildren()) {
		to_return.insert(child->getOutput(maplen + paddingAfterMapName(), wldlen + paddingAfterWLD()));
	}
	return to_return;
}

int MapController::longestMapName(const std::string& which_maps)
{
	int longest = 0;

	if (which_maps == "maptypes") {
		for (auto mapt : maptypes_) if (longest < mapt->getName().size())
			longest = mapt->getName().size();
		return longest;
	}

	if (which_maps == "all") {

		longest = total_->getName().size();

		for (auto mapt : maptypes_) if (longest < mapt->getName().size()) 
			longest = mapt->getName().size();

		for (auto map : maps_) if (longest < map->getName().size())
			longest = map->getName().size();

		return longest;
	}

	if (which_maps == "total") {
		return total_->getName().size();
	}

	// Kokeillaan maptype
	Map* mapt = findMap(which_maps, true);
	if (mapt != nullptr) {
		for (Map* map : mapt->getChildren()) 
			if (longest < map->getName().size()) longest = map->getName().size();
		return longest;
	}

	// Kokeillaan map
	Map* map = findMap(which_maps, false);
	if (map == nullptr) return 0;
	return map->getName().size();
}

int MapController::longestWLD(const std::string& which_maps)
{
	int longest = 0;

	if (which_maps == "maptypes") {
		for (auto mapt : maptypes_) if (longest < mapt->WLDLength())
			longest = mapt->WLDLength();
		return longest;
	}

	if (which_maps == "all") {

		longest = total_->WLDLength();

		for (auto mapt : maptypes_) if (longest < mapt->WLDLength())
			longest = mapt->WLDLength();

		for (auto map : maps_) if (longest < map->WLDLength())
			longest = map->WLDLength();

		return longest;
	}

	if (which_maps == "total") {
		return total_->WLDLength();
	}

	// Kokeillaan maptype
	Map* mapt = findMap(which_maps, true);
	if (mapt != nullptr) {
		for (Map* map : mapt->getChildren()) 
			if (longest < map->WLDLength()) longest = map->WLDLength();
		return longest;
	}

	// Kokeillaan map
	Map* map = findMap(which_maps, false);
	if (map == nullptr) return 0;
	return map->WLDLength();
}

fileData MapController::generateFileData()
{
	fileData data;
	data.meta_ = meta_;
	data.total_ = total_;
	data.maps_ = maps_;
	data.maptypes_ = maptypes_;
	data.pref_ = pref_;

	// Jos historiaan on jäänyt liikaa tavaraa:
	while (history_.size() > numberToKeepInHistory()) {
		history_.pop_back();
	}

	data.history_ = history_;

	return data;
}

void MapController::addUnsavedTag()
{
	unsavedChanges_ = true;
}

void MapController::removeUnsavedTag()
{
	unsavedChanges_ = false;
}

void MapController::addToHistory(const std::string& command)
{
	int to_save = numberToKeepInHistory();

	while (history_.size() + 1 > to_save) {
		history_.pop_back();
	}

	if (to_save <= 0) {
		return;
	}

	history_.insert(history_.begin(),
		command + "?" + Utils::getTimeString());
}

void MapController::checkAndAutoSave()
{
	if (stateSaveOnPrefEdit()) {
		save({}, std::cout);
		removeUnsavedTag();
		return;
	}
	addUnsavedTag();
}

void MapController::updatePref()
{
	bool found;
	for (auto prefdefault : DEFAULT_PREFERENCES) {
		found = false;
		for (auto prefsaved : pref_) {
			if (prefdefault.first == prefsaved.first) {
				found = true;
				break;
			}
		}
		if (not found) {
			pref_.push_back(prefdefault);
		}
	}
}

bool MapController::stateSaveOnEdit() const
{
	for (auto pref : pref_)
		if (pref.first == "autoSaveOnEdit") {
			if (pref.second == "true") return true;
			return false;
		}
	return false;
}

bool MapController::stateSaveOnPrefEdit() const
{
	for (auto pref : pref_)
		if (pref.first == "autoSaveOnChangingPreferences") {
			if (pref.second == "true") return true;
			return false;
		}
	return false;
}

int MapController::numberToKeepInHistory() const
{
	for (auto pref : pref_)
		if (pref.first == "numberOfCommandsSavedInHistory") {
			return std::stoi(pref.second);
		}
	return 0;
}

int MapController::paddingAfterMapName() const
{
	for (auto pref : pref_)
		if (pref.first == "paddingAfterMapName") {
			return std::stoi(pref.second);
		}
	return 0;
}

int MapController::paddingAfterWLD() const
{
	for (auto pref : pref_)
		if (pref.first == "paddingAfterWLD") {
			return std::stoi(pref.second);
		}
	return 0;
}

int MapController::paddingInHistoryAfterNumber() const
{
	for (auto pref : pref_)
		if (pref.first == "paddingInHistoryAfterNumber") {
			return std::stoi(pref.second);
		}
	return 0;
}

int MapController::paddingInHistoryAfterCommand() const
{
	for (auto pref : pref_)
		if (pref.first == "paddingInHistoryAfterCommand") {
			return std::stoi(pref.second);
		}
	return 0;
}
