#ifndef AUDIOFILE_H
#define AUDIOFILE_H
#include <set>
#include <vector>
#include <string>
#include "Player.h"
using namespace std;

//todo: add Podcast class, will inherit from AudioCollection
//todo: add more meat here
//todo: podcasts will be like playlists

class AudioFile
{
protected:
	string m_file_name;
	string m_file_path; 
	string m_duration; //m:ss format
	int m_release_date;
	static int id_code;	// in this class we will be able to give id to every music file
	int m_id_code;
	Player m_player; //todo: add a counter to plays 
	AudioFile(string file_name, string file_path, string duration="", int release_date = 0);
public:

	virtual void Play() = 0;
	const string& get_name() const; // get the name of the song
	const string& get_path() const; // get the name of the song
	const int get_id() const;

	virtual void set_name(string& name);
	virtual void set_duration(string& duration);
};

#endif	// AUDIOFILE_H
