#pragma once
#include "Utils.h"
#include "MapController.h"
#include <iostream>
#include <map>
#include <vector>

const std::string PROMPT = "> ";

enum COMMAND_TYPES {
	EXIT, PARAMS, NOPARAMS, OPTPARAMS, HELP, PREFERENCES, PREVCOMM
};
// OPTPARAMS EI KÄYTÖSSÄ


using Func = void (MapController::*)(const std::vector<std::string>& params,
	std::ostream& output);

struct CInfo
{
	COMMAND_TYPES ct_;
	std::vector<std::string> aliases;
	std::vector<std::string> params;
	Func funcPointer_;
};

class Cli
{
public:
	Cli();

	bool exec(MapController& mapController);

private:
	std::vector<CInfo> comms_ = {
		{EXIT, {"quit", "q", "e", "exit"}, {}, nullptr},
		{PARAMS, {"addwin", "win", "addw", "aw", "w"},
			{"Mapname"}, &MapController::addWin},
		{PARAMS, {"adddraw", "draw", "addd", "ad", "d"},
			{"Mapname"}, &MapController::addDraw},
		{PARAMS, {"addloss", "loss", "addl", "al", "l"},
			{"Mapname"}, &MapController::addLoss},
		{PARAMS, {"delwin", "delw", "dwin", "dw",
				  "deletewin", "deletew"}, {"Mapname"},
				  &MapController::delWin},
		{PARAMS, {"delloss", "dell", "dloss", "dl",
				  "deleteloss", "deletel"}, {"Mapname"},
				  &MapController::delLoss},
		{PARAMS, {"deldraw", "deld", "ddraw", "dd",
				  "deletedraw", "deleted"}, {"Mapname"},
				  &MapController::delDraw},
		{NOPARAMS, {"printall", "printa", "pa", "pall",
			"print", "p"}, {}, &MapController::printAll},
		{NOPARAMS, {"printtotal", "printt", "pt", "ptotal",
			"ptot"}, {}, &MapController::printTotal},
		{PARAMS, {"printmap", "printm", "pm", "pmap"},
			{"Mapname"}, &MapController::printMap},
		{PARAMS, {"printmaptype", "printmampt", "printmt",
			"pmt","pmaptype", "pmapt"},
			{"Maptypename"}, &MapController::printMapType},
		{NOPARAMS, {"printmaptypes", "printmampts", "printmts",
			"pmts","pmaptypes", "pmapts"},
			{}, &MapController::printMapTypes},
		{NOPARAMS, {"history", "hist", "hst", "hs", "his"},
			{}, &MapController::history},
		{HELP, {"help", "h", "-h"}, {},nullptr},
		{NOPARAMS, {"save", "s"}, {},&MapController::manualSave},
		{PARAMS, {"export", "exp", "csv", "ex"}, {"file name"},
			&MapController::exportToCsv},
		{PREFERENCES, {"preferences", "preference", "pref", "settings",
			"options", "o"}, {}, nullptr},
		{PREVCOMM, {"^", "previouscommand", "prevcommand", "pcommand",
			"previouscomm", "prevcomm", "pcomm", "previousc", "prevc", "pc"},
			{"optional n"}, nullptr}
	};

	const std::string HELP_ = "AVAILABLE COMMANDS:\n"
		" - help\n    Brings up this help.\n"
		" - quit\n    Quits the program. (DOES NOT SAVE!)\n"
		" - addwin \"map name\"\n    Adds a win to the given map as well as"
		" it's map type and the total.\n"
		" - addloss \"map name\"\n    Adds a loss to the given map as well"
		" as it's map type and the total.\n"
		" - adddraw \"map name\"\n    Adds a draw to the given map as well"
		" as it's map type and the total.\n"
		" - deletewin \"map name\"\n    Deletes a win from the given map as"
		" well as it's map type and the total.\n"
		" - deleteloss \"map name\"\n    Deletes a loss from the given map as"
		" well as it's map type and the total.\n"
		" - deletedraw \"map name\"\n    Deletes a draw from the given map as"
		" well as it's map type and the total.\n"
		" - printall\n    Prints all stats.\n"
		" - printtotal\n    Prints stats for total.\n"
		" - printmap \"map name\"\n    Prints stats for single map.\n"
		" - printmaptype \"map type name\"\n    Prints stats for single "
		"maptype and it's maps.\n"
		" - printmaptypes\n    Prints stats for all maptypes.\n"
		" - save\n    Saves changes.\n"
		" - export [file name]\n    Exports stats to a CSV, which can be "
		"opened in for example Microsoft Excel or LibeOffice Calc. DOES NOT "
		"SAVE! Export the current \"working\" copy of data i.e. what is saved "
		"and what you have entered.\n"
		" - preferences\n    Change preferences/options.\n\n"
		"To pass parameteres composing of multiple words, wrap them in \"\"."
		"\n  e.g. win \"Blizzard World\".\n"
		"\nAll commands have multiple aliases, e.g. \"help\" and \"h\" both "
		"bring up this message.\n"
		"\nMade with C++ and STL.\n"
		"If you have any questions, problems or suggestions, feel free to "
		"message me on Twitter/X @realeliju or Discord: eliju";

	bool command(MapController& mapController, 
		std::vector<std::string> input);

	CInfo* findComm(std::string& commName);

	bool exitFunction(MapController& mapController);

	void preferences(MapController& mapController);

	void nextOptionInPreference(
		std::vector<std::pair<std::string, std::string>>& pref,
		const int& pos);

	bool previousCommand(MapController& mapController,
		const std::vector<std::string>& input);
};