#ifndef SERVER_H
#define SERVER_H
#include <string>
#include "Song.h"
#include "Playlists.h"
#include <set>
using namespace std;

class Server {
private:
	static set<Song*>* m_all_songs_by_id;
	static multiset<Song*>* m_all_songs_by_artist;
	static multiset<Song*>* m_all_songs_by_name;
	static multiset<Song*>* m_all_songs_by_album;
	static multiset<Song*>* m_all_songs_by_genre; //change to unordered_map for O(1) find/access functionality
public:
	Server();
	static multiset<Song*>* get_songs_by_name(); //default comparison (by name)
	static set<Song*>*  get_songs_by_id();
	static multiset<Song*>*  get_songs_by_artist();
	static multiset<Song*>* get_songs_by_album();
	static multiset<Song*>* get_songs_by_genre();
	static multiset<Song*>* find_by_name(string name);
	static multiset<Song*>* find_by_singer(string singer);
	static multiset<Song*>* find_by_album(string album);
	static multiset<Song*>* find_by_genre(string genre);
};
#endif