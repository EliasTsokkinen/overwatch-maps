#include "Utils.h"
#include "File.h"
#include "MapController.h"
#include "CommandlineInterface.h"
#include "Map.h"
#include <iostream>
#include <fstream>

const std::string INFORMATION =
"Overwatch map winrate calculator by Eliju\n"
"Twitter/X: @realeliju\n"
"Discord: eliju\n"
"Version B.2.3\n"
"Made for Overwatch version 2.13.0";

const std::string MAIN_FILE_NAME = ".data.owmd";


int main() {
	std::cout << INFORMATION << "\n\n\n\n" << std::endl;

	// Aloittaa ohjelman
	Utils::init(std::cout, MAIN_FILE_NAME);

	// Luo MapData-olion johon tiedoston tiedot tallennetaan
	MapController mapController(MAIN_FILE_NAME);

	// Jos tiedoston lukeminen epäonnistui:
	if (not mapController.didFileRead()) {
		std::cout << "Error reading file :(" << std::endl;
		system("PAUSE");
		return 1;
	}

	Cli cli;
	while (cli.exec(mapController));

	return 0;
}

