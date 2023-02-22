#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
using namespace std;

class Utilities
{
public:
	//Replaces all underscores with space and vice versa in each string in the vector.
	static void Replace_All(vector<string> params);
	//Checks if reached the end of the file to avoid empty lines
	static bool Is_End_Of_File(fstream& fileToRead);
	//If the file isn't valid, prints a message.
	static bool Is_File_Valid(fstream& fileToRead);
};
#endif
