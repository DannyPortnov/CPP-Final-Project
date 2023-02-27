
#ifndef SONG_H
#define SONG_H
#include "AudioFile.h"
//#include "Library.h"
//#include <Phonon/MediaSource>
//#include <Phonon/MediaObject>
#include <iostream>
#include <unordered_set>
//#include <Qstd::string>
//#include <ctime>
//#include <chrono>
//using namespace std::chrono;



//int Song::id_code = 0;
class Song : public AudioFile
{
private:
	std::string m_artist;
	std::string m_album;
	std::string m_genre;
	static int song_id;
	//int m_song_id;
	int m_plays_counter; // counter for the amount of times the song was played
	std::unordered_set<std::string> m_playlist_appearances; // stores the names of the playlists that have this song
public:
	
	Song(std::string song_name, std::string file_path,
	std::string album="",std::string artist="", std::string genre="", std::string release_date="", std::string duration = "");
	//works
	Song(int id, std::string song_name, std::string file_path,
		std::string album, std::string singer, std::string genre, std::string release_date, std::string duration, int plays_count);
	void Play(); //plays a song // todo: maybe to move to an abstract class: audioFile
	
	const std::string& get_album() const; 
	const std::string& get_genre() const;
	const std::string& get_artist() const;
	const int get_plays_count() const;
	std::unordered_set<std::string>* get_playlist_appearances();

	void set_artist(std::string& artist);
	void set_album(std::string& album);
	void set_genre(std::string& genre);
	void set_playlist_appearances(const std::string& playlist);
	void remove_from_playlist(const std::string& playlist);
	void clear_from_all_playlists();

	void update_plays_counter();

	void print_playlists() const;
	//Song& operator=(const Song& exisiting_song);
	
friend bool operator<(const Song& a, const Song& b);
friend ostream& operator<<(std::ostream& os, const Song& song);
//friend bool operator!=(const Song& a, const Song& b);
};

bool operator<(const Song& a, const Song& b);
std::ostream& operator<<(std::ostream& os, const Song& song);

//bool operator!=(const Song& a, const Song& b);

#endif
