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
	addToHistory("addWin \"" + params.at(0) + "\"");
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
	addToHistory("addLoss \"" + params.at(0) + "\"");
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
	addToHistory("addDraw \"" + params.at(0) + "\"");
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
	addToHistory("delWin \"" + params.at(0) + "\"");
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
	addToHistory("delLoss \"" + params.at(0) + "\"");
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
	addToHistory("delDraw \"" + params.at(0) + "\"");
	checkAndAutoSave();
}

void MapController::printAll(const std::vector<std::string>& params,
	std::ostream& output)
{
	std::vector<std::set<std::string>> alloutputs 
		= { outputTotal(), outputMapTypes(), outputMaps() };

	std::string::size_type longest =
		Utils::lengthOfLongestMapName(alloutputs);

	alloutputs =
	{ outputTotal(longest), outputMapTypes(longest), outputMaps(longest) };

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
	for (auto op : outputTotal()) {
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

	for (auto op : outputMap(target)) {
		output << op << std::endl;
	}
	addToHistory("printMap \"" + params.at(0) + "\"");
}

void MapController::printMapType(const std::vector<std::string>& params,
	std::ostream& output)
{
	Map* target = findMap(params.at(0), true);
	if (target == nullptr) {
		printMapTypeNotFound(params.at(0), output);
		return;
	}

	std::vector<std::set<std::string>> alloutputs
		= { outputMap(target), outputChildren(target) };

	std::string::size_type longest =
		Utils::lengthOfLongestMapName(alloutputs);

	alloutputs = { outputMap(target, longest),
		outputChildren(target, longest) };

	// itse maptype
	for (std::string line : alloutputs.at(0)) {
		output << line << std::endl;
	}

	output << "\n";

	// lapset
	for (std::string line : alloutputs.at(1)) {
		output << line << std::endl;
	}

	addToHistory("printMapType \"" + params.at(0) + "\"");
}

void MapController::printMapTypes(const std::vector<std::string>& params,
	std::ostream& output)
{
	std::string::size_type longest =
		Utils::lengthOfLongestMapName({ outputMapTypes() });

	for (std::string line : outputMapTypes(longest)) {
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
	for (std::string line : history_) {

		std::vector<std::string> split = Utils::split(line, '?');
		std::string action = split.at(0);
		std::string time = split.at(1);

		output << i << ". " << action << "   (" << time << ")" << std::endl;
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
	const std::string::size_type& maplen) const
{
	std::set<std::string> set;
	for (std::shared_ptr<Map> map : maps_) {
		set.insert(map->getOutput(maplen));
	}
	return set;
}

std::set<std::string> MapController::outputMapTypes(
	const std::string::size_type& maplen) const
{
	std::set<std::string> set;
	for (std::shared_ptr<Map> maptype : maptypes_) {
		set.insert(maptype->getOutput(maplen));
	}
	return set;
}

std::set<std::string> MapController::outputTotal(
	const std::string::size_type& maplen) const
{
	return { total_->getOutput(maplen) };
}

std::set<std::string> MapController::outputMap(const Map* map,
	const std::string::size_type& maplen) const
{
	return { map->getOutput(maplen) };
}

std::set<std::string> MapController::outputChildren(const Map* map, const std::string::size_type& maplen) const
{
	std::set<std::string> to_return;
	for (Map* child : map->getChildren()) {
		to_return.insert(child->getOutput(maplen));
	}
	return to_return;
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
