#ifndef LIBRARY_H
#define LIBRARY_H
#include <string>
#include "Song.h"
#include "Playlists.h"
#include <map>
#include <array>
using namespace std;

class Library
{
private:
	map<int,Song> m_songs_by_id;
	map<string,Song> m_songs_by_name;
	map<string, Playlists> m_playlists;

public:
	Library(); 
	//Song find_by_name(string name);
	//Song find_by_singer(string singer);
	//Song find_by_album(string singer);
	//Song find_by_genre(string singer);
	//what are the Update methods?
	void Delete(int id);
	void Delete(string song_name);
	void PrintSong(int id);
	void PrintSong(string song_name);
	void Add2PL(int id, string playlist_name);
	void RemoveFromPL(string song_name, string playlist_name); //what if there's more than one song called like this?
	void PrintPL();
};

#endif