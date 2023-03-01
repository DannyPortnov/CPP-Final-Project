#include "Song.h"

int Song::song_id = 1;

Song::Song(std::string song_name, std::string file_path,
	std::string album, std::string singer, std::string genre, std::string release_date, std::string duration)
	: AudioFile(song_name, file_path, duration, release_date), m_album(album), m_artist(singer), m_genre(genre), m_plays_counter(0)
{
	//to add duration calculation
	m_id_code = song_id;
	song_id++;
}

Song::Song(int id, std::string song_name, std::string file_path, std::string album, std::string singer, 
	std::string genre, std::string release_date, std::string duration, int plays_count) :AudioFile(id, song_name, file_path, duration, release_date),
	m_album(album), m_artist(singer), m_genre(genre), m_plays_counter(plays_count)
{
	song_id++;
}



void Song::set_artist(std::string& artist) {
	m_artist = artist;
}
void Song::set_album(std::string& album) {
	m_album = album;
}
void Song::set_genre(std::string& genre) {
	m_genre = genre;
}

const std::string& Song::get_album() const
{
	return m_album;
}

const std::string& Song::get_genre() const
{
	return m_genre;
}

const std::string& Song::get_artist() const
{
	return m_artist;
}

const int Song::get_plays_count() const {
	return m_plays_counter;
}


void Song::print_playlists() const {
	std::cout << "Appears in playlists: ";
	if (m_playlist_appearances.size() == 0) {
		std::cout << " None." << std::endl;
		return;
	}
	std::unordered_set<std::string>::iterator it;
	for (it = m_playlist_appearances.begin(); it != m_playlist_appearances.end(); it++) {
		std::cout << *it;
		if (it!= --m_playlist_appearances.end()) { //don't print ',' after the last element
			std::cout << ", ";
		}
	}
	std::cout << std::endl;
}

void Song::Play()
{
	update_plays_counter();
	AudioFile::Play();
	//m_player.play(m_file_path);
	//the most_played, and most_recent update is made in Library and Server
}


// update when a song is played
void Song::update_plays_counter() {
	m_plays_counter += 1;
}

void Song::set_playlist_appearances(const std::string& playlist) {
	m_playlist_appearances.insert(playlist);
}

void Song::remove_from_playlist(const std::string& playlist) {
	m_playlist_appearances.erase(playlist);
}

// used when trying to delete a song in library
void Song::clear_from_all_playlists() {
	m_playlist_appearances.clear();
}

std::unordered_set<std::string>* Song::get_playlist_appearances() {
	return &m_playlist_appearances;
}

// returns true if the song names are in the right order.
bool operator<(const Song& a, const Song& b) {
	return (a.get_name() < b.get_name());

}


std::ostream& operator<<(std::ostream& os, const Song& song) //todo: make all prints in the same line (except for playlists, those are seperate line)
{
	os << static_cast<const AudioFile&>(song); // call AudioFile's operator<<
	if (song.m_artist != "") {
		os<< "Artist: " << song.m_artist << ", ";
	}
	if (song.m_album != "") {
		os<< "Album: " << song.m_album << ", ";
	}
	if (song.m_genre != "") {
		os << "Genre: " << song.m_genre << std::endl;
	}
	song.print_playlists();
	return os;
}
