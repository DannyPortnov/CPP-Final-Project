#include "Playlist.h"
#include "Library.cpp"



Playlist::Playlist(string name, Library* library) : m_playlist_name(name), m_library(library) {}

Playlist::~Playlist() {
	clear_all_playlist(false);
}


bool Playlist::check_if_continue_playing() {
	string prompt = "Would you like to continue playing " + m_playlist_name + "? y/n: ";
	string reject_message = "Stopped playing " + m_playlist_name;
	string accept_message = "Continue playing " + m_playlist_name; // or empty string "";
	if (Utilities::user_prompts_and_dialog(prompt, reject_message, accept_message)) {
		return true;
	}
	return false;
}

// play the songs in alphabetical order
void Playlist::Play() {
	if (m_songs.size() == 0) {
		cout << "There are no songs in the this playlist." << endl;
		return;
	}
	cout << "Playing" << m_playlist_name << ":" << endl;
	multiset<Song*>::iterator it;
	for (it = m_songs.begin(); it != m_songs.end(); it++) {
		auto song_to_play = *it;
		//cout << "Now playing: " << song_to_play << endl;
		//song_to_play->update_plays_counter();
		m_library->play_song(song_to_play); //todo: check if works
		//m_player.play((it->second)->get_path(), true); //todo: check if true is needed (not sure what is the purpose of wait)
		if (check_if_continue_playing() == false)
			return;
	}
}

// play the songs randomly
void Playlist::Play_Random() {
	// Create a vector of iterators to the elements in the multimap
	vector<multiset<Song*>::iterator> songs_vector;
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
		//cout << "Now playing: " << *(song) << endl;
		//(*song)->update_plays_counter();
		m_library->play_song(*song); //todo: check if works
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
	multiset<Song*>::iterator it;
	int i = 1;
	cout << "List of songs in " << m_playlist_name << " playlist:" << endl;
	for (it = m_songs.begin(); it != m_songs.end(); it++) {
		cout << "(" << i << "). " << *(it) << endl; // it->second contains Song*
		i++;
	}
}

//returns the playlist
multiset<Song*> Playlist::get_songs() {
	return m_songs;
}

// add a song to the playlist. m_songs is a multimap
void Playlist::add_song_to_playlist(Song* song, bool add_print) {
	if (m_songs.find(song) != m_songs.end()) {
		if (add_print)
			cout << "This song is already in " << m_playlist_name << "!" << endl;
	}
	else {
		m_songs.insert(song);
		// m_songs_by_id.insert(make_pair(song->get_id(), song));
		song->set_playlist_appearances(m_playlist_name);
		if (add_print) // if equals to false, don't print
			cout << "Song was successfully added to " << m_playlist_name << "!" << endl;
	}
}

// double checks with the user if the song should be deleted, if yes- returns true.
bool Playlist::make_sure_to_remove_song(Song* song, bool make_sure) {
	cout << "You chose to remove the song: " << endl;
	//cout << "The song details are:" << endl;
	cout << *song << endl;
	if (make_sure == false) {
		return true;
	}
	string prompt = "Are you sure you want to remove this song from: " + m_playlist_name + "? y/n: ";
	string reject_message = "The song wasn't removed from playlist: " + m_playlist_name + "!";
	string accept_message = "Song was successfully removed from playlist: " + m_playlist_name + "!";
	if (Utilities::user_prompts_and_dialog(prompt, reject_message, accept_message)) {
		return true;
	}
	return false;
}

// removes a song from playlist
// if doesn't need to make_sure, the song will be deleted (returns true)
void Playlist::remove_song_from_playlist(Song* song, bool make_sure) {
	if (make_sure_to_remove_song(song, make_sure)) {
		// need to pick media or to remove by id
		m_songs.erase(song);//erase by object type
		//m_songs_by_id.erase(song->get_id());// erase by id

		song->remove_from_playlist(m_playlist_name);
	}
	// the printing of the results of the operation happans in make_sure_to_remove
}

//todo: check if overriden properly
void Playlist::remove_song_from_playlist(string song_name, bool make_sure) {
	auto song = get_song_by_name(song_name);
	remove_song_from_playlist(song, make_sure);
}

// remove all songs from the playlist.
void Playlist::clear_all_playlist(bool add_print) { // favorites will also implement this (meaning,it empties the playlist)
	if (add_print) { // check before clearing playlist prompt
		string prompt = "Are you sure that you want to remove: " + m_playlist_name + "? y/n: ";
		string reject_message = m_playlist_name + " wasn't removed!";
		string accept_message = m_playlist_name + " was successfully removed!";
		if (Utilities::user_prompts_and_dialog(prompt, reject_message, accept_message) == false) {
			return;
		}
	}
	multiset<Song*>::iterator it; // go over all songs in the playlist
	for (it = m_songs.begin(); it != m_songs.end(); it++) {
		(*it)->remove_from_playlist(m_playlist_name);// removes the playlist name from Song's m_playlist_appearences
	}
	m_songs.clear(); // remove all songs from the playlist itself
	//m_songs_by_id.clear();
}

void Playlist::save_playlist() {
	ofstream write_playlist("c:\\temp\\playlists.dat", ios::in);
	if (!Utilities::Is_File_Valid(write_playlist)) {
		return;
	}
	vector<string*> params = { &m_playlist_name };
	Utilities::Replace_All(params, false);
	for (auto& song : m_songs) {
		write_playlist << " " << m_playlist_name << " " << song->get_id() << endl;
	}
}
//returns true if the playlists name are in the right order.
bool operator<(const Playlist& a, const Playlist& b) {
	return (a.get_name() < b.get_name());
}

//todo: merge later with pick media
// get a specific song, even if there are few songs with the same name
Song* Playlist::get_song_by_name(string song_name)
{
	unordered_multimap<string, Song*> filtered_songs;
	for (auto& song : m_songs) {
		filtered_songs.emplace(song->get_name(), song);
	}
	int number_of_media_items = filtered_songs.count(song_name); //O(log n), distance is O(n). overall doesn't matter
	if (number_of_media_items == 0) {
		return nullptr;
	}
	if (number_of_media_items == 1) {
		return filtered_songs.find(song_name)->second;
	}
	int i = 1;//can't define 2 variables of different type in for (you can but it's less readable)
	unordered_multimap<string, Song*>::iterator iterator;
	for (iterator = filtered_songs.begin(); iterator != filtered_songs.end(); ++iterator, ++i) {//loops over all podcasts
		cout << i << " - " << iterator->second << endl;//prints numbered songs
	}
	int answer;
	do {
		cout << "Please choose a number between 1 and " << number_of_media_items << " (0 to cancel): ";
		cin >> answer;
	} while (answer < 0 || answer > number_of_media_items); //todo: check string input
	if (answer == 0) {
		throw exception();
	}
	iterator = filtered_songs.begin();
	advance(iterator, answer - 1);
	return iterator->second;
}


// check if a song exist in the playlist by id, return true if exist
//bool Playlist::check_if_song_exist_in_playlist_by_id(int id) {
//	auto song = Server::find_song_by_id(id);
//	if (m_songs_by_id.find(song->get_id()) != m_songs_by_id.end()) { //if not found, find method returns '.end()', comlexity: O(log(n))
//		return true;
//	}
//	return false;
//}

//todo: use pick media instea of get_ong_by_name and count_song_name_appearences
// get how many songs have the same name
/*
int Playlist::count_song_name_appearences(string song_name) {

	return m_songs.count(song_name);
}

//todo: maybe to chabge to pick media
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
*/








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

