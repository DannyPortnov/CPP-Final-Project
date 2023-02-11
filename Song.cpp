#include "Song.h"

Song::Song(string song_name, string file_path, string album,
	string singer, string genre, int release_date)
	: m_song_name(song_name), m_file_path(file_path), m_album(album),
	m_artist(singer), m_genre(genre), m_release_date(release_date)
{
	//to add duration calculation
}

const string& Song::get_name() const { // get the name of the song
	return m_song_name;
}

// get the path of the song
const string& Song::get_path() const { 
	return m_file_path;
}

// get the id of the song
const int Song::get_id() const { 
	return m_id_code;
}

void Song::Play()
{
	m_player.play(m_file_path);
}

// returns true if the song names are in the right order.
bool operator<(const Song& a, const Song& b) {
	return (a.get_name() < b.get_name());

}


ostream& operator<<(ostream& os, const Song& song)
{
	os << song.m_song_name;
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
