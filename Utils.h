#pragma once
#include "Map.h"
#include "File.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <chrono>
#include <memory>
#include <set>



namespace Utils {
	// Aloittaa ohjelman
	void init(std::ostream& output, const std::string& main_file_name);

	// Tarkistaa onko tiedostoa olemassa
	bool fileExists(const std::string& file_name);

	// Tarkistaa onko tiedosto samaa versiota
	bool mapverUpToDate(const std::string& file_name,
		const std::string& mapver);

	// Lukee maptyyppien m‰‰r‰n metasta
	int getNumberOfMapTypes(const std::vector<std::pair<std::string,
		std::string>>& meta);

	// Palauttaa osoittimen maptyyppiin listasta
	Map* getMapType(const std::string& maptype_name,
		const std::vector<std::shared_ptr<Map>>& maptypes);

	// Erottelee merkkijonot
	std::vector<std::string> split(const std::string& to_split,
		const char splitter = ';',
		const char compound = '"');

	// Palauttaa merkkijonon pienill‰ kirjaimilla
	std::string in_lower(const std::string& str);

	// Kartat
	std::map<std::string, std::vector<std::string>> getMaps();

	// Karttaversio
	std::string getMapver();

	// Palauttaa nykyisen ajan
	std::time_t getTime();

	// Palauttaa output tyylisist‰ stringeist‰ pisimm‰n mapnimen
	// ">Watchpoint: Gibraltar:< x/y/z W-%: ab"
	// palauttaa >< v‰lisen pituuden
	std::string::size_type lengthOfLongestMapName(
		const std::vector<std::set<std::string>>& input);

	/*
	Poistaa seuraavat merkit:
	' '
	':'
	'''
	'"'
	'/'
	'\'
	*/
	std::string removeSpecials(const std::string& input);

	// Yhdist‰‰ removeSpecials ja in_lower
	std::string format(const std::string& input);

	std::string lookForAlias(const std::string& input);
}