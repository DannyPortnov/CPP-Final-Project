#include "Song.h"

Song::Song(string song_name, string file_path,
	string album,string singer, string genre, int release_date, string duration)
	: AudioFile(song_name, file_path, duration), m_album(album), m_artist(singer), m_genre(genre), m_plays_counter(0)
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

const int Song::get_plays_count() const {
	return m_plays_counter;
}


void Song::Play()
{
	update_plays_counter();
	m_player.play(m_file_path);
	//the most_played, and most_recent update is made in Library and Server
}

// update when a song is played
void Song::update_plays_counter() {
	m_plays_counter += 1;
}

void Song::set_playlist_appearences(const string& playlist) {
	m_playlist_appearences.insert(playlist);
}

void Song::remove_from_playlist(const string& playlist) {
	m_playlist_appearences.erase(playlist);
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
	if(song.)
	return os;
}
