#include "Song.h"

Song::Song(string song_name, string file_path, string album,
	string singer, string genre, int release_date)
	: m_song_name(song_name), m_file_path(file_path), m_album(album),
	m_singer(singer), m_genre(genre), m_release_date(release_date)
{
	//to add duration calculation
}
