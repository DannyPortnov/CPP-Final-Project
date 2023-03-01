#include "Utilities.h"



//Converts a string to its equivalent enum (workaround to use switch case on strings)
string_code Utilities::hashit(std::string const& inString) {
	if (inString == "Daily Mix") return eDailyMix;
	if (inString == "Search") return eSearch;
	if (inString == "Search by genre") return eSearchByGenre;
	if (inString == "Search by album") return eSearchByAlbum;
	if (inString == "Search by name") return eSearchByName;
	if (inString == "Search by singer") return eSearchBySinger;
	if (inString == "Podcast") return ePodcast;
	if (inString == "Library") return eLibrary;
	if (inString == "Playlists") return ePlaylists;
	if (inString == "Help") return eHelp;
	if (inString == "Back") return eBack;
	if (inString == "Exit") return eExit;
	if (inString == "More") return eMore;
	if (inString == "AddEpisode") return eAddEpisode;
	if (inString == "UpdateEpisode") return eUpdateEpisode;
	if (inString == "DeleteEpisode") return eDeleteEpisode;
	if (inString == "List") return eList;
	if (inString == "Add") return eAdd;
	if (inString == "Update") return eUpdate;
	if (inString == "Delete") return eDelete;
	if (inString == "Print") return ePrint;
	if (inString == "PrintSong") return ePrintSong;
	if (inString == "Add2PL") return eAdd2PL;
	if (inString == "RemoveFromPL") return eRemoveFromPL;
	if (inString == "PrintPL") return ePrintPL;
	if (inString == "PlayAll") return ePlayAll;
	if (inString == "Play") return ePlay;
	if (inString == "PlayRandom") return ePlayRandom;
	if (inString == "Random") return eRandom;
	return eError;
}

//Returns the full adress of the file specified
void Utilities::Format_Address(std::string& file_name)
{
	std::string full_address = address_template;
	file_name = std::regex_replace(full_address, std::regex(name_to_replace), file_name); //replace name_to_replace with file_name
}

void Utilities::Replace_All(std::vector<std::string*>& params, bool is_serialization)
{
	std::string charToRemove = " ", charToReplaceWith = "_"; //if there's any '_', replace them with ' '
	if (is_serialization) { //or other way around
		charToRemove = "_";
		charToReplaceWith = " ";
	}

	for (auto param : params) {
		std::regex pattern(charToRemove);
		*param = regex_replace(*param, pattern, charToReplaceWith);
	}

}

//Assumes folder exists
bool Utilities::Is_End_Of_File(std::ios_base& fileToCheck)
{
	std::istream* input = dynamic_cast<std::istream*>(&fileToCheck);
	if (!input) {
		std::cerr << "Could not cast to input stream." << std::endl;
		return false;
	}

	char endOfLine = input->get(); //Skips the \n at the end of line
	char startOfNextLine = input->peek(); //Peeks at the start of the next line
	if (startOfNextLine == '\n') //if the next line is also \n, quit
	{
		return true;
	}
	return false;
}
//Checks if file is empty of if reached the end of it
bool Utilities::Is_End_Of_File_Or_Empty(std::ifstream& fileToCheck)
{
	if (fileToCheck.peek() == std::ifstream::traits_type::eof() || fileToCheck.eof()) {
		return true;
	}
	return false;
}

bool Utilities::Is_File_Valid(std::ios& fileToCheck, const std::string& file_name) 
{
	if (!fileToCheck) { 
		std::cout << "Couldn't open file " << file_name << " for serialization or deserialization" << std::endl;
		return false;
	}
	return true;
}

// create general prompt function for dialog with user
bool Utilities::user_prompts_and_dialog(std::string& prompt_message, std::string& reject_message, std::string& accept_message) {
	while (true) {
		std::cout << prompt_message << " "; // answer is y/n
		std::string answer;
		std::cin >> answer; std::cout << std::endl;
		if (answer == "n") {
			std::cout << reject_message << std::endl;
			return false;
		}
		if (answer == "y") {
			std::cout << accept_message << std::endl;
			return true;
		}
		std::cout << "Invalid answer! try again." << std::endl;
	}

}