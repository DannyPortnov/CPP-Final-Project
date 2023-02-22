#ifndef AUDIOFILE_H
#define AUDIOFILE_H
#include "Player.h"
#include "Date.h"
#include <set>
#include <vector>
#include <string>
#include <sstream> 
using namespace std;

class AudioFile
{
protected:
	string m_file_name;
	string m_file_path; 
	string m_duration; //m:ss format
	Date m_release_date; //todo: make Date
	int m_id_code;
	Player m_player;
	AudioFile(string file_name, string file_path, string duration="", string release_date = "");
	//works
	AudioFile(int id, string file_name, string file_path, string duration="", string release_date = "");
public:

	virtual void Play() = 0;
	const string& get_name() const; // get the name of the song
	const string& get_path() const; // get the name of the song
	const int get_id() const;
	const Date& get_release_date() const;
	const string& get_duration() const;

	virtual void set_name(string& name);
	virtual void set_duration(string& duration);
	virtual void set_release_date(Date release_date);
};

#endif	// AUDIOFILE_H
