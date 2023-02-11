#include "Song.h"

Song::Song(string song_name, string file_path, string album,
	string singer, string genre, int release_date)
	: m_song_name(song_name), m_file_path(file_path), m_album(album),
	m_artist(singer), m_genre(genre), m_release_date(release_date)
{
	//to add duration calculation
}

void Song::Play()
{
	m_player.play(m_file_path);
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
		os << " was released in " << song.m_release_date;
	}
	return os;
}
