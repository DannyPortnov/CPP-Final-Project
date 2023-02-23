#ifndef SERVER_H
#define SERVER_H
#include "Song.h"
#include "Podcast.h"
#include "Episode.h"
#include "Utilities.h"
#include <string>
#include <set>
#include <map>
#include <list>
#include <unordered_map>
#include <unordered_set>
using namespace std;

#define max_recents 30 // 30 and not 10 because we want to save some spare songs


class Server {
	
private:
	//sorting - O(log n) - FOR PRINTING
	static multimap<string, Song*>* m_songs_by_alphabet_order;
	static map<string, Podcast*>* m_podcasts_by_alphabet_order;

	//search - O(1), insertion - O(1), deletion - O(1) - FOR EVERYTHING ELSE
	static unordered_map<int, Episode*>* m_all_episodes_by_id;
	static unordered_map<string, Episode*>* m_all_episodes_by_name;
	static unordered_map<string, Podcast*>* m_all_podcasts;

	static unordered_map<int, Song*>* m_all_songs_by_id;
	static unordered_set<string>* m_songs_file_paths;  //Nessecary to check songs uniqueness in the server (can't add same song twice)
	static unordered_multimap<string, Song*>* m_all_songs_by_artist;
	static unordered_multimap<string, Song*>* m_all_songs_by_name;
	static unordered_multimap<string, Song*>* m_all_songs_by_album;
	static unordered_multimap<string, Song*>* m_all_songs_by_genre;

	static list<Song*>* m_recently_played; // good complexity for insertion/deletion O(1)
	static unordered_map<int, Song*>* m_recently_played_by_id; // good complexity for searching song in playlist O(1) (search by id)

	static multimap<int, Song*>* m_most_played; // songs in an oreder from least played to most played

	static unordered_multimap<string, Song*>* find_all(string& key, unordered_multimap<string, Song*>* collection);

	template<class T>
	static void remove_song_from_collection(T* songs_by_field, Song* song);
	template <class T>
	static void Destory_Allocations(T* collection);
	template <class T>
	static void Clear_And_Delete(T* collection);
	//Finds a unique element in an unordered_map with the given key parameter.
	//Throws an exception if the TKey is not present in the unordered_map.
	template<class TKey, class TValue>
	static TValue* Find_Unique(TKey param, unordered_map<TKey, TValue*>* collection_to_Search);
	static void Add_Song_To_Collections(Song*& song); //works
public:
	Server(); //works
	~Server();

	static void Destroy_All_Allocations();
	//getters
	static unordered_map<int, Song*>* get_songs_by_id(); //todo: implement
	static unordered_multimap<string, Song*>* get_songs_by_name(); //default comparison (by name)
	//Returns the unique episode with given name
	static multimap<string, Song*>* get_songs_sorted_by_alphabet(); //default comparison (by name)
	static unordered_multimap<string, Song*>*  get_songs_by_artist();
	static unordered_multimap<string, Song*>* get_songs_by_album();
	static unordered_multimap<string, Song*>* get_songs_by_genre();

	static list<Song*>* get_recently_played();
	static multimap<int, Song*>* get_most_played();

	static Episode* find_episode_by_id(int id); // returns 1 episode, there is a unique ID for every episode
	static Episode* find_episode_by_name(string name);
	static Podcast* find_podcast_by_name(string name);
	static Song* find_song_by_id(int id); // returns 1 song, there is a unique ID for every song
	//search the data structures based on a parameter
	static unordered_multimap<string, Song*>* find_by_name(string& name);
	static unordered_multimap<string, Song*>* find_by_artist(string& singer);
	static unordered_multimap<string, Song*>* find_by_album(string& album);
	static unordered_multimap<string, Song*>* find_by_genre(string& genre);


	//Checks if song was already added to server
	static bool Does_Song_Exist(const string& file_path);
	//Checks if episode was already added to server
	static bool Does_Episode_Exist(string& episode_name);
	static bool Does_Podcast_Exist(string& podcast_name);

	//Allocates memory for the new song and adds to each data structure
	static void Upload_Song(string song_name, string file_path, string artist = "",
		string album = "", string genre = "", string duration = "", string release_Date = "");
	//Creates an episode and adds to a podcast. If podcast doesn't exist creates one. If episodes exists doesn't do anything
	static void Upload_Episode_To_Podcast(Podcast* podcast, string episode_name, string podcast_name, string file_path,
		string duration, string release_Date);

	//Loads all songs saved on file (serialization)
	static void Restore_Songs(); //works
	static void Restore_Podcasts();
	static void Restore_Most_Recent();

	//Saves all songs into a file (deserialization)
	static void Save_Songs();

	// add song to recently played by song id
	static void add_to_recently_played(int id);
	// remove a song from recently played data structure
	static void remove_from_recently_played(int id);
	//void update_recently_played(int id);

	// update most played by checking the amount of times a song was played
	static void update_most_played_songs();

	//permanently deletes a song from the database COMPLETETLY
	static void Permanent_Delete_Song(Song* song);
	//Removes the episode from all listings and from its podcast
	static void Permanent_Delete_Podcast_Episode(Episode* episode);
	//Deletes a podcast and each episode in it
	static void Permanent_Delete_Podcast(Podcast* podcast);

	//updates an existing song - did that in Library
//static void Update(string song_name, string new_name = "", string artist = "", string album = ""
//	, string genre = "", string duration = "");
};
#endif

