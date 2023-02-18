#ifndef PLAYLIST_H
#define PLAYLIST_H
#include <algorithm>
#include <random>
#include <ctime>
#include<map>
#include<unordered_map>
#include<unordered_set>
#include "AudioCollection.h"
#include "Song.h"


class Playlist {

private:
	//unordered_set<string> m_songs_names; 
	multimap<string, Song*> m_songs; // uses to store all songs in the playlist in alphabetical order. 
	string m_playlist_name;
	Player m_player;
	bool check_if_continue_playing();

public:
	Playlist(string name);
	~Playlist();
	void add_song_to_playlist(Song* song);
	void remove_song_from_playlist(const string& song_name);
	//void remove_song_from_playlist_by_name(const string& song_name);
	void clear_all_playlist();
	string get_name() const { return m_playlist_name; }
	void Play();
	void Play_Random();
	void Print();
	multimap<string, Song*> get_songs();
	unordered_multimap<string, Song*>* get_songs_with_same_name(const string& song_name);

	bool check_if_songs_have_same_names(const string& song_name);
	bool check_if_song_exist_in_playlist(const string& song_name);
	Song* get_song_by_name(string song_name);
//friend bool operator<(Playlist* a, Playlist* b);

friend bool operator<(const Playlist& a, const Playlist& b); //todo: check if we need this
//friend bool operator!=(const Playlist& a, const Playlist& b);


};

// todo: implement operator < in order set stl will be able to sort
//bool operator<(Playlist* a, Playlist* b);
bool operator<(const Playlist& a, const Playlist& b);
//bool operator!=(const Playlist& a, const Playlist& b);


#endif// PLAYER_H