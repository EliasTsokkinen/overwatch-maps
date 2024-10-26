#include "File.h"

void File::createMainFile(std::ostream& output,
	const std::string& main_file_name,
	bool overwrite)
{
	if (!overwrite && Utils::fileExists(main_file_name)) {
		output << "ERROR! Trying to rewrite main data file even "
			<< "though it already exists!" << std::endl;
		return;
	}

	std::ofstream newFile(main_file_name);

	// Metadataa
	newFile << "#crt%" << Utils::getTime() << std::endl;
	newFile << "#upd%" << Utils::getTime() << std::endl;
	newFile << "#mup%" << Utils::getTime() << std::endl;
	newFile << "#mpv%" << Utils::getMapver() << std::endl;
	newFile << "#mpt%" << Utils::getMaps().size() << std::endl;
	{
		std::string::size_type number_of_maps = 0;
		for (auto map_type : Utils::getMaps()) {
			number_of_maps += map_type.second.size();
		}
		newFile << "#map%" << number_of_maps << std::endl;
	}

	newFile << "#ovr%meta" << std::endl;
	// Metadata loppuu

	// Yhteensä
	newFile << "Total;0;0;0" << std::endl;

	// Maptyypit
	for (auto map_type : Utils::getMaps()) {
		newFile << map_type.first << ";0;0;0" << std::endl;;
	}

	// Mapit
	for (auto map_type : Utils::getMaps()) {
		for (auto map : map_type.second) {
			newFile << map << ";" << map_type.first << ";0;0;0" << std::endl;
		}
	}
}

fileData File::readFile(const std::string& file_name)
{
	fileData data;

	std::ifstream file(file_name);

	if (!file) {
		data.meta_.push_back({ "#red", "fail" });
		return data;
	}

	std::string row;
	std::vector<std::string> sr;
	while (std::getline(file, row) && row != "#ovr%meta") {
		sr = Utils::split(row, '%');
		// Jos löytyy virheellinen rivi
		if (row.size() == 0 or sr.size() != 2) {
			data.meta_.push_back({ "#red", "fail" });
			return data;
		}
		
		// Jos kyseessä muu meta kuin asetus
		if (sr.at(0) != "#prf") {
			data.meta_.push_back({ sr.at(0), sr.at(1)});
			continue;
		}
		// Eli kyseessä on asetus
		std::vector<std::string> sp = Utils::split(sr.at(1), ':');

		// Virheellinen
		if (sp.size() != 2) {
			data.meta_.push_back({ "#red", "fail" });
			return data;
		}
		data.pref_.push_back({ sp.at(0),sp.at(1) });
	}

	int maptypes = Utils::getNumberOfMapTypes(data.meta_);
	if (maptypes <= 0) {
		data.meta_.push_back({ "#red", "fail" });
		return data;
	}
	int added = -1;
	// iteroidaan a:n läpi, tallennetaan total ja maptyypit erilleen
	// -1 koska maptypes ei sisällä totalia

	while (std::getline(file, row)) {
		// Virheellinen rivi
		if (row.size() == 0) {
			data.meta_.push_back({ "#red", "fail" });
			return data;
		}

		sr = Utils::split(row);

		if (added < maptypes) {
			// Virheellinen rivi
			if (sr.size() != 4) {
				data.meta_.push_back({ "#red", "fail" });
				return data;
			}
			// total hoidettu erikseen
			if (added == -1) {
				std::shared_ptr<Map> total = std::make_shared<Map>("Total");

				total->setWLD({ stoi(sr.at(1)),stoi(sr.at(2)),stoi(sr.at(3)) });

				data.total_ = total;
			}
			else {
				// Maptypes
				std::shared_ptr<Map> maptype =
					std::make_shared<Map>(sr.at(0), &(*data.total_));

				maptype->setWLD({ stoi(sr.at(1)),
					stoi(sr.at(2)),stoi(sr.at(3)) });

				data.maptypes_.push_back(maptype);
				// Lisätään totaliin
				data.total_->addChild(&(*maptype));
			}
		}
		else {
			// Virheellinen rivi
			if (sr.size() != 5) {
				data.meta_.push_back({ "#red", "fail" });
				return data;
			}
			// Kartat

			// Etsitään maptype
			Map* maptype = Utils::getMapType(sr.at(1), data.maptypes_);
			// Ei löydy
			if (maptype == nullptr) {
				data.meta_.push_back({ "#red", "fail" });
				return data;
			}

			std::shared_ptr<Map> map =
				std::make_shared<Map>(sr.at(0), maptype);
			map->setWLD({ stoi(sr.at(2)),stoi(sr.at(3)),stoi(sr.at(4)) });
			data.maps_.push_back(map);
			// Lisätään maptyypin alle
			maptype->addChild(&(*map));
		}
		++added;
	}
	return data;

}

bool File::save(const std::string& file_name, const fileData& data)
{
	std::ofstream file(file_name);
	if (not file) {
		return false;
	}

	// Tallennetaan metatiedot
	for (auto meta : data.meta_) {
		// muutetaan joitain tietoja
		if (meta.first == "#mup") {
			file << "#mup%" << Utils::getTime() << std::endl;
			continue;
		}

		file << meta.first << "%" << meta.second << std::endl;
	}

	// Tallenetaan asetukset:
	for (auto pref : data.pref_) {
		file << "#prf%" << pref.first << ":" << pref.second << std::endl;
	}

	// Tieto metan loppumisesta
	file << "#ovr%meta" << std::endl;

	// Total
	file << "Total;" << data.total_->getWins() << ";"
		<< data.total_->getLosses() << ";"
		<< data.total_->getDraws() << std::endl;

	// Maptyypit
	for (auto mapt : data.maptypes_) {
		file << mapt->getName() << ";" << mapt->getWins() << ";"
			<< mapt->getLosses() << ";" << mapt->getDraws() << std::endl;
	}

	// Kartat
	for (auto map : data.maps_) {
		file << map->getName() << ";"
			<< map->getMapt()->getName() << ";"
			<< map->getWins() << ";" << map->getLosses() << ";"
			<< map->getDraws() << std::endl;
	}
	return true;
}

bool File::exportToCsv(const std::string& file_name, const fileData& data)
{
	if (Utils::fileExists(file_name + ".csv")) {
		return false;
	}

	std::ofstream file(file_name + ".csv");
	if (not file) {
		return false;
	}

	// Total
	file << "Total;" << data.total_->getWins() << ";"
		<< data.total_->getLosses() << ";"
		<< data.total_->getDraws() << ";" <<
		data.total_->getWinrate() << std::endl;


	// Maptyypit
	for (auto mapt : data.maptypes_) {
		file << mapt->getName() << ";" << mapt->getWins() << ";"
			<< mapt->getLosses() << ";" << mapt->getDraws()
			<< ";" << mapt->getWinrate() << std::endl;
	}

	// Kartat
	for (auto map : data.maps_) {
		file << map->getName() << ";"
			<< map->getWins() << ";" << map->getLosses() << ";"
			<< map->getDraws() << ";" << map->getWinrate() << std::endl;
	}
	return true;
}
