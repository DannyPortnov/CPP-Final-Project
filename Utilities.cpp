#include "Utilities.h"


void Utilities::Replace_All(vector<string> params)
{
	char charToRemove = '_', charToReplaceWith=' '; //if there's any '_', replace them with ' '
	if (params.front().find('_') == std::string::npos) { //else, replace ' ' with '_'
		charToRemove = ' ';
		charToReplaceWith = '_';
	}
	for (auto& param : params) {
		std::replace(param.begin(), param.end(), charToRemove, charToReplaceWith);
	}
}

bool Utilities::Is_End_Of_File(fstream& fileToCheck)
{
	char endOfLine = fileToCheck.get(); //Skips the \n at the end of line
	char startOfNextLine = fileToCheck.peek(); //Peeks at the start of the next line
	if (startOfNextLine == '\n') //if the next line is also \n, quit
	{
		return true;
	}
	return false;
}

bool Utilities::Is_File_Valid(fstream& fileToCheck)
{
	if (!fileToCheck) {
		cout << "Couldn't open file for serialization or deserialization" << endl;
		return false;
	}
	return true;
}
