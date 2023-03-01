#define   _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include "Library.h"
#include "Playlist.h"
#include "DailyMix.h"
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#define DailyMix_Name "DailyMix"

DailyMix::DailyMix(Library* library, Server* server) : Playlist(DailyMix_Name, library, server),
m_last_date_saved(get_date_from_file()), m_dailymix_file("c:\\temp\\DailyMix.dat", ios::in) 
{
	restore_playlist(); 
}

void DailyMix::restore_playlist() 
{
	Date new_date;
	if (check_if_date_changed(new_date)) {
		m_dailymix_file.close();
		m_last_date_saved = new_date;
		generate_daily_mix();
		return;
	} 

	while (!Utilities::Is_End_Of_File_Or_Empty(m_dailymix_file)) {
		int song_id;
		m_dailymix_file >> song_id;
		auto song = m_server->find_song_by_id(song_id);
		Playlist::add_song_to_playlist(song, false);
		if (Utilities::Is_End_Of_File(m_dailymix_file)) {
			m_dailymix_file.close();
			return;
		}
	}
}

//Reads the date that is in the file
Date& DailyMix::get_date_from_file() {
	Date default_date("");
	if (!Utilities::Is_File_Valid(m_dailymix_file, DailyMix_Name) || Utilities::Is_End_Of_File_Or_Empty(m_dailymix_file)) {
		return default_date;
	}
	std::string date;
	getline(m_dailymix_file, date);
	Date saved_date(date);
	return saved_date;
}

// checks if the date has changed. if yes: returns true, else: returns false.
bool DailyMix::check_if_date_changed(Date& new_date) {
	if (m_last_date_saved < new_date) {
		return true;
	}
	return false;
}


// deserialization for daily mix
void DailyMix::save_playlist(std::string file_name, ios_base::openmode mode) {

	ofstream write("c:\\temp\\" + m_playlist_name + ".dat", ios::out);
	if (!Utilities::Is_File_Valid(write, m_playlist_name)) {
		return;
	}
	// Write the std::string to the file first
	write << m_last_date_saved << std::endl;
	// Write the song data to the file
	multiset<Song*>::iterator itr;
	for (itr = m_songs.begin(); itr != m_songs.end(); itr++)
	{
		write << (*itr)->get_id() << std::endl;
	}
	write.close();
}

// generate a random mix of 10 songs from the library/server
void DailyMix::generate_daily_mix() {
	auto songs_to_shuffle = m_server->get_songs_by_id();
	if (songs_to_shuffle == nullptr) {
		return;
	}
	// Create a vector of iterators to the elements in the multimap
	vector<unordered_map<int, Song*>::iterator> shuffled_songs_vector;
	for (auto it = songs_to_shuffle->begin(); it != songs_to_shuffle->end(); ++it) {
		shuffled_songs_vector.push_back(it);
	}
	// Shuffle the keys of the multimap randomly
	shuffle(shuffled_songs_vector.begin(), shuffled_songs_vector.end(), Library::Get_Gen());

	//add 10 songs from shuffled_songs_vector to m_daily_mix 
	for (int i = 0; i < max_songs && i < shuffled_songs_vector.size(); i++) { // if library has less than 10 songs, mix them also.
		auto& song = shuffled_songs_vector[i];
		//int id = song->second->get_id();
		Playlist::add_song_to_playlist(song->second, false);
	}
	shuffled_songs_vector.clear();
}


