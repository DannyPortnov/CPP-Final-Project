#ifndef LIBRARY_H
#define LIBRARY_H
#define NOMINMAX //HAS TO BE FIRST
#include "Utilities.h"
#include "Song.h"
#define   _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include "Server.h"
#include <string>
#include <map>
#include <unordered_map>
#include <array>
#include <fstream>
#include <limits> 
#include <regex>
#include <optional>
#include <random>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

class Playlist;
class Favorites;
class DailyMix;
class Trash;
class Automatic_Playlist;
class Most_Recent;
class Most_Played;

#define playlists_file_name "playlists"


class Library 
{
	friend ostream& operator<<(ostream& os, const Library& lib) ;
private:
	Server* m_server;
	std::set<string> m_user_playlist_names; // in order to print playlist names in alphabetical order.
	std::unordered_map<string, Playlist*> m_playlists; // store the user playlists, sorted by name of the playlist.
													   // better comlexity when using unordered_map.
	Favorites* m_favorites;
	DailyMix* m_daily_mix;
	Most_Recent* m_recent;
	Most_Played* m_most_played;
	Trash* m_deleted;

	const int num_of_songs_to_print = 10;
	static mt19937 m_gen;
	template<class T>
	T* Update_Media_By_Id(int episode_id, T* (Server::* methodPtr)(int), std::string new_name, std::string duration, std::string release_date);
	//Returns the choosen song. If wrong name returns nullptr!
	Song* Pick_Media(std::string media_name, unordered_multimap<string, Song*>* collection_to_search);
	
	bool check_if_playlist_exist(const std::string& playlist_name);
	bool check_if_user_playlist(const std::string& playlist_name);
	bool check_if_continue_playing();

	void Print_Not_Found_By_Id_Error(int song_id, std::string item_type);
	void Print_No_Input_Parameters_Error();
	void Print_Not_Found_By_Name_Error(std::string& song_name);
	void Print_Media_Exists_Error(std::string& new_name, const std::string & media_type);
	
	bool Are_All_Parameters_Empty(const std::string & param1, const std::string & param2, const std::string & param3, const std::string & param4, const std::string & param5);

public:
	Library(); //works
	~Library();
	//Creates a song and adds it to the library.
	void Add_Song(std::string song_name, std::string file_path, std::string artist, std::string album, std::string genre, std::string duration, std::string release_date);
	//Creates an episode and adds to a podcast. If podcast doesn't exist creates one. If episodes exists doesn't do anything
	void Add_Podcast_Episode(std::string episode_name, std::string podcast_name,
	std::string file_path, std::string duration = "", std::string release_Date="");

	void Begin_Deserialization();//works
	void Begin_Serialization();

	static mt19937 Get_Gen();

	//Deletes a song from the library
	void Delete_Song(int id);
	//Deletes a song from the library
	void Delete_Song(std::string song_name);
	// method removes a specific song, used by the two Delete_Song methods
	void delete_song(Song* song_to_delete);
	//Deletes an episode from the library
	void Delete_Episode(int id);
	//Deletes an episode from the library
	void Delete_Episode(std::string episode_name);
	//Deletes a podcast from the library
	void Delete_Podcast(std::string podcast_name);


	void Add2PL(int id, const std::string& playlist_name, bool prints_enabled = true); 
	void RemoveFromPL(const std::string& song_name, const std::string& playlist_name, bool make_sure = true); // added element that checks if we want to make sure if the user want to remove a song

	ostream& Print(ostream& os, int begin, int end) const;
	void PrintPL();
	void PrintSong(int id);
	void PrintSong(std::string song_name);
	void PrintPlaylist(std::string playlist_name); // print a specific playlist data

	void Update_Song(std::string song_name, std::string new_name = "", std::string artist = "",
	 std::string album = "", std::string genre = "", std::string duration = "", std::string release_date ="");
	void Update_Song(int song_id, std::string new_name = "", std::string artist = "",
		std::string album = "", std::string genre = "", std::string duration = "", std::string release_date="");
	//Update podcast's name
	void UpdatePodcast(std::string podcast_name, std::string new_name);
	void UpdateEpisode(int episode_id, std::string new_name = "", std::string duration = "", std::string release_date="");
	
	//return the playlist that needs to be played
	Playlist* get_playlist_by_name(std::string playlist_name, bool prints_enable = true);
	//play a playlist by its name 
	void PlayPlaylist(std::string playlist_name);
	//play a playlist shuffled by its name 
	void PlayPlaylistShuffled(std::string playlist_name);


	void Play(std::string song_name);
	void Play(int id);
	void PlayAll(bool shuffle);
	void PlayAll(std::vector<Song*>* songs_to_play, const std::string& message, bool shuffle, bool delete_ptr);
	//play song and update song data
	void play_song(Song* song);
	void Play_Podcast(std::string podcast_name);

	void create_playlist(const std::string& playlist_name, bool prints_enabled = true); // create a new playlist 
	void delete_playlist(std::string playlist_name); // delete a playlist
	
	void update_most_played(); 
	void update_most_recent();
	void remove_from_daily_mix(Song* song);

	DailyMix* Get_DailyMix();
	Favorites* Get_Favorites();
	Most_Played* Get_Most_Played();
	Most_Recent* Get_Most_Recent();
	Trash* Get_Deleted();
	Server* Get_Server();

};

ostream& operator<<(ostream& os, const Library& lib);

#endif