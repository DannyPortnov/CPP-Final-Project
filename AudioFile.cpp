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
void AudioFile::set_duration(std::string& duration) //only allow m:ss ?
{
    int minutes, seconds;
    char delimiter;
    istringstream time_stream(duration);

    time_stream >> minutes;
    if (time_stream.fail() || time_stream.get() != ':') {
        throw exception();
    }

    time_stream >> seconds;
    if (time_stream.fail() || time_stream.get() != EOF || seconds >= 60) {
        throw exception();
    }
    m_duration = duration;
}

void AudioFile::set_release_date(Date release_date)
{
	m_release_date = release_date;
}
