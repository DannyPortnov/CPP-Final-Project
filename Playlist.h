#ifndef PLAYLIST_H
#define PLAYLIST_H
#define   _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include "Song.h"
#include "Server.h"
#include <algorithm>
#include <random>
#include <ctime>
#include<map>
#include<unordered_map>
#include<unordered_set>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG


class Library;

class Playlist { 
protected:
	Server* m_server;
	Library* m_library;
	using CompareSongsByName = decltype([](const Song* a, const Song* b) { return a->get_name() < b->get_name(); }); //Custom comparator for m_songs
	std::multiset<Song*, CompareSongsByName> m_songs; // uses to store all songs in the playlist in alphabetical order (use it for play). 
	std::string m_playlist_name;
	bool check_if_continue_playing();
	bool make_sure_to_remove_song(Song* song, bool make_sure = true); // double checks with the user if the song should be deleted, if yes- removes the song.
public:
	Playlist(std::string name, Library* library, Server* server, bool restore_songs = false);
	virtual ~Playlist();
	virtual void add_song_to_playlist(Song* song, bool prints_enabled = true); //works
	virtual void remove_song_from_playlist(Song* song, bool make_sure = true);
	void remove_song_from_playlist(std::string song_name, bool make_sure = true); 
	virtual void clear_all_playlist(bool prints_enabled = true);
	virtual void restore_playlist(std::string file_name);
	virtual void save_playlist(std::string file_name, std::ios_base::openmode mode = ios::out);
	std::string get_name() const { return m_playlist_name; }
	void Play(bool shuffle);
	std::multiset<Song*, Playlist::CompareSongsByName> get_songs();
	Song* get_song_by_name(std::string song_name);

friend std::ostream& operator<<(std::ostream& os, const Playlist& playlist);
};

std::ostream& operator<<(std::ostream& os, const Playlist& playlist);


#endif// PLAYER_H