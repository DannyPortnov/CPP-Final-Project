#ifndef AUDIOFILE_H
#define AUDIOFILE_H
#include "Player.h"
#include "Date.h"
#include <set>
#include <vector>
#include <string>
#include <sstream> 


class AudioFile
{
	friend std::ostream& operator<<(std::ostream& os, const AudioFile& audio);
protected:
	std::ostream& Print(std::ostream& os) const;
	std::string m_file_name;
	std::string m_file_path; 
	std::string m_duration; //m:ss format
	Date m_release_date;
	int m_id_code;
	Player m_player;
	AudioFile(std::string file_name, std::string file_path, std::string duration="", std::string release_date = "");
	//works
	AudioFile(int id, std::string file_name, std::string file_path, std::string duration="", std::string release_date = "");
public:

	virtual void Play();
	const std::string& get_name() const; // get the name of the song
	const std::string& get_path() const; // get the name of the song
	const int get_id() const;
	const Date& get_release_date() const;
	const std::string& get_duration() const;

	virtual void set_name(std::string& name);
	virtual void set_duration(std::string& duration);
	virtual void set_release_date(Date release_date);
};
std::ostream& operator<<(std::ostream& os, const AudioFile& audio);
#endif	// AUDIOFILE_H
