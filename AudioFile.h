#ifndef AUDIOFILE_H
#define AUDIOFILE_H
#include <set>
#include <vector>
#include <string>
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

public:
	virtual void Play() = 0;
};

#endif	// AUDIOFILE_H
