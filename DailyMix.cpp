#include "DailyMix.h"

// after initiallizing the system, if the date did not change: the same mix that was first created in that date
// needs to be presented to the user.
DailyMix::DailyMix() : m_dailymix_file("c:\\temp\\daily_mix.dat", ios::in), m_last_date_saved(get_date_from_file()) {
	
	//m_last_date_saved = saved_date;
	Date new_date;
	// check if date has changed. if so, generate a new daily mix
	if (check_if_date_changed(new_date)) {
		m_last_date_saved = new_date;
		generate_daily_mix();
		return;
	} //Untill here works
	// if day hasn't changed, reload dailymix data back in to the data structure.
	while (!m_dailymix_file.eof()) {
		int song_id;
		m_dailymix_file >> song_id;
		auto song = Server::find_song_by_id(song_id);
		m_daily_mix.emplace(song_id, song);
		if (Utilities::Is_End_Of_File(m_dailymix_file)) {
			return;
		}
	}
}
//Works
Date& DailyMix::get_date_from_file() {
	Date default_date("");
	if (!Utilities::Is_File_Valid(m_dailymix_file)) {
		return default_date;
	}
	string date;
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
// removes a song from the mix if it was deleted from the libarary.
// the mix stays with less songs until the next day
//todo: maybe add a feature that lets the user to remix the playlist after removing a song.
void DailyMix::remove_song_from_mix(int id) {
	m_daily_mix.erase(id);
}


// deserialization for daily mix
void DailyMix::save_dailymix() {

	fstream write("c:\\temp\\daily_mix.dat", ios::out);
	if (!write) {
		cout << "Couldn't open file for serialization" << endl;
		return;
	}
	// Write the string to the file first
	write << m_last_date_saved << endl;
	// Write the song data to the file
	unordered_map<int, Song*>::iterator itr;
	for (itr = m_daily_mix.begin(); itr != m_daily_mix.end(); itr++)
	{
		write << itr->first << endl;
	}
}

// generate a random mix of 10 songs from the library/server
//todo: maybe add a feature to let the user to remix the daily mix.
void DailyMix::generate_daily_mix(){
	auto songs_to_shuffle = Server::get_songs_by_id();
	if (songs_to_shuffle == nullptr) { //todo: dailymix will be empty
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
	int i = 1;
	for (int i = 0; i < max_songs && i < shuffled_songs_vector.size(); i++) { // if library has less than 10 songs, mix them also.
		auto song = shuffled_songs_vector[i];
		int id = song->second->get_id();
		m_daily_mix.insert(make_pair(id, song->second));
	}
}


