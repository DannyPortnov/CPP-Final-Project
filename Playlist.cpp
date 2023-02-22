#include "Playlist.h"
#include "Library.h"



Playlist::Playlist(string name) : m_playlist_name(name) {}

Playlist::~Playlist() {
	clear_all_playlist();
}


bool Playlist::check_if_continue_playing() {
	bool invalid_char = true;
	while (invalid_char) {
		cout << "Would you like to continue playing " << m_playlist_name << "? y/n: ";
		char answer;
		cin >> answer; cout << endl;
		if (answer == 'n') {
			cout << "Stopped playing " << m_playlist_name << endl;
			invalid_char = false;
			return false;
		}
		else if (answer == 'y') {
			cout << "Continue playing " << m_playlist_name << endl;
			invalid_char = false;
			return true;
		}
		else {
			cout << "Invalid character entered, try again: " << endl;
		}
	}
}

// play the songs in alphabetical order
void Playlist::Play() {
	if (m_songs.size() == 0) {
		cout << "There are no songs in the this playlist." << endl;
		return;
	}
	cout << "Playing" << m_playlist_name << ":" << endl;
	multimap<string, Song*>::iterator it;
	for (it = m_songs.begin(); it != m_songs.end(); it++) {
		cout << "Now playing: " << *it->second << endl;
		it->second->update_plays_counter();
		Library::play_song(it->second); //todo: check if works
		//m_player.play((it->second)->get_path(), true); //todo: check if true is needed (not sure what is the purpose of wait)
		if (check_if_continue_playing() == false)
			return;
	}
}

// play the songs randomly
void Playlist::Play_Random() {
	// Create a vector of iterators to the elements in the multimap
	vector<multimap<string, Song*>::iterator> songs_vector;
	for (auto it = m_songs.begin(); it != m_songs.end(); ++it) {
		songs_vector.push_back(it);
	}

	// Shuffle the keys of the multimap randomly
	random_device rd;
	mt19937 generator(rd());
	shuffle(songs_vector.begin(), songs_vector.end(), generator);

	cout << "Playing" << m_playlist_name << ", shuffled: " << endl;
	
	// Play the songs of the multimap in the shuffled order
	for (auto const& song : songs_vector) {
		cout << "Now playing: " << *(song->second) << endl;
		song->second->update_plays_counter();
		Library::play_song(song->second); //todo: check if works
		//m_player.play((song->second)->get_path(), true); //todo: check if true is needed (not sure what is the purpose of wait)
		if (check_if_continue_playing() == false)
			return;
	}
	/*
	 probably don't needed, another implementation:
	// Convert the set to a vector for shuffling
	std::vector<string> songs_vector(m_songs.begin(), m_songs.end());
	// Seed the random number generator with the current time
	std::mt19937 generator(time(nullptr));
	// Shuffle the vector
	std::shuffle(songs_vector.begin(), songs_vector.end(), generator);
	// Print the shuffled elements using an iterator
	std::vector<Song*>::iterator it;
	for (it = audio_files_vector.begin(); it != audio_files_vector.end(); it++) {
		m_player.play((*it)->get_path(), true); //todo: check if true is needed (not sure what is the purpose of wait)
	}
	 todo: check if necessary
	 Free the dynamically allocated objects
	/.for (it = audio_files_vector.begin(); it != audio_files_vector.end(); it++) {
		delete* it;
	}
	*/
}


//print the playlist's content - songs in the playlist (organized alphabetically)
void Playlist::Print() {
	multimap<string, Song*>::iterator it;
	int i = 1;
	cout << "List of songs in " << m_playlist_name << " playlist:" << endl;
	for (it = m_songs.begin(); it != m_songs.end(); it++) {
		cout << "(" << i << "). " << *(it->second) << endl; // it->second contains Song*
		i++;
	}
}

//returns the playlist
multimap<string, Song*> Playlist::get_songs() {
	return m_songs;
}

// add a song to the playlist. m_songs is a multimap
void Playlist::add_song_to_playlist(Song* song) {
	if (check_if_song_exist_in_playlist_by_id(song->get_id())) {
		cout << "This song is already in the playlist!" << endl;
	}
	else {
		m_songs.insert(make_pair(song->get_name(), song));
		song->set_playlist_appearances(m_playlist_name);
		if (m_playlist_name != "deleted")
			cout << "Song was successfully added to " << m_playlist_name << "!" << endl;
	}
}

// double checks with the user if the song should be deleted, if yes- returns true.
bool Playlist::make_sure_to_remove_song(Song* song, bool make_sure) {
	cout << "You chose to remove the song: " << endl;
	cout << "The song details are:" << endl;
	cout << *song << endl;
	if (make_sure == false) {
		return true;
	}
	bool invalid_char = true;
	while (invalid_char) {
		cout << "Are you sure you want to remove this song from: " << m_playlist_name << "?" << endl;
		cout << "Press y/n: ";
		char answer;
		cin >> answer;
		cout << endl;
		if (answer == 'y') {
			return true;
		}
		if (answer == 'n') {
			return false;
		}
		cout << "Invalid answer! try again." << endl;
	}
}

// removes a song from playlist
// if doesn't need to make_sure, the song will be deleted (returns true)
void Playlist::remove_song_from_playlist(Song* song, bool make_sure ) {
	if (make_sure_to_remove_song(song, make_sure)) {
		m_songs.erase(song->get_name());
		song->remove_from_playlist(m_playlist_name);
		cout << "Song was successfully removed from playlist: " << m_playlist_name << "!" << endl;
	}
	cout << "The song wasn't removed from playlist: " << m_playlist_name << "!" << endl;

}


// remove all songs from the playlist.
void Playlist::clear_all_playlist() {
	multimap<string, Song*>::iterator it; // go over all songs in the playlist
	for (it = m_songs.begin(); it != m_songs.end(); it++) {
		it->second->remove_from_playlist(m_playlist_name);// removes the playlist name from Song's m_playlist_appearences
	}
	m_songs.clear(); // remove all songs from the playlist itself
}

// check if a song exist in the playlist by id, return true if exist
bool Playlist::check_if_song_exist_in_playlist_by_id(int id) {
	auto song = Server::find_song_by_id(id);
	if (m_songs.find(song->get_name()) != m_songs.end()) { //if not found, find method returns '.end()', comlexity: O(log(n))
		return true;
	}
	return false;
}

// get how many songs have the same name
int Playlist::count_song_name_appearences(string song_name) {
	return m_songs.count(song_name);
}


// get a specific song, even if there are few songs with the same name
Song* Playlist::get_song_by_name(string song_name)
{
	int appearences_number = count_song_name_appearences(song_name); //O(log n), distance is O(n). overall doesn't matter
	if (appearences_number == 0) {
		return nullptr;
	}
	if (appearences_number == 1) {
		return m_songs.find(song_name)->second;
	}
	cout << "There is more than one song with this name: " << endl;
	unordered_map<int, Song*> same_name_songs; // unordered_map (hash_table) to store same name songs, search: O(1) in the avg case
	int i = 1;
	multimap<string, Song*>::iterator it;
	for (it = m_songs.begin(); it != m_songs.end(); it++) {
		if (it->first == song_name) {
			same_name_songs.insert(make_pair(it->second->get_id(), it->second)); // unordered_map key: id, value: song.
			cout << "(" << i << "). " << *(it->second) << endl; // it->second contains Song*
			i++;
		}
	}
	bool invalid_id = true;
	while (invalid_id) {
		int answer;
		cout << "Please Choose a song." << endl;
		cout << "Enter the id of the chosen song: ";
		cin >> answer;
		cout << endl;
		auto song_found = same_name_songs.find(answer);
		if (song_found != same_name_songs.end()) {
			return song_found->second;
		}
		cout << "Invalid id! try again." << endl;
	}
}

//returns true if the playlists name are in the right order.
bool operator<(const Playlist& a, const Playlist& b) {
	return (a.get_name() < b.get_name());
}







//************************************************************************************************************************
//********************************************* Methods That May Not Be Needed********************************************
//************************************************************************************************************************ 



//// check if a song exist in the playlist by name, return true if exist
//bool Playlist::check_if_song_exist_in_playlist_by_name(const string& song_name) {
//	if (m_songs.find(song_name) != m_songs.end()) { // if not found, find method returns '.end()', comlexity: O(log(n))
//		return true;
//	}
//	return false;
//}
//
//// check if multiple songs in the playlist have the same name
//bool Playlist::check_if_songs_have_same_names(const string& song_name) {
//	if (m_songs.count(song_name) > 1) { 
//		return true;
//	}
//	return false;
//}


//Song* Playlist::get_song_by_name(string song_name) {
//
//	if (check_if_song_exist_in_playlist_by_name(song_name)) {
//		if (check_if_songs_have_same_names(song_name) == false) {
//			return m_songs.find(song_name)->second; // return the song that was found
//		}
//	}
//	else {
//		return nullptr;
//	}
//}


//unordered_multimap<string, Song*>* Playlist::get_songs_with_same_name(const string& song_name) {
//	unordered_multimap<string, Song*>* same_name_songs = new unordered_multimap<string, Song*>; //todo: check memroy allocation, check if we need to delete.
//	if (check_if_songs_have_same_names(song_name)) {
//		multimap<string, Song*>::iterator it;
//		for (it = m_songs.begin(); it != m_songs.end(); ++it) {
//			if (it->first == song_name) {
//				same_name_songs->insert(make_pair(it->first, it->second));
//			}
//		}
//	}
//	return same_name_songs; // delete after finishing using it!
//}



////compare playlists.
//bool operator!=(const Playlist& a, const Playlist& b) {
//	return (a.get_name() != b.get_name());
//}

////returns true if the playlists name are in the right order.
//bool operator<(Playlist* a, Playlist* b) {
//	return ((*a).get_name() < (*b).get_name());
//}

