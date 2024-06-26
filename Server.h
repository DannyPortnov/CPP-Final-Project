#ifndef SERVER_H
#define SERVER_H
#define   _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
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
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#define songs_file_name "songs"
#define podcasts_file_name "podcasts"
#define most_recent_file_name "Most_Recent"

//	The Server class represents a music server that can storeand manage songs,
//  podcasts, and episodes. The class has several data structures 
//	for sorting, searching, and storing the data.
class Server {
	
private:
	//sorting - O(log n) - FOR PRINTING
	std::multimap<std::string, Song*>* m_songs_by_alphabet_order;
	std::map<std::string, Podcast*>* m_podcasts_by_alphabet_order;

	//search - O(1), insertion - O(1), deletion - O(1) - FOR EVERYTHING ELSE
	std::unordered_map<int, Episode*>* m_all_episodes_by_id;
	std::unordered_map<std::string, Episode*>* m_all_episodes_by_name;
	std::unordered_map<std::string, Podcast*>* m_all_podcasts;

	std::unordered_map<int, Song*>* m_all_songs_by_id;
	std::unordered_set<std::string>* m_songs_file_paths;  //Nessecary to check songs uniqueness in the server (can't add same song twice)
	std::unordered_multimap<std::string, Song*>* m_all_songs_by_artist;
	std::unordered_multimap<std::string, Song*>* m_all_songs_by_name;
	std::unordered_multimap<std::string, Song*>* m_all_songs_by_album;
	std::unordered_multimap<std::string, Song*>* m_all_songs_by_genre;

	list<Song*>* m_recently_played; // good complexity for insertion/deletion O(1)
	unordered_map<int, Song*>* m_recently_played_by_id; // good complexity for searching song in playlist O(1) (search by id)

	multimap<int, Song*>* m_most_played; // songs in an order from least played to most played

	std::unordered_multimap<std::string, Song*>* find_all(std::string& key, std::unordered_multimap<std::string, Song*>* collection);

	template<class T>
	void remove_song_from_collection(T* songs_by_field, Song* song);
	template <class T>
	void Destory_Allocations(T* collection);
	template <class T>
	void Clear_And_Delete(T* collection);
	//Finds a unique element in an unordered_map with the given key parameter.
	//Throws an exception if the TKey is not present in the unordered_map.
	template<class TKey, class TValue>
	TValue* Find_Unique(TKey param, unordered_map<TKey, TValue*>* collection_to_Search);
	void Add_Song_To_Collections(Song*& song); //works
public:
	Server(); //works
	~Server();

	void Destroy_All_Allocations();
	//getters
	std::unordered_map<int, Song*>* get_songs_by_id(); 
	std::unordered_multimap<std::string, Song*>* get_songs_by_name(); //default comparison (by name)
	//Returns the unique episode with given name
	multimap<std::string, Song*>* get_songs_sorted_by_alphabet(); //default comparison (by name)
	std::unordered_multimap<std::string, Song*>*  get_songs_by_artist();
	std::unordered_multimap<std::string, Song*>* get_songs_by_album();
	std::unordered_multimap<std::string, Song*>* get_songs_by_genre();
	std::unordered_map<int, Episode*>* get_episodes_by_id();
	std::unordered_map<std::string, Podcast*>* get_podcasts();

	std::list<Song*>* get_recently_played();
	std::multimap<int, Song*>* get_most_played();

	Episode* find_episode_by_id(int id); // returns 1 episode, there is a unique ID for every episode
	Episode* find_episode_by_name(std::string name);
	Podcast* find_podcast_by_name(std::string name);
	Song* find_song_by_id(int id); // returns 1 song, there is a unique ID for every song
	//search the data structures based on a parameter
	std::unordered_multimap<std::string, Song*>* find_by_name(std::string& name);
	std::unordered_multimap<std::string, Song*>* find_by_artist(std::string& singer);
	std::unordered_multimap<std::string, Song*>* find_by_album(std::string& album);
	std::unordered_multimap<std::string, Song*>* find_by_genre(std::string& genre);


	//Checks if song was already added to server
	bool Does_Song_Exist(const std::string& file_path);
	//Checks if episode was already added to server
	bool Does_Episode_Exist(std::string& episode_name);
	bool Does_Podcast_Exist(std::string& podcast_name);

	//Allocates memory for the new song and adds to each data structure
	void Upload_Song(std::string song_name, std::string file_path, std::string artist = "",
		std::string album = "", std::string genre = "", std::string duration = "", std::string release_Date = "");
	//Creates an episode and adds to a podcast. If podcast doesn't exist creates one. If episodes exists doesn't do anything
	void Upload_Episode_To_Podcast(Podcast* podcast, std::string episode_name, std::string podcast_name, std::string file_path,
		std::string duration, std::string release_Date);

	//Loads all songs saved on file (serialization)
	void Restore_Songs(); 
	void Restore_Podcasts();
	void Restore_Most_Recent();

	//Saves all songs into a file (deserialization)
	void Save_Songs();
	void Save_Most_Recent();
	void Save_Podcasts();

	// add song to recently played by song id
	void add_to_recently_played(int id);
	// remove a song from recently played data structure
	void remove_from_recently_played(int id);
	//void update_recently_played(int id);

	// update most played by checking the amount of times a song was played
	void update_most_played_songs();

	//permanently deletes a song from the database COMPLETETLY
	void Permanent_Delete_Song(Song* song);
	//Removes the episode from all listings and from its podcast
	void Permanent_Delete_Podcast_Episode(Episode* episode, bool make_sure = false, bool delete_from_podcast = true);
	//Deletes a podcast and each episode in it
	void Permanent_Delete_Podcast(Podcast* podcast, bool make_sure = false);
};
#endif

