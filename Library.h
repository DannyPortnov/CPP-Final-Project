#ifndef LIBRARY_H
#define LIBRARY_H
#include <string>
#include "Song.h"
#include "Playlists.h"
#include <map>
#include <array>
#include "Server.h"

using namespace std;

class Library
{
private:
	map<int,Song>* m_songs_by_id;
	multimap<string,Song>* m_songs_by_name;
	map<string, Playlists>* m_playlists;
public:
	//Library(); //what are the Update methods?
	//Adds a song to the library
	/*void Add(string path, string song_name, string artist = "", string album = "",
		string genre = "", string duration = "", int release_date =0);*/
	void Add(string song_name);
	void Delete(int id);
	void Delete(string song_name);
	void PrintSong(int id);
	void PrintSong(string song_name);
	void Add2PL(int id, string playlist_name);
	void RemoveFromPL(string song_name, string playlist_name); //what if there's more than one song called like this?
	void PrintPL();
};

#endif