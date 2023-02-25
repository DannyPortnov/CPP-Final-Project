
#ifndef SONG_H
#define SONG_H
#include "AudioFile.h"
//#include "Library.h"
//#include <Phonon/MediaSource>
//#include <Phonon/MediaObject>
#include <iostream>
#include <unordered_set>
//#include <Qstring>
//#include <ctime>
//#include <chrono>
//using namespace std::chrono;
using namespace std;


//int Song::id_code = 0;
class Song : public AudioFile
{
private:
	string m_artist;
	string m_album;
	string m_genre;
	static int song_id;
	//int m_song_id;
	int m_plays_counter; // counter for the amount of times the song was played
	unordered_set<string> m_playlist_appearances; // stores the names of the playlists that have this song
public:
	
	Song(string song_name, string file_path,
	string album="",string artist="", string genre="", string release_date="", string duration = "");
	//works
	Song(int id, string song_name, string file_path,
		string album, string singer, string genre, string release_date, string duration, int plays_count);
	void Play(); //plays a song // todo: maybe to move to an abstract class: audioFile
	
	const string& get_album() const; 
	const string& get_genre() const;
	const string& get_artist() const;
	const int get_plays_count() const;
	unordered_set<string>* get_playlist_appearances();

	void set_artist(string& artist);
	void set_album(string& album);
	void set_genre(string& genre);
	void set_playlist_appearances(const string& playlist);
	void remove_from_playlist(const string& playlist);
	void clear_from_all_playlists();

	void update_plays_counter();

	void print_playlists() const;
	//Song& operator=(const Song& exisiting_song);
	
friend bool operator<(const Song& a, const Song& b);
friend ostream& operator<<(ostream& os, const Song& song);
//friend bool operator!=(const Song& a, const Song& b);
};

bool operator<(const Song& a, const Song& b);
ostream& operator<<(ostream& os, const Song& song);

//bool operator!=(const Song& a, const Song& b);

#endif
