#define   _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <iostream>
#include <fstream>
#include <string>
//#include "Library.h"
//#include "Server.h"
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
	fstream read("c:\\temp\\songs_out.dat", ios::in);
	string song_name, artist, album, genre, duration, release_date, file_path;
	fstream write("c:\\temp\\songs.dat", ios::out);
	string buffer;
	//int id;
	while (!read.eof()) {
		read >> song_name >> file_path >> artist >> album >> genre >> duration >> release_date;
		write << song_name << " " << file_path << " " << artist << " " << album << " " << genre << " " << duration << " " <<
			release_date << endl;
		char c1 = read.get(); //Skips the \n at the end of line
		char c2 = read.peek(); //Peeks at the start of the next line
		if (c2 == '\n') //if the next line is also \n, quit
		{
			break;
		}
		//in.getline(buf, 80);
		//cout << buf << endl;
	}
	//Server::get_all_songs()








}

int main()
{
	mp3player();
	cout << "Memory Leaks: " << _CrtDumpMemoryLeaks() << endl;
	return 0;
}
