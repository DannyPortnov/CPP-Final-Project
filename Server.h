#ifndef SERVER_H
#define SERVER_H
#include <string>
#include "Song.h"
#include "Playlists.h"
#include <map>
#include <array>
using namespace std;

class Server {
private:
	static map<int, Song>* m_all_songs_by_id;
	static multimap<string, Song>* m_all_songs_by_artist;
	static multimap<string, Song>* m_all_songs_by_name;
	static multimap<string, Song>* m_all_songs_by_album;
	static multimap<string, Song>* m_all_songs_by_genre;
public:
	Server();
	static multimap<string, Song>* get_songs_by_name();
	static map<int, Song>*  get_songs_by_id();
	static multimap<string, Song>*  get_songs_by_artist();
	static multimap<string, Song>*  get_songs_by_album();
	static multimap<string, Song>*  get_songs_by_genre();
	static multimap<string, Song>* find_by_name(string name);
	static multimap<string, Song>* find_by_singer(string singer);
	static multimap<string, Song>* find_by_album(string album);
	static multimap<string, Song>* find_by_genre(string genre);
};
#endif