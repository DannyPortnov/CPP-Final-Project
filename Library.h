#ifndef LIBRARY_H
#define LIBRARY_H
#include "Song.h"
#define   _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
//#include "Playlist.h"
//#include "DailyMix.h"
//#include "Favorites.h"
//#include "Most_Played.h"
//#include "Most_Recent.h"
//#include "Trash.h"
#include "Server.h"
#include <string>
#include <map>
#include <unordered_map>
#include <array>
#include <fstream>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

using namespace std;

class Playlist;
class Favorites;
class DailyMix;
class Trash;
class Automatic_Playlist;
class Most_Recent;
class Most_Played;


class Library //: Server //todo: remove inheritance
{
	friend ostream& operator<<(ostream& os, const Library& lib) ;
private:
	Server* m_server;
	set<string> m_user_playlist_names; // in order to print playlist names in alphabetical order.
	unordered_map<string, Playlist*> m_playlists; // store the user playlists, sorted by name of the playlist.
													   // better comlexity when using un_ordered_map.
	Favorites* m_favorites;
	DailyMix* m_daily_mix;
	Most_Recent* m_recent;
	Most_Played* m_most_played;
	Trash* m_deleted;
	//unordered_map<string, Playlist*> m_saved_playlist_names;

	const int num_of_songs_to_print = 10;

	template<class T>
	T* Update_Media_By_Id(int episode_id, T* (Server::* methodPtr)(int), string new_name, string duration, string release_date);
	//Returns the choosen song. If wrong name returns nullptr!
	Song* Pick_Media(string media_name, unordered_multimap<string, Song*>* collection_to_search);
	
	bool check_if_playlist_exist(const string& playlist_name); //works
	bool check_if_user_playlist(const string& playlist_name);
	bool check_if_continue_playing();
	bool make_sure_to_delete_song(Song* song);
	void Print_Not_Found_By_Id_Error(int song_id, string item_type);
	void Print_No_Input_Parameters_Error();
	void Print_Not_Found_By_Name_Error(std::string& song_name);
	void Print_Media_Exists_Error(std::string& new_name, const string & media_type);

	bool Are_All_Parameters_Empty(const string & param1, const string & param2, const string & param3, const string & param4, const string & param5);

	
	
	//void add_to_most_recent(int id);
	//todo: maybe move to private, no one needs to use this methods
	//void update_most_recent();	
	// the update of most recent happans in remove_from_most_recent() method.

	
	
	// void ask_user_to_remove_song(Song* song, Playlist* playlist); // was implemented in playlist instead
	//Song* Pick_Song(string song_name);
	//Podcast* Pick_Podcast(string episode_name);

public:
	Library(); //works
	~Library();
	//Creates a song and adds it to the library.
	void Add_Song(string song_name, string file_path, string artist, string album, string genre, string duration, string release_date);
	//Creates an episode and adds to a podcast. If podcast doesn't exist creates one. If episodes exists doesn't do anything
	void Add_Podcast_Episode(string episode_name, string podcast_name,
	string file_path, string duration = "", string release_Date="");

	void Begin_Serialization();//works
	void Begin_Deserialization();


	//Deletes a song from the library
	void Delete_Song(int id);
	//Deletes a song from the library
	void Delete_Song(string song_name);
	// method removes a specific song, used by the two Delete_Song methods
	void delete_song(Song* song_to_delete);
	//Deletes an episode from the library
	void Delete_Episode(int id);
	//Deletes an episode from the library
	void Delete_Episode(string episode_name);
	//Deletes a podcast from the library
	void Delete_Podcast(string podcast_name);


	void Add2PL(int id, const string& playlist_name, bool prints_enabled = true); //works
	void RemoveFromPL(const string& song_name, const string& playlist_name, bool make_sure = true); // added element that checks if we want to make sure if the user want to remove a song
	void add_to_favorites(Song* song);

	ostream& Print(ostream& os, int begin, int end) const;
	void PrintPL();
	void PrintSong(int id);
	void PrintSong(string song_name);

	void Update_Song(string song_name, string new_name = "",
 string artist = "", string album = "", string genre = "", string duration = "", string release_date ="");
	void Update_Song(int song_id, string new_name = "", string artist = "",
		string album = "", string genre = "", string duration = "", string release_date="");
	//Update podcast's name
	void UpdatePodcast(string podcast_name, string new_name);
	void UpdateEpisode(int episode_id, string new_name = "", string duration = "", string release_date="");
	void Update_Episode(string episode_name, string new_name = "", string duration = "", int release_date = 0); //maybe later
	
	//return the playlist that needs to be played
	Playlist* get_playlist_by_name(string playlist_name);
	//play a playlist by its name 
	void PlayPlaylist(string playlist_name);
	//play a playlist shuffled by its name 
	void PlayPlaylistShuffled(string playlist_name);


	//gets the data structure from Server!
	void Play(string song_name);
	//gets the data structure from Server!
	void Play(int id);
	//gets the data structure from Server!
	void PlayAll();
	template <template<typename, typename> class MapType>
	void PlayAll(MapType<string, Song*>*);
	//gets the data structure from Server!
	void PlayRandom();
	//play song and update song data
	void play_song(Song* song);
	void Play_Podcast(string podcast_name);

	//void print_all_playlists(); // print all playlists in library
	void create_playlist(const string& playlist_name, bool prints_enabled = true); // create a new playlist //works
	void delete_playlist(string playlist_name); // delete a playlist
	
	void remove_from_most_recent(int id);
	void update_most_played(); 
	void remove_from_daily_mix(Song* song);

	void Podcasts_Menu(const string& input);
	/*void Add(string path, string song_name, string artist = "", string album = "",
	string genre = "", string duration = "", int release_date =0);*/
};

ostream& operator<<(ostream& os, const Library& lib);

#endif