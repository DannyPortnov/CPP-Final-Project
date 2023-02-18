#include "Song.h"

Song::Song(string song_name, string file_path,
	string album,string singer, string genre, int release_date, string duration)
	: AudioFile(song_name, file_path, duration), m_album(album), m_artist(singer), m_genre(genre)
{
	//to add duration calculation
}


const string& Song::get_album() const
{
	return m_album;
}

const string& Song::get_genre() const
{
	return m_genre;
}

const string& Song::get_artist() const
{
	return m_artist;
}

void Song::Play()
{
	m_player.play(m_file_path);
	//todo: add the song to Recent 
}

// returns true if the song names are in the right order.
bool operator<(const Song& a, const Song& b) {
	return (a.get_name() < b.get_name());

}


ostream& operator<<(ostream& os, const Song& song)
{
	os << song.m_file_name;
	if (song.m_album != "") {
		os<< ", from " << song.m_album;
	}
	if (song.m_artist != "") {
		os<< ", by " << song.m_artist;
	}
	if (song.m_genre != "") {
		os << " " << song.m_genre << "genre";
	}
	if (song.m_release_date != 0) {
		os << " release date: " << song.m_release_date;
	}
	return os;
}
