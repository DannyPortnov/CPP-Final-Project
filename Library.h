#ifndef LIBRARY_H
#define LIBRARY_H
#include <string>
#include "Song.h"
#include "Playlists.h"
#include <map>
#include <unordered_map>
#include <array>
#include "Server.h"

using namespace std;

class Library
{
	friend ostream& operator<<(ostream& os, const Library& lib) ;
private:
	Playlist* m_favorites;
	Playlist* m_daily_mix;
	Playlist* m_recent;
	Playlist* m_most_played;
	Playlist* m_deleted;
	unordered_set<string> m_saved_playlist_names;
	set<string> m_user_playlist_names; // in order to print playlist names in alphabetical order.
	unordered_map<string, Playlist*> m_user_playlists; // store the user playlists, sorted by name of the playlist.
													   // better comlexity when using un_ordered_map.
	//map<string, Playlist*> m_user_playlists; // store the user playlists, sorted by name of the playlis

	static const int num_of_songs_to_print = 10;
	ostream& print(ostream& os, int begin, int end) const;
	//Song* Pick_Song(string song_name);
	//Podcast* Pick_Podcast(string episode_name);
	//Returns the choosen song. If wrong name returns nullptr!
	template<class T>
	T* Pick_Media(string media_name, unordered_multimap<string, T*>* collection_to_search);
	void ask_user_to_remove_song(Song* song, Playlist* playlist);
	bool check_if_user_playlist_exist(const string& playlist_name);
	bool check_if_playlist_can_be_edited(const string& playlist_name);
	void Print_Not_Found_By_Id_Error(int song_id);
public:
	Library(); //what are the Update methods?
	/*void Add(string path, string song_name, string artist = "", string album = "",
		string genre = "", string duration = "", int release_date =0);*/
	//Creates a song and adds it to the library.
	void Add_Song(string song_name, string file_path, string artist, string album, string genre, string duration, int release_Date);
	//Creates an episode and adds to a podcast. If podcast doesn't exist creates one. If episodes exists doesn't do anything
	void Add_Podcast_Episode(string episode_name, string podcast_name, string file_path,
		string duration = "", int release_Date = 0);
	//Deletes a song from the library
	void Delete(int id);
	ostream& Print(ostream& os, int begin, int end) const;
	//Deletes a song from the library
	void Delete(string song_name);
	void Print_Not_Found_By_Name_Error(std::string& song_name);
	void PrintSong(int id);
	void PrintSong(string song_name);
	void Add2PL(int id, const string& playlist_name);
	void RemoveFromPL(string& song_name, const string& playlist_name); //what if there's more than one song called like this?
	void PrintPL();
	void Update_Song(string song_name, string new_name = "", string artist = "", string album = ""
		, string genre = "", string duration = "");
	void Update_Song(int song_id, string new_name = "", string artist = "", string album = ""
		, string genre = "", string duration = "");
	
	void UpdatePodcast(string song_name, string new_name = "", string artist = "", string album = ""
		, string genre = "", string duration = "");
	void UpdateEpisode(int song_id, string new_name = "", string artist = "", string album = ""
		, string genre = "", string duration = "");
	void UpdateEpisode(string song_name, string new_name = "", string artist = "", string album = ""
		, string genre = "", string duration = "");
	//gets the data structure from Server!
	void Play(string song_name);
	//gets the data structure from Server!
	void Play(int id);

	//void print_all_playlists(); // print all playlists in library
	void create_playlist(const string& playlist_name); // create a new playlist
	void delete_playlist(Playlist* playlist); // delete a playlist
	



};

#endif