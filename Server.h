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
	map<int, Song> m_all_songs_by_id;
	map<string, Song> m_all_songs_by_artist;
	map<string, Song> m_all_songs_by_name;
	map<string, Song> m_all_songs_by_album;
	map<string, Song> m_all_songs_by_genre;
public:
	Server();
};
#endif