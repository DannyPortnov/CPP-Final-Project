#define   _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Library.h"
#include "Server.h"
using namespace std;

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

enum string_code {
	eDailyMix,
	eSearch,
	ePodcast,
	eLibrary,
	ePlaylists,
	eHelp,
	eBack
};

string_code hashit(std::string const& inString) {
	if (inString == "Daily Mix") return eDailyMix;
	if (inString == "Search") return eSearch;
	if (inString == "Podcast") return ePodcast;
	if (inString == "Library") return eLibrary;
	if (inString == "Playlists") return ePlaylists;
	if (inString == "Help") return eHelp;
	if (inString == "Back") return eBack;
}

//void foo() {
//	switch (hashit(stringValue)) {
//	case eFred:
//		...
//	case eBarney:
//		...
//	}
//}

void Main_Menu() {
	cout << "Main Menu: " << endl;
	cout << " Daily Mix" << endl;
	cout << " Search" << endl;
	cout << " Podcast" << endl;
	cout << " Library" << endl;
	cout << " Playlists" << endl;
	cout << " Help" << endl;
	cout << " Back" << endl;
}

void mp3player() {
	string answer;
	bool run_program = true;
	Library lib;
	while (run_program) {
		cout << "Welcome To mp3player" << endl;
		cout << endl;

//		cout << "To continue press (1), To print board press (2), To end game press (0):" << endl;
		cin >> answer;
		switch (hashit(answer))
		{
		case(eDailyMix): {

			break;
		}
		case(eSearch): {

			break;
		}
		case(ePodcast): {

			break;
		}
		case(eLibrary): {

			break;
		}
		case(ePlaylists): {

			break;
		}
		case(eHelp): {
			Main_Menu();
			break;
		}
		case(eBack): {

			break;
		}
		default: {
			cout << "Wrong Key Was PRESSED!!! Please Try Another Time" << endl;
		}
		}

		cout << endl;
		cout << endl;
	}
	//lib.Begin_Serialization();

	//lib.Begin_Deserialization();

	//int id;
	
	//....

}

int main()
{
	mp3player();
	//Server::Destroy_All_Allocations();
	cout << "Memory Leaks: " << _CrtDumpMemoryLeaks() << endl;
	return 0;
}
