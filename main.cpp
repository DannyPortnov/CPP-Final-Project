#define   _CRTDBG_MAP_ALLOC
//#include "MethodMap.h"
#include "Library.h"
#include "Mp3Interface.h"
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



void mp3player() {
	Library lib;
	Mp3Interface mp3(&lib);
	mp3.Run_Program();
	//MethodMap methodmap;
	std::string key = "hello";
	// ** How to use MethodMap for methods with any parameters** //
	//methodmap.Insert<std::string>("second", &Library::Example_Func_For_MethodMap); //Add method to map with its key
	////methodmap.Insert<void>("first", &Library::Example_Func_For_MethodMap); //Add method to map with its key
	//methodmap.Insert("third", &Library::Example2_Func_For_MethodMap);
	//methodmap.Call<void>("first", &lib); // Call method that belongs to its key
	//methodmap.Call<void>("second", &lib, std::string("wow")); // Call method that belongs to its key
	//methodmap.Call<void>("third", &lib, std::string("wow")); // Call method that belongs to its key
	//** **//
	
	
	//lib.Begin_Serialization();

	//lib.Begin_Deserialization();

	//int id;
	
	//....

}
//
//void test_library_menu() {
//	Library lib;
//	lib.Library_Menu();
//}
//
//void test_playlists_menu() {
//	Library lib;
//	lib.Playlists_Menu();
//}

int main()
{
	mp3player();
	//test_library_menu();
	//test_playlists_menu();
	//Server::Destroy_All_Allocations();
	std::cout << "Memory Leaks: " << _CrtDumpMemoryLeaks() << std::endl;
	return 0;
}
