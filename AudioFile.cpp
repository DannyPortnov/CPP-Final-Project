#include "AudioFile.h"

int AudioFile::id_code = 1;

AudioFile::AudioFile(string file_name, string file_path, string duration, int release_date)
	: m_file_name(file_name), m_file_path(file_path), m_duration(duration), m_release_date(release_date)
{
	m_id_code = id_code;
	id_code++;
}

const string& AudioFile::get_name() const {
	return m_file_name;
}

const string& AudioFile::get_path() const {
	return m_file_path;
}

const int AudioFile::get_id() const {
	return m_id_code;
}

void AudioFile::set_name(string& name)
{
	m_file_name = name;
}

void AudioFile::set_duration(string& duration)
{
	//todo: add check for format m:ss
}

void AudioFile::set_release_date(int release_date)
{
	m_release_date = release_date;
}
