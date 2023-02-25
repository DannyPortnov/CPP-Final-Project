#include "Utilities.h"




string_code Utilities::hashit(std::string const& inString) {
	if (inString == "Daily Mix") return eDailyMix;
	if (inString == "Search") return eSearch;
	if (inString == "Podcast") return ePodcast;
	if (inString == "Library") return eLibrary;
	if (inString == "Playlists") return ePlaylists;
	if (inString == "Help") return eHelp;
	if (inString == "Back") return eBack;
}

//todo: put regex and use refs
void Utilities::Replace_All(vector<string*>& params, bool is_serialization)
{
	// REGEX IMPLEMENTATION:
	string charToRemove = " ", charToReplaceWith = "_"; //if there's any '_', replace them with ' '
	if (is_serialization) {
		charToRemove = "_";
		charToReplaceWith = " ";
	}

	for (auto param : params) {
		regex pattern(charToRemove);
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

bool Utilities::Is_End_Of_File(ios_base& fileToCheck)
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
bool Utilities::Is_End_Of_File_Or_Empty(ifstream& fileToCheck)
{
	//first condition: file is empty, second: finished reading
	auto first = fileToCheck.peek(); //todo: temporary, to be removed after testing
	auto second = fileToCheck.eof();
	if (fileToCheck.peek() == std::ifstream::traits_type::eof() || fileToCheck.eof()) {
		return true;
	}
	return false;
}

bool Utilities::Is_File_Valid(ios& fileToCheck) //todo: add promt to create folder if doesn't exist, or check for a command that creates the folder
{
	if (!fileToCheck) { //todo: pass the file name also
		cout << "Couldn't open file for serialization or deserialization" << endl;
		return false;
	}
	return true;
}

// create general prompt function for dialog with user
bool Utilities::user_prompts_and_dialog(string& prompt_message, string& reject_message, string& accept_message) {
	while (true) {
		cout << prompt_message << " "; // answer is y/n
		string answer; //todo: mabye switch back to char
		cin >> answer; cout << endl;
		if (answer == "n") {
			cout << reject_message << endl;
			return false;
		}
		else if (answer == "y") {
			cout << accept_message << endl;
			return true;
		}
		cout << "Invalid answer! try again." << endl;
	}

}