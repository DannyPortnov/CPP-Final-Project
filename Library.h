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
	friend ostream& operator<<(ostream& os, const Library& lib) ;
private:
	static const int num_of_songs_to_print = 10;
	map<int,Song*>* m_songs_by_id;
	multiset<Song*>* m_songs_by_name;
	Playlists* m_playlists;  //maybe static
	ostream& print(ostream& os, int begin, int end) const;
	int Count_Songs(multiset<Song*>* songs, string song_name, multiset<Song*>::iterator* start, multiset<Song*>::iterator* end,
		multiset<Song*>::iterator* first_wanted_song, multiset<Song*>::iterator* last_wanted_song) const;
	Song* Pick_Song(int number_of_songs, multiset<Song*>::iterator* start, multiset<Song*>::iterator* end);
public:
	Library(); //what are the Update methods?
	/*void Add(string path, string song_name, string artist = "", string album = "",
		string genre = "", string duration = "", int release_date =0);*/
	//Adds a song to the library
	void Add(string song_name);
	//Deletes a song from the library
	void Delete(int id);
	//Deletes a song from the library
	void Delete(string song_name);
	void PrintSong(int id);
	void PrintSong(string song_name);
	void Add2PL(int id, string playlist_name);
	void RemoveFromPL(string song_name, string playlist_name); //what if there's more than one song called like this?
	void PrintPL();

	
};

#endif