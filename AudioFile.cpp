#include "AudioFile.h"

//Song and episode have seperate ID number
AudioFile::AudioFile(std::string file_name, std::string file_path, std::string duration, std::string release_date)
	: m_file_name(file_name), m_file_path(file_path), m_release_date(release_date)
{
	set_duration(duration);
}

AudioFile::AudioFile(int id, std::string file_name, std::string file_path, std::string duration, std::string release_date)
	: m_id_code(id), m_file_name(file_name), m_file_path(file_path), m_release_date(release_date)
{
	set_duration(duration);
}

void AudioFile::Play()
{
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

// This function sets the duration of an audio file.
// The duration must be in the format mm:ss.
// If the duration is not in the correct format, the user is prompted to enter a new duration.
// If the duration is "00:00", the duration is set to an empty string.
void AudioFile::set_duration(std::string& duration) //only allows mm:ss
{
	do {
		if (duration.empty()) { // If the duration is empty, exit the loop
			break;
		}
		// Use a regular expression to validate the format of the duration
		std::regex validTime("^([0-5][0-9]):([0-5][0-9])$");
		if (std::regex_match(duration, validTime)) {
			break;
		}
		// If the duration is not in the correct format, prompt the user to enter a new duration
		std::cout << "Duration is invalid, please enter again in the format mm:ss (press enter to cancel)" << std::endl;
		std::cin >> duration;
	} while (true);
	if (duration == "00:00") { //same as no duration
		duration = "";
	}
	// Set the duration of the audio file
	m_duration = duration;
}

// This function sets the release date of an audio file.
// If the release date is not in the correct format, the user is prompted to enter a new release date.
// If he cancels the operation, he can either erase the date or keep it.
void AudioFile::set_release_date(string& release_date)
{
	// Create a Date object from the release date string
	auto date = Date(release_date);
	// If the Date object is invalid, prompt the user to 
	//keep the previous release date or enter a new release date
	std::string answer;
	if (date == Date("")) {
		std::cout << "Do you want to erase previous release_date? If so enter y, anything else cancels the operation" << std::endl;
		std::cin >> answer;
		// If the user enters "y", erase the previous release date
		if (answer == "y") {
			m_release_date = Date("");
		}
		return;
	}
	// Set the release date of the audio file
	m_release_date = date;
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
	return os;
}

