#ifndef SERVER_H
#define SERVER_H
#include <string>
#include "Song.h"
#include "Playlists.h"
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include "Podcast.h"
using namespace std;

class Server {
private:
	//sorting - O(log n) - FOR PRINTING
	static multimap<string, Song*> m_songs_by_alphabet_order;
	static multimap<string, Podcast*> m_podcasts_by_alphabet_order;

	//search - O(1), insertion - O(1), deletion - O(1) - FOR EVERYTHING ELSE
	static unordered_map<int, Song*> m_all_songs_by_id;
	static unordered_multimap<string, Song*> m_all_songs_by_artist;
	static unordered_multimap<string, Song*> m_all_songs_by_name;
	static unordered_multimap<string, Song*> m_all_songs_by_album;
	static unordered_multimap<string, Song*> m_all_songs_by_genre;
	static unordered_multimap<string, Podcast*> m_all_podcasts;
	static unordered_multiset<Song*>* find(string& key, unordered_multimap<string, Song*>& collection);

	template<typename T>
	static void remove_song_from_collection(T& songs_by_field, Song* song);
public:
	//Server();
	//getters
	static unordered_multimap<string, Song*>* get_songs_by_name(); //default comparison (by name)
	static multimap<string, Song*>* get_sorted_by_alphabet_songs(); //default comparison (by name)
	static unordered_multimap<string, Song*>*  get_songs_by_artist();
	static unordered_multimap<string, Song*>* get_songs_by_album();
	static unordered_multimap<string, Song*>* get_songs_by_genre();
	static unordered_multimap<string, Podcast*>* get_podcasts_by_name();

	static Song* find_song_by_id(int id); // returns 1 song, there is a unique ID for every song

	//search the data structures based on a parameter
	static Song* find_song_by_id(int id);
	static unordered_multiset<Song*>* find_by_name(string& name);
	static unordered_multiset<Song*>* find_by_artist(string& singer);
	static unordered_multiset<Song*>* find_by_album(string& album);
	static unordered_multiset<Song*>* find_by_genre(string& genre);

	//adds the new song to each data structure
	static void Upload_Song(Song* song);
	//Uploads a NEW podcast
	static void Upload_Podcast_Series(Podcast* episode);

	//updates an existing song - did that in Library
	//static void Update(string song_name, string new_name = "", string artist = "", string album = ""
	//	, string genre = "", string duration = "");

	//permanently deletes a song from the database COMPLETETLY
	static void Permanent_Delete_Song(Song* song);
	static void Permanent_Delete_Podcast_Episode(Episode* episode);
	static void permanent_delete_podcast(Podcast* podcast);
};
#endif

