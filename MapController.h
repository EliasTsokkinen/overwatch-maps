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
	{"autoSaveOnChangingPreferences", "true"}
};

const std::map<std::string, std::vector<std::string>> PREFERENCE_OPTIONS = {
	{"autoSaveOnEdit", {"true", "false"}},
	{"autoSaveOnChangingPreferences", {"true", "false"}}
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

	// Tallentaa
	void save(const std::vector<std::string>& params,
		std::ostream& output);

	// Vie .csv tiedostona
	void exportToCsv(const std::vector<std::string>& params,
		std::ostream& output);

	// Tarkistaa, onko tallentamattomia tietoja
	bool hasUnsavedChanges() const;

	std::vector<std::pair<std::string, std::string>> getPreferences() const;

	void updatePreferences(
		std::vector<std::pair<std::string, std::string>> newprefs);

private:
	Map* findMap(const std::string& mapname) const;

	Map* findMapType(const std::string& maptypename) const;

	void printMapNotFound(const std::string& mapname,
		std::ostream& output) const;

	void printMapTypeNotFound(const std::string& mapname,
		std::ostream& output) const;

	std::set<std::string> outputMaps(const std::string::size_type& maplen = 0)
		const;
	std::set<std::string> outputMapTypes(const std::string::size_type& maplen = 0)
		const;
	std::set<std::string> outputTotal(const std::string::size_type& maplen = 0)
		const;

	std::set<std::string> outputMap(const Map* map,
		const std::string::size_type& maplen = 0) const;

	std::set<std::string> outputChildren(const Map* map,
		const std::string::size_type& maplen = 0) const;

	fileData generateFileData();

	// Merkkaa tallentamattomaksi
	void addUnsavedTag();

	// Merkkaa tallennetuksi
	void removeUnsavedTag();

	void updatePref();

	std::string file_name_;
	std::shared_ptr<Map> total_ = nullptr;
	std::vector<std::shared_ptr<Map>> maptypes_ = { nullptr };
	std::vector<std::shared_ptr<Map>> maps_ = { nullptr };
	std::vector<std::pair<std::string, std::string>> meta_;
	std::vector<std::pair<std::string, std::string>> pref_;

	bool unsavedChanges_ = false;

	bool stateSaveOnEdit();

	bool stateSaveOnPrefEdit();
};