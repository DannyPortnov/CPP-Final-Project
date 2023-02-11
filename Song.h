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
	friend ostream& operator<<(ostream& os, const Song& song);
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
	string get_name(); // get the name of the song
	int get_id(); // get id of the song

};

#endif
