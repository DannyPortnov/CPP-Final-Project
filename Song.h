#ifndef SONG_H
#define SONG_H
#include <string>
#include "Player.h"
//#include <Phonon/MediaSource>
//#include <Phonon/MediaObject>
#include <iostream>
//#include <Qstring>
//#include <ctime>
//#include <chrono>
//using namespace std::chrono;
using namespace std;


//int Song::id_code = 0;
class Song
{
private:
	string m_song_name; // todo: maybe to move to an abstract class: audioFile
	string m_artist;
	string m_album;
	string m_genre;
	string m_file_path; // todo: maybe to move to an abstract class: audioFile
	int m_release_date; // todo: maybe to move to an abstract class: audioFile
	string m_duration; //m:ss format
	static int id_code; // todo: maybe to move to an abstract class: audioFile
	int m_id_code; // todo: maybe to move to an abstract class: audioFile
	Player m_player; //todo: add a counter to plays // todo: maybe to move to an abstract class: audioFile
public:
	Song(string song_name, string file_path, string album="",
		string artist="", string genre="", int release_date=0);
	void Play(); //plays a song // todo: maybe to move to an abstract class: audioFile
	const string& get_name() const; // get the name of the song
	const string& get_path() const; // get the name of the song
	const string& get_album() const; // get the name of the song
	const string& get_genre() const; // get the name of the song
	const string& get_artist() const; // get the name of the song

	const int get_id() const; // get the name of the song

	//Song& operator=(const Song& exisiting_song);
	
friend bool operator<(const Song& a, const Song& b);
friend ostream& operator<<(ostream& os, const Song& song);
//friend bool operator!=(const Song& a, const Song& b);
};

bool operator<(const Song& a, const Song& b);
ostream& operator<<(ostream& os, const Song& song);

//bool operator!=(const Song& a, const Song& b);

#endif
