#include "Utilities.h"




string_code Utilities::hashit(std::string const& inString) {
	if (inString == "Daily Mix") return eDailyMix;
	if (inString == "Search") return eSearch;
	if (inString == "Podcast") return ePodcast;
	if (inString == "Library") return eLibrary;
	if (inString == "Playlists") return ePlaylists;
	if (inString == "Help") return eHelp;
	if (inString == "Back") return eBack;
	if (inString == "More") return eMore;
	if (inString == "List") return eList;
	if (inString == "Add") return eAdd;
	if (inString == "Update") return eUpdate;
	//if (inString == "Update song by using name") return eUpdateSongByName;
	//if (inString == "Update song by using id") return eUpdateSongById;
	//if (inString == "Delete song by using name") return eDeleteSongByName;
	//if (inString == "Delete song by using id") return eDeleteSongById;
	if (inString == "Delete") return eDelete;
	if (inString == "PrintSong") return ePrintSong;
	//if (inString == "Print song by using name") return ePrintSongByName;
	//if (inString == "Print song by using id") return ePrintSongById;
	if (inString == "PrintSong") return ePrintSong;
	if (inString == "Add2PL") return eAdd2PL;
	if (inString == "RemoveFromPL") return eRemoveFromPL;
	if (inString == "PrintPL") return ePrintPL;
	//if (inString == "Play song by using name") return ePlaySongByName;
	//if (inString == "Play song by using id") return ePlaySongById;
	if (inString == "Play") return ePlay;
	if (inString == "PlayAll") return ePlayAll;
	if (inString == "Play") return ePlay;
	if (inString == "PlayRandom") return ePlayRandom;
	if (inString == "Help") return eHelp;
	if (inString == "Back") return eBack;
	return eError;
}

//todo: put regex and use refs
void Utilities::Replace_All(std::vector<std::string*>& params, bool is_serialization)
{
	// REGEX IMPLEMENTATION:
	std::string charToRemove = " ", charToReplaceWith = "_"; //if there's any '_', replace them with ' '
	if (is_serialization) {
		charToRemove = "_";
		charToReplaceWith = " ";
	}

	for (auto param : params) {
		std::regex pattern(charToRemove);
		*param = regex_replace(*param, pattern, charToReplaceWith);
	}

	#pragma region previous implementation
	//for (auto param : params) {
	//	char charToRemove = '_', charToReplaceWith = ' '; //if there's any '_', replace them with ' '
	//	if (param->find('_') == std::string::npos) { //else, replace ' ' with '_'
	//		charToRemove = ' ';
	//		charToReplaceWith = '_';
	//	}
	//	std::replace(param->begin(), param->end(), charToRemove, charToReplaceWith);
	//}  
	#pragma endregion

}

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
bool Utilities::Is_End_Of_File_Or_Empty(std::ifstream& fileToCheck)
{
	//first condition: file is empty, second: finished reading
	auto first = fileToCheck.peek(); //todo: temporary, to be removed after testing
	auto second = fileToCheck.eof();
	if (fileToCheck.peek() == std::ifstream::traits_type::eof() || fileToCheck.eof()) {
		return true;
	}
	return false;
}

bool Utilities::Is_File_Valid(std::ios& fileToCheck) //todo: add promt to create folder if doesn't exist, or check for a command that creates the folder
{
	if (!fileToCheck) { //todo: pass the file name also
		std::cout << "Couldn't open file for serialization or deserialization" << std::endl;
		return false;
	}
	return true;
}

// create general prompt function for dialog with user
bool Utilities::user_prompts_and_dialog(std::string& prompt_message, std::string& reject_message, std::string& accept_message) {
	while (true) {
		std::cout << prompt_message << " "; // answer is y/n
		std::string answer; //todo: mabye switch back to char
		std::cin >> answer; std::cout << std::endl;
		if (answer == "n") {
			std::cout << reject_message << std::endl;
			return false;
		}
		else if (answer == "y") {
			std::cout << accept_message << std::endl;
			return true;
		}
		std::cout << "Invalid answer! try again." << std::endl;
	}

}