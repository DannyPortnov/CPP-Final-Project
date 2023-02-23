#include "Utilities.h"

//todo: put regex and use refs
void Utilities::Replace_All(vector<string*>& params)
{
	//if (params.front()->find('_') == std::string::npos) { //else, replace ' ' with '_'
	//	charToRemove = ' ';
	//	charToReplaceWith = '_';
	//}
	for (auto param : params) {
		char charToRemove = '_', charToReplaceWith = ' '; //if there's any '_', replace them with ' '
		if (param->find('_') == std::string::npos) { //else, replace ' ' with '_'
			charToRemove = ' ';
			charToReplaceWith = '_';
		}
		std::replace(param->begin(), param->end(), charToRemove, charToReplaceWith);
	}
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

bool Utilities::Is_File_Valid(ios& fileToCheck)
{
	if (!fileToCheck) {
		cout << "Couldn't open file for serialization or deserialization" << endl;
		return false;
	}
	return true;
}
