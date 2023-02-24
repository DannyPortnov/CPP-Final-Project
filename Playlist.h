#ifndef PLAYLIST_H
#define PLAYLIST_H
#include "AudioCollection.h"
#include "Song.h"
#include "Server.h"
//#include "Library.h"
#include <algorithm>
#include <random>
#include <ctime>
#include<map>
#include<unordered_map>
#include<unordered_set>


class Library;
class Playlist { //todo: make deleted, recent, favorites classes which derive from playlist
	//todo: make a library pointer inside playlist
private:
	Player m_player;
	bool check_if_continue_playing();
	bool make_sure_to_remove_song(Song* song, bool make_sure = true); // double checks with the user if the song should be deleted, if yes- removes the song.
protected:
	multimap<string, Song*> m_songs; // uses to store all songs in the playlist in alphabetical order. 
	// chose to use multimap because if we would choose to store in unordered_multimap and use another data structure
	// for the names of the song (oredered alphabeticaly), we will still need to search the song inside the multimap,
	// the implementation will be complicated.
	string m_playlist_name;


public:
	Playlist(string name);
	~Playlist();
	virtual void add_song_to_playlist(Song* song); //works
	virtual void remove_song_from_playlist(Song* song, bool make_sure = true);
	virtual void clear_all_playlist();
	string get_name() const { return m_playlist_name; }
	void Play();
	void Play_Random();
	void Print();
	multimap<string, Song*> get_songs();
	bool check_if_song_exist_in_playlist_by_id(int id); //works
	Song* get_song_by_name(string song_name);
	int count_song_name_appearences(string song_name); // count the number of songs with the same name

	//void remove_song_from_playlist_by_name(const string& song_name);
	//bool check_if_songs_have_same_names(const string& song_name);
	//bool check_if_song_exist_in_playlist_by_name(const string& song_name);
	//unordered_multimap<string, Song*>* get_songs_with_same_name(const string& song_name);
//friend bool operator<(Playlist* a, Playlist* b);

friend bool operator<(const Playlist& a, const Playlist& b); //todo: check if we need this
//friend bool operator!=(const Playlist& a, const Playlist& b);


};

// todo: implement operator < in order set stl will be able to sort
//bool operator<(Playlist* a, Playlist* b);
bool operator<(const Playlist& a, const Playlist& b);
//bool operator!=(const Playlist& a, const Playlist& b);


#endif// PLAYER_H