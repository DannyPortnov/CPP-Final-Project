#define   _CRTDBG_MAP_ALLOC
#include "MethodMap.h"
#include "Library.h"
//#include "Utilities.h"
//#include "Server.h"
#include <crtdbg.h>
#include <iostream>
#include <fstream>
#include <string>
//using namespace std;

//it is generally not recommended to add 
//using namespace std in a header file or a global scope 
//as it can cause naming conflicts and ambiguity issue
//s with other libraries or future code changes.
//It's a best practice to explicitly prefix
//standard library names with std::.

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG




//void foo() {
//	switch (hashit(std::stringValue)) {
//	case eFred:
//		...
//	case eBarney:
//		...
//	}
//}

void Main_Menu() {
	std::cout << "Main Menu: " << std::endl;
	std::cout << " Daily Mix" << std::endl;
	std::cout << " Search" << std::endl;
	std::cout << " Podcast" << std::endl;
	std::cout << " Library" << std::endl;
	std::cout << " Playlists" << std::endl;
	std::cout << " Help" << std::endl;
	std::cout << " Back\n" << std::endl;
}

void mp3player() {
	std::string answer;
	bool run_program = true;
	Library lib;
	MethodMap methodmap;
	std::string key = "hello";
	// ** How to use MethodMap for methods with any parameters** //
	//methodmap.Insert<std::string>("second", &Library::Example_Func_For_MethodMap); //Add method to map with its key
	////methodmap.Insert<void>("first", &Library::Example_Func_For_MethodMap); //Add method to map with its key
	//methodmap.Insert("third", &Library::Example2_Func_For_MethodMap);
	//methodmap.Call<void>("first", &lib); // Call method that belongs to its key
	//methodmap.Call<void>("second", &lib, std::string("wow")); // Call method that belongs to its key
	//methodmap.Call<void>("third", &lib, std::string("wow")); // Call method that belongs to its key
	//** **//
	while (run_program) {
		std::cout << "Welcome To mp3player" << std::endl;
		std::cout << std::endl;
		Main_Menu();
//		std::cout << "To continue press (1), To print board press (2), To end game press (0):" << std::endl;
		cin >> answer;

		switch (Utilities::hashit(answer))
		{
			case(eDailyMix): {

				break;
			}
			case(eSearch): {

				break;
			}
			case(ePodcast): {
				lib.Podcasts_Menu();
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
				run_program=false;
				break;
			}
			default: {
				std::cout << "Wrong Key Was PRESSED!!! Please Try Another Time" << std::endl;
				break;
			}
		}

		std::cout << std::endl;
		std::cout << std::endl;
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
	std::cout << "Memory Leaks: " << _CrtDumpMemoryLeaks() << std::endl;
	return 0;
}
