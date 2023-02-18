#ifndef SONG_H
#define SONG_H
#include "AudioFile.h"
//#include <Phonon/MediaSource>
//#include <Phonon/MediaObject>
#include <iostream>
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
	int m_plays_counter; // counter for the amount of times the song was played
public:
	Song(string song_name, string file_path,
	string album="",string artist="", string genre="", int release_date=0, string duration = "");
	void Play(); //plays a song // todo: maybe to move to an abstract class: audioFile

	
	const string& get_album() const; 
	const string& get_genre() const;
	const string& get_artist() const; 

	void set_artist(string& artist);
	void set_album(string& album);
	void set_genre(string& genre);
	void update_plays_counter();

	//Song& operator=(const Song& exisiting_song);
	
friend bool operator<(const Song& a, const Song& b);
friend ostream& operator<<(ostream& os, const Song& song);
//friend bool operator!=(const Song& a, const Song& b);
};

bool operator<(const Song& a, const Song& b);
ostream& operator<<(ostream& os, const Song& song);

//bool operator!=(const Song& a, const Song& b);

#endif
