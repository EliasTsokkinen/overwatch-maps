#pragma once
#include "Utils.h"
#include "Map.h"
#include "File.h"
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <set>

const std::string::size_type NUMBER_OF_PREF = 1;

const std::vector<std::pair<std::string, std::string>> DEFAULT_PREFERENCES = {
	{"autoSaveOnEdit", "true"},
	{"autoSaveOnChangingPreferences", "true"},
	{"numberOfCommandsSavedInHistory", "20"},
	{"paddingAfterMapName", "3"},
	{"paddingAfterWLD", "3"},
	{"paddingInHistoryAfterNumber", "1"},
	{"paddingInHistoryAfterCommand", "3"}
};

const std::map<std::string, std::vector<std::string>> PREFERENCE_OPTIONS = {
	{"autoSaveOnEdit", {"true", "false"}},
	{"autoSaveOnChangingPreferences", {"true", "false"}},
	{"numberOfCommandsSavedInHistory", {"%posint"}},
	{"paddingAfterMapName", {"%posint"}},
	{"paddingAfterWLD", {"%posint"}},
	{"paddingInHistoryAfterNumber", {"%posint"}},
	{"paddingInHistoryAfterCommand", {"%posint"}}
};

class MapController
{
public:
	MapController(std::string file_name);

	// Read file
	void readFileMC();

	// Palauttaa onnistuiko tiedoston lukeminen (false = löytyy meta #red%fail)
	bool didFileRead();

	// Lisää voiton tietylle kartalle ja sen "vanhemmille"
	void addWin(const std::vector<std::string>& params,
		std::ostream& output);

	// Lisää häviön tietylle kartalle ja sen "vanhemmille"
	void addLoss(const std::vector<std::string>& params,
		std::ostream& output);

	// Lisää tasapelin tietylle kartalle ja sen "vanhemmille"
	void addDraw(const std::vector<std::string>& params,
		std::ostream& output);

	// Poistaa voiton kartalta ja sen vanhemmilta
	void delWin(const std::vector<std::string>& params,
		std::ostream& output);

	// Poistaa häviön kartalta ja sen vanhemmilta
	void delLoss(const std::vector<std::string>& params,
		std::ostream& output);

	// Poistaa tasapalein kartalta ja sen vanhemmilta
	void delDraw(const std::vector<std::string>& params,
		std::ostream& output);

	// Tulostaa kaiken
	void printAll(const std::vector<std::string>& params,
		std::ostream& output);

	// Tulostaa total
	void printTotal(const std::vector<std::string>& params,
		std::ostream& output);

	// Tulostaa tietyn kartan
	void printMap(const std::vector<std::string>& params,
		std::ostream& output);

	// Tulostaa tietyn karttatyypin ja sen lapset
	void printMapType(const std::vector<std::string>& params,
		std::ostream& output);

	// Tulostaa kaikki karttatyypit
	void printMapTypes(const std::vector<std::string>& params,
		std::ostream& output);

	// Tallentaa, manuaalisesti käyttäjän haluama (tulostaa ilmoituksen)
	void manualSave(const std::vector<std::string>& params,
		std::ostream& output);

	// Tallentaa
	void save(const std::vector<std::string>& params,
		std::ostream& output, const bool autosave=true);

	// Vie .csv tiedostona
	void exportToCsv(const std::vector<std::string>& params,
		std::ostream& output);

	// Tulostaa muokkaushistorian
	void history(const std::vector<std::string>& params,
		std::ostream& output);

	// Palauttaa historiaan tallennetun n.:n komennon
	std::string previousCommand(const int& n) const;

	// Tarkistaa, onko tallentamattomia tietoja
	bool hasUnsavedChanges() const;

	std::vector<std::pair<std::string, std::string>> getPreferences() const;

	void updatePreferences(
		std::vector<std::pair<std::string, std::string>> newprefs);

private:
	std::string file_name_;
	std::shared_ptr<Map> total_ = nullptr;
	std::vector<std::shared_ptr<Map>> maptypes_ = { nullptr };
	std::vector<std::shared_ptr<Map>> maps_ = { nullptr };
	std::vector<std::pair<std::string, std::string>> meta_;
	std::vector<std::pair<std::string, std::string>> pref_;
	std::vector<std::string> history_;

	bool unsavedChanges_ = false;

	Map* findMap(const std::string& mapname, const bool maptype=false) const;

	void printMapNotFound(const std::string& mapname,
		std::ostream& output) const;

	void printMapTypeNotFound(const std::string& mapname,
		std::ostream& output) const;

	std::set<std::string> outputMaps(const std::string::size_type& maplen = 0,
		const std::string::size_type& wldlen = 5)
		const;
	std::set<std::string> outputMapTypes(const std::string::size_type& maplen = 0,
		const std::string::size_type& wldlen = 5)
		const;
	std::set<std::string> outputTotal(const std::string::size_type& maplen = 0,
		const std::string::size_type& wldlen = 5)
		const;

	std::set<std::string> outputMap(const Map* map,
		const std::string::size_type& maplen = 0,
		const std::string::size_type& wldlen = 5) const;

	std::set<std::string> outputChildren(const Map* map,
		const std::string::size_type& maplen = 0,
		const std::string::size_type& wldlen = 5) const;

	int longestMapName(const std::string& which_maps = "all");

	int longestWLD(const std::string& which_maps = "all");

	fileData generateFileData();

	// Merkkaa tallentamattomaksi
	void addUnsavedTag();

	// Merkkaa tallennetuksi
	void removeUnsavedTag();

	// Lisää historiaan uusimman komennon
	void addToHistory(const std::string& command);

	void checkAndAutoSave();

	void updatePref();

	bool stateSaveOnEdit() const;

	bool stateSaveOnPrefEdit() const;

	int numberToKeepInHistory() const;

	int paddingAfterMapName() const;

	int paddingAfterWLD() const;

	int paddingInHistoryAfterNumber() const;

	int paddingInHistoryAfterCommand() const;
};