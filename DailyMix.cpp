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

// after initiallizing the system, if the date did not change: the same mix that was first created in that date
// needs to be presented to the user.
//DailyMix::DailyMix() : m_dailymix_file("c:\\temp\\daily_mix.dat", ios::in), m_last_date_saved(get_date_from_file()) {
//	
//	
//}
#define DailyMix_Name "DailyMix"

DailyMix::DailyMix(Library* library, Server* server) : Playlist(DailyMix_Name, library, server),
	m_last_date_saved(get_date_from_file()), m_dailymix_file("c:\\temp\\DailyMix.dat", ios::in)
{
	restore_playlist();
}

void DailyMix::restore_playlist() //todo: make maybe another parent class
{
	//m_last_date_saved = saved_date;
	Date new_date;
	// check if date has changed. if so, generate a new daily mix
	if (check_if_date_changed(new_date)) {
		m_dailymix_file.close();
		m_last_date_saved = new_date;
		generate_daily_mix();
		return;
	} //Untill here works
	// if day hasn't changed, reload dailymix data back in to the data structure.

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

//Works
Date& DailyMix::get_date_from_file() {
	Date default_date("");
	if (!Utilities::Is_File_Valid(m_dailymix_file) || Utilities::Is_End_Of_File_Or_Empty(m_dailymix_file)) {
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


//todo: need to delete a song from here when we delete from library.

// removes a song from the mix if it was permanently deleted from the libarary.
// add a feature that lets the user to chose if he wants to remix the playlist after removing a song,
// or to leave the mix with less songs until the next day
void DailyMix::remove_song_from_playlist(Song* song, bool make_sure) {

	std::cout << "A song was deleted from " << m_playlist_name << "." << std::endl;
	std::string prompt = "Would you like to regenerate " + m_playlist_name + "? y/n: ";
	std::string reject_message = m_playlist_name + " wasn't regenerated!";
	std::string accept_message = m_playlist_name + " was regenerated successfully!";
	if (Utilities::user_prompts_and_dialog(prompt, reject_message, accept_message)) {
		Playlist::clear_all_playlist(false); // don't add prints
		generate_daily_mix();
		return;
	}
	Playlist::remove_song_from_playlist(song, false);
	return;
}


// deserialization for daily mix
void DailyMix::save_playlist(std::string file_name, ios_base::openmode mode) {

	ofstream write("c:\\temp\\" + m_playlist_name + ".dat", ios::out);
	if (!Utilities::Is_File_Valid(write)) {
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
//todo: maybe add a feature to let the user to remix the daily mix.
void DailyMix::generate_daily_mix(){
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
	std::random_device rd;
	std::mt19937 generator(rd());
	shuffle(shuffled_songs_vector.begin(), shuffled_songs_vector.end(), generator);

	//add 10 songs from shuffled_songs_vector to m_daily_mix 
	for (int i = 0; i < max_songs && i < shuffled_songs_vector.size(); i++) { // if library has less than 10 songs, mix them also.
		auto& song = shuffled_songs_vector[i];
		//int id = song->second->get_id();
		Playlist::add_song_to_playlist(song->second, false);
	}
	shuffled_songs_vector.clear();
}


