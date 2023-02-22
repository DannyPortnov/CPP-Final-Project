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


void mp3player() {
	int answer;
	bool run_program = true;
	cout << "Welcome To mp3player" << endl;
	cout << endl;
	Library::Begin_Serialization();
	Library::Begin_Deserialization();

	//int id;
	
	//....

}

int main()
{
	mp3player();
	cout << "Memory Leaks: " << _CrtDumpMemoryLeaks() << endl;
	return 0;
}
