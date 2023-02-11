#ifndef SONG_H
#define SONG_H
#include <string>
#include "Player.h"
//#include <Phonon/MediaSource>
//#include <Phonon/MediaObject>
//#include <iostream>
//#include <Qstring>
//#include <ctime>
//#include <chrono>
//using namespace std::chrono;
using namespace std;

int Song::id_code = 0;

class Song
{
private:
	string m_song_name;
	string m_artist;
	string m_album;
	string m_genre;
	string m_file_path;
	int m_release_date;
	string m_duration; //m:ss format
	static int id_code;
	int m_id_code;
	Player m_player;
public:
	Song(string song_name, string file_path, string album="",
		string artist="", string genre="", int release_date=0);
	void Play(); //plays a song
	const string& get_name() const; // get the name of the song
	const string& get_path() const; // get the name of the song

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
