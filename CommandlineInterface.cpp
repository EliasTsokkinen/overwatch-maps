#include "CommandlineInterface.h"


Cli::Cli()
{
}

bool Cli::exec(MapController& mapController)
{
	std::string row;
	std::cout << std::endl << std::endl;
	std::cout << PROMPT;
	std::getline(std::cin, row);

	std::vector<std::string> input = Utils::split(row, ' ');
	
	return command(mapController, input);
}

bool Cli::command(MapController& mapController, 
	std::vector<std::string> input)
{
	if (input.empty() or input.at(0) == "") {
		return true;
	}

	std::string commName = input.at(0);
	input.erase(input.begin());

	CInfo* comm = findComm(commName);

	if (comm == nullptr) {
		std::cout << "Unknown command \"" << commName << "\". "
			"Try \"help\"." << std::endl;
		return true;
	}

	if (comm->ct_ == COMMAND_TYPES(HELP)) {
		std::cout << HELP_ << std::endl;
		return true;
	}

	if (comm->ct_ == COMMAND_TYPES(PREVCOMM)) {
		std::cout << "Not implemented yet." << std::endl;
	}

	if (comm->params.size() != input.size()) {
		// Jos parametrit eivät ole vapaaehtoisia TAI vapaaehtoisella parametreja on
		// väärä määrä
		// OPTPARAMS ei enää käytössä
		if (not(comm->ct_ == COMMAND_TYPES(OPTPARAMS)) or
			not(input.size() == 0)) {
			std::cout << "Wrong number of parameters for command \""
				<< commName << "\". " << comm->params.size()
				<< " parameters expected, " << input.size() << " provided."
				" Try the command \"help\"."
				<< std::endl;
			return true;
		}
	}
	if (comm->ct_ == COMMAND_TYPES(EXIT)) {
		return !exitFunction(mapController);
	}

	if (comm->ct_ == COMMAND_TYPES(PREFERENCES)) {
		preferences(mapController);
		return true;
	}

	(mapController.*(comm->funcPointer_))(input, std::cout);
	return true;
}

CInfo* Cli::findComm(std::string& commName)
{
	// muutetaan pieniksi kirjaimiksi
	commName = Utils::in_lower(commName);

	for (auto i = comms_.begin(); i != comms_.end(); ++i) {
		for (auto& name : i->aliases) {
			if (name == commName) {
				return &(*i);
			}
		}
	}
	return nullptr;
}

bool Cli::exitFunction(MapController& mapController)
{
	if (mapController.hasUnsavedChanges()) {
		std::cout << "Warning! You have unsaved changes! To confirm exiting "
			"without saving, type \"quit\". To save and the quit, type "
			"\"save\". To cancel, press ENTER\n> ";

		std::string response = "";
		std::getline(std::cin, response);
		response = Utils::in_lower(response);

		if (response == "quit" or response == "q") {
			return true;
		}
		if (response == "save" or response == "s") {
			mapController.save({}, std::cout);
			return true;
		}

		std::cout << "Abandoning quit." << std::endl;
		return false;
	}
	return true;
}

void Cli::preferences(MapController& mapController)
{
	std::vector<std::pair<std::string, std::string>> prefs = 
		mapController.getPreferences();

	std::string response = "";
	int i = 1;
	bool change = false;

	while (!(response == "save" or response == "s"
		or response == "cancel" or response == "c")) {
		i = 1;
		std::cout << "\n\n";
		change = false;
		for (auto pref : prefs) {
			std::cout << "[" << i << "]";
			for (std::string::size_type j = 0; j < std::to_string(i).length(); ++j) {
				std::cout << " ";
			}
			std::cout << pref.first << ": " << pref.second << std::endl;
			++i;
		}
		std::cout << "\nType the number of a preference to cycle through "
			"it's options. Type \"save\" to save changes to preferences "
			"and \"cancel\" to discard any changes.\n> ";

		std::getline(std::cin, response);
		response = Utils::format(response);

		for (int a = 0; a < i; ++a) {
			if (response == std::to_string(a + 1)) {
				change = true;
				i = a;
				break;
			}
		}

		if (change) {
			nextOptionInPreference(prefs, i);
		}
	}

	if (response == "save" or response == "s") {
		mapController.updatePreferences(prefs);
		std::cout << "Preferences saved." << std::endl;
		return;
	}

	std::cout << "Changes to preferences abandoned." << std::endl;
}

void Cli::nextOptionInPreference(
	std::vector<std::pair<std::string, std::string>>& prefs,
	const int& a)
{
	int pos = 0;
	while (true) {
		if (PREFERENCE_OPTIONS.at(prefs.at(a).first).at(pos) 
			== prefs.at(a).second) {
			break;
		}
		++pos;
	}

	if (pos == PREFERENCE_OPTIONS.at(prefs.at(a).first).size() - 1) {
		pos = 0;
	}
	else ++pos;

	prefs.at(a).second = PREFERENCE_OPTIONS.at(prefs.at(a).first).at(pos);
}