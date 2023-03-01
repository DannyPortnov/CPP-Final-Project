#include "AudioFile.h"


//int AudioFile::id_code = 1;

AudioFile::AudioFile(std::string file_name, std::string file_path, std::string duration, std::string release_date)
	: m_file_name(file_name), m_file_path(file_path), m_release_date(release_date)
{
	//m_id_code = id_code;
	//id_code++;
	set_duration(duration);
}

AudioFile::AudioFile(int id, std::string file_name, std::string file_path, std::string duration, std::string release_date)
	: m_id_code(id), m_file_name(file_name), m_file_path(file_path), m_release_date(release_date)
{
	set_duration(duration);
}

void AudioFile::Play()
{
	//todo: see that this works
	m_player.play(m_file_path);
	std::cout << "Now playing: " << m_file_name << std::endl;
}

const std::string& AudioFile::get_name() const {
	return m_file_name;
}

const std::string& AudioFile::get_path() const {
	return m_file_path;
}

const int AudioFile::get_id() const {
	return m_id_code;
}

const Date& AudioFile::get_release_date() const
{
	return m_release_date;
}

const std::string& AudioFile::get_duration() const
{
	return m_duration;
}

void AudioFile::set_name(std::string& name)
{
	m_file_name = name;
}

//works
void AudioFile::set_duration(std::string& duration) //only allows mm:ss or m:ss
{
	do {
		if (duration.empty()) {
			break;
		}
		std::regex validTime("^([0-5][0-9]):([0-5][0-9])$");
		if (std::regex_match(duration, validTime)) {
			break;
		}
		std::cout << "Duration is invalid, please enter again in the format mm:ss (press enter to cancel)" << std::endl;
		std::cin >> duration;
	} while (true);
	if (duration == "00:00") { //same as no duration
		duration = "";
	}
	m_duration = duration;
}

void AudioFile::set_release_date(Date release_date)
{
	m_release_date = release_date;
}

std::ostream& operator<<(std::ostream& os, const AudioFile& audio)
{
	os << "ID code: " << audio.m_id_code << ", ";
	os << "Name: " << audio.m_file_name;
	if (audio.m_release_date != Date("")) {
		os << ", Release date: " << audio.m_release_date;
	}
	if (!audio.m_duration.empty()) {
		os << ", Duration: " << audio.m_duration;
	}
	//song.print_playlists();
	return os;
}

//std::ostream& AudioFile::Print(std::ostream& os) const
//{
//	os << m_file_name;
//	if (!(audio.m_release_date == Date(""))) {
//		os << " release date: " << audio.m_release_date;
//	}
//	//song.print_playlists();
//	return os;
//}
