#ifndef LIBRARY_H
#define LIBRARY_H
#include <string>
#include "Song.h"
#include "Playlists.h"
#include <map>
#include <unordered_map>
#include <array>
#include "Server.h"

using namespace std;

class Library
{
	friend ostream& operator<<(ostream& os, const Library& lib) ;
private:
	Playlist* m_favorites;
	Playlist* m_daily_mix;
	Playlist* m_recent;
	Playlist* m_most_played;
	Playlist* m_deleted;
	set<string> m_user_playlist_names; // in order to print playlist names in alphabetical order.
	unordered_map<string, Playlist*> m_user_playlists; // store the user playlists, sorted by name of the playlist.
													   // better comlexity when using un_ordered_map.
	//map<string, Playlist*> m_user_playlists; // store the user playlists, sorted by name of the playlist




	static const int num_of_songs_to_print = 10;
	ostream& print(ostream& os, int begin, int end) const;
	int Count_Songs(multiset<Song*>* songs, string song_name, multiset<Song*>::iterator* start, multiset<Song*>::iterator* end,
		multiset<Song*>::iterator* first_wanted_song, multiset<Song*>::iterator* last_wanted_song) const;
	Song* Pick_Song(string song_name);
	void Update_Playlists_Map();





public:
	Library(); //what are the Update methods?
	/*void Add(string path, string song_name, string artist = "", string album = "",
		string genre = "", string duration = "", int release_date =0);*/
	//Adds a song to the library. Maybe asks to which playlist, and allows to create a new playlist? 
	void Add(string song_name, string file_path, string artist, string album, string genre, string duration, int release_Date);
	//Deletes a song from the library
	void Delete(int id);
	//Deletes a song from the library
	void Delete(string song_name);
	void PrintSong(int id);
	void PrintSong(string song_name);
	void Add2PL(int id, const string& playlist_name);
	void RemoveFromPL(string song_name, const string& playlist_name); //what if there's more than one song called like this?
	void PrintPL();
	void Update(string song_name, string new_name = "", string artist = "", string album = ""
		, string genre = "", string duration = "");
	void Update(int song_id, string new_name = "", string artist = "", string album = ""
		, string genre = "", string duration = "");
	//gets the data structure from Server!
	void Play(string song_name);
	//gets the data structure from Server!
	void Play(int id);

	void print_all_playlists(); // print all playlists in library
	void create_playlist(const string& playlist_name); // create a new playlist
	void delete_playlist(Playlist* playlist); // delete a playlist
	bool check_if_playlist_exist(const string& playlist_name);
	bool check_if_playlist_can_be_edited(const string& playlist_name);
	char ask_user_to_remove_song(int id, const string& playlist_name);

};

#endif