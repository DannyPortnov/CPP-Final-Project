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
	fstream read("c:\\temp\\songs.dat", ios::in);
	string song_name, artist, album, genre, duration, release_date, file_path;

	//int id;
	while (!read.eof()) {
		read >> song_name >> file_path >> artist >> album >> genre >> duration >> release_date;
		Library::Add_Song(song_name, file_path, artist, album, genre, duration, release_date);
		char c1 = read.get(); //Skips the \n at the end of line
		char c2 = read.peek(); //Peeks at the start of the next line
		if (c2 == '\n') //if the next line is also \n, quit
		{
			break;
		}
	}
	//....
	fstream write("c:\\temp\\songs.dat", ios::out);
	unordered_map<int, Song*>::iterator itr;
	for (itr = Server::get_songs_by_id()->begin(); itr != Server::get_songs_by_id()->end(); itr++)
	{
		auto song = itr->second;
		write << song->get_name() << " " << song->get_path()  << " " << song->get_artist() << " " << song->get_album()
			<< " " << song->get_genre() << " " << song->get_duration() << " " << song->get_release_date() << endl;
	}






}

int main()
{
	mp3player();
	cout << "Memory Leaks: " << _CrtDumpMemoryLeaks() << endl;
	return 0;
}
