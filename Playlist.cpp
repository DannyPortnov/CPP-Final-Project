#include "Library.h"
#include "Playlist.h"



Playlist::Playlist(std::string name, Library* library, Server* server, bool restore_songs) : 
	m_playlist_name(name), m_library(library), m_server(server) {
	if (restore_songs) {
		restore_playlist(name);
	}
}

Playlist::~Playlist() {
	clear_all_playlist(false);
}


bool Playlist::check_if_continue_playing() {
	std::string prompt = "Would you like to continue playing " + m_playlist_name + "? y/n: ";
	std::string reject_message = "Stopped playing " + m_playlist_name;
	std::string accept_message = "Continue playing " + m_playlist_name; // or empty string "";
	if (Utilities::user_prompts_and_dialog(prompt, reject_message, accept_message)) {
		return true;
	}
	return false;
}

// play the songs in alphabetical order
void Playlist::Play(bool shuffle) {
	std::vector<Song*> songs_vector(m_songs.begin(), m_songs.end()); //range constructor
	m_library->PlayAll(&songs_vector,"Playing " + m_playlist_name + ":", shuffle, false);

	#pragma region Previous implementation
	//if (m_songs.size() == 0) {
//	std::cout << "There are no songs in the this playlist." << std::endl;
//	return;
//}
//std::cout <<  << std::endl;
//std::multiset<Song*>::iterator it;
//bool first_play = true;
//for (it = m_songs.begin(); it != m_songs.end(); it++) {
//	if (!first_play && !check_if_continue_playing()) { //asks after first play
//		return;
//	}
//	auto song_to_play = *it;
//	//std::cout << "Now playing: " << song_to_play << std::endl;
//	//song_to_play->update_plays_counter();
//	m_library->play_song(song_to_play); //todo: check if works
//	first_play = false;
//	//m_player.play((it->second)->get_path(), true); //todo: check if true is needed (not sure what is the purpose of wait)
//}  
#pragma endregion
}

//void Playlist::Play_Songs(const std::string& message, bool shuffle) {
//	if (m_songs.empty()) {
//		std::cout << "There are no songs in this playlist." << std::endl;
//		return;
//	}
//	std::cout << message << std::endl;
//	bool first_play = true;
//	std::vector<Song*> songs_vector (m_songs.begin(), m_songs.end()); //range constructor
//	if (shuffle) {
//		std::random_device rd;
//		std::mt19937 generator(rd());
//		std::shuffle(songs_vector.begin(), songs_vector.end(), generator);
//	}
//	for (const auto& it : songs_vector) {
//		if (!first_play && !check_if_continue_playing()) { //asks after first play
//			return;
//		}
//		//auto song_to_play = it;
//		m_library->play_song(it); //todo: check if works
//		first_play = false;
//	}
//	std::cout << "Finished playing all songs" << std::endl;
//}

//// play the songs randomly
//void Playlist::Play_Random() {
//
//	m_library->PlayAll(std::vector<Song*>(m_songs.begin(), m_songs.end()),
//		"Playing" + m_playlist_name + ", shuffled: ", true);
//	//Play_Songs("Playing" + m_playlist_name + ", shuffled: ", true);
//
//	#pragma region Previous implementation
//	//if (m_songs.size() == 0) {
//	//	std::cout << "There are no songs in the this playlist." << std::endl;
//	//	return;
//	//}
//	//// Create a vector of iterators to the elements in the multimap
//	//std::vector<std::multiset<Song*>::iterator> songs_vector;
//	//for (auto it = m_songs.begin(); it != m_songs.end(); ++it) {
//	//	songs_vector.push_back(it);
//	//}
//	//// Shuffle the keys of the multimap randomly
//	//std::random_device rd;
//	//std::mt19937 generator(rd());
//	//shuffle(songs_vector.begin(), songs_vector.end(), generator);
//	//std::cout << "Playing" << m_playlist_name << ", shuffled: " << std::endl;
//	//
//	//// Play the songs of the multimap in the shuffled order
//	//for (auto const& song : songs_vector) {
//	//	//std::cout << "Now playing: " << *(song) << std::endl;
//	//	//(*song)->update_plays_counter();
//	//	m_library->play_song(*song); //todo: check if works
//	//	//m_player.play((song->second)->get_path(), true); //todo: check if true is needed (not sure what is the purpose of wait)
//	//	if (check_if_continue_playing() == false)
//	//		return;
//	//}  
//	#pragma endregion
//
//	/*
//	 probably isn't needed, another implementation:
//	// Convert the set to a vector for shuffling
//	std::vector<string> songs_vector(m_songs.begin(), m_songs.end());
//	// Seed the random number generator with the current time
//	std::mt19937 generator(time(nullptr));
//	// Shuffle the vector
//	std::shuffle(songs_vector.begin(), songs_vector.end(), generator);
//	// Print the shuffled elements using an iterator
//	std::vector<Song*>::iterator it;
//	for (it = audio_files_vector.begin(); it != audio_files_vector.end(); it++) {
//		m_player.play((*it)->get_path(), true); //todo: check if true is needed (not sure what is the purpose of wait)
//	}
//	 todo: check if necessary
//	 Free the dynamically allocated objects
//	/.for (it = audio_files_vector.begin(); it != audio_files_vector.end(); it++) {
//		delete* it;
//	}
//	*/
//}



//returns the playlist
std::multiset<Song*> Playlist::get_songs() {
	return m_songs;
}

// add a song to the playlist. m_songs is a multimap
void Playlist::add_song_to_playlist(Song* song, bool prints_enabled) {
	if (m_songs.find(song) != m_songs.end()) {
		if (prints_enabled)
			std::cout << "This song is already in " << m_playlist_name << "!" << std::endl;
	}
	else {
		m_songs.insert(song);
		// m_songs_by_id.insert(make_pair(song->get_id(), song));
		song->set_playlist_appearances(m_playlist_name);
		if (prints_enabled) // if equals to false, don't print
			std::cout << "Song was successfully added to " << m_playlist_name << "!" << std::endl;
	}
}

// double checks with the user if the song should be deleted, if yes- returns true.
bool Playlist::make_sure_to_remove_song(Song* song, bool make_sure) {
	if (make_sure == false) {
		return true;
	}
	std::cout << "You chose to remove the song: " << std::endl;
	//std::cout << "The song details are:" << std::endl;
	std::cout << *song << std::endl;
	std::string prompt = "Are you sure you want to remove this song from: " + m_playlist_name + "? y/n: ";
	std::string reject_message = "The song wasn't removed from playlist: " + m_playlist_name + "!";
	std::string accept_message = "Song was successfully removed from playlist: " + m_playlist_name + "!";
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
void Playlist::remove_song_from_playlist(std::string song_name, bool make_sure) {
	auto song = get_song_by_name(song_name);
	remove_song_from_playlist(song, make_sure);
}

// remove all songs from the playlist.
void Playlist::clear_all_playlist(bool prints_enabled) { // favorites will also implement this (meaning,it empties the playlist)
	if (prints_enabled) { // check before clearing playlist prompt
		std::string prompt = "Are you sure that you want to remove: " + m_playlist_name + "? y/n: ";
		std::string reject_message = m_playlist_name + " wasn't removed!";
		std::string accept_message = m_playlist_name + " was successfully removed!";
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

void Playlist::save_playlist(std::string file_name, ios_base::openmode mode) {
	ofstream write_playlist("c:\\temp\\" + file_name + ".dat", mode);
	if (!Utilities::Is_File_Valid(write_playlist)) {
		return;
	}
	std::vector<std::string*> params = { &m_playlist_name };
	Utilities::Replace_All(params, false);
	for (auto& song : m_songs) {
		write_playlist << m_playlist_name << " " << song->get_id() << std::endl;
	}
}
void Playlist::restore_playlist(std::string file_name) {
	std::ifstream read_user_playlists("c:\\temp\\" + file_name + ".dat", ios::in);
	if (!Utilities::Is_File_Valid(read_user_playlists)) {
		return;
	}
	while (!Utilities::Is_End_Of_File_Or_Empty(read_user_playlists)) {
		std::string playlist_name;
		int song_id;
		read_user_playlists >> playlist_name >> song_id;
		std::vector<std::string*> params = { &playlist_name };
		Utilities::Replace_All(params, true);
		add_song_to_playlist(m_server->find_song_by_id(song_id), false);
		if (Utilities::Is_End_Of_File(read_user_playlists)) {
			break;
		}
	}
}

//returns true if the playlists name are in the right order.
bool operator<(const Playlist& a, const Playlist& b) {
	return (a.get_name() < b.get_name());
}


//todo: merge later with pick media
// get a specific song, even if there are few songs with the same name
Song* Playlist::get_song_by_name(std::string song_name)
{
	std::unordered_multimap<string, Song*> filtered_songs;
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
	std::unordered_multimap<std::string, Song*>::iterator iterator;
	for (iterator = filtered_songs.begin(); iterator != filtered_songs.end(); ++iterator, ++i) {//loops over all podcasts
		std::cout << i << " - " << iterator->second << std::endl;//prints numbered songs
	}
	int answer;
	do {
		std::cout << "Please choose a number between 1 and " << number_of_media_items << " (0 to cancel): ";
		cin >> answer;
	} while (answer < 0 || answer > number_of_media_items); //todo: check std::string input
	if (answer == 0) {
		throw exception();
	}
	iterator = filtered_songs.begin();
	advance(iterator, answer - 1);
	return iterator->second;
}


//print the playlist's content - songs in the playlist (organized alphabetically)
ostream& operator<<(ostream& os, const Playlist& playlist) {
	multiset<Song*>::iterator it;
	int i = 1;
	os << "List of songs in " << playlist.m_playlist_name << " playlist:" << std::endl;
	for (it = playlist.m_songs.begin(); it != playlist.m_songs.end(); it++) {
		os << "(" << i << "). " << **(it) << std::endl; // *it is *Song, so **it is Song!
		i++;
	}
	return os;
}

//void Playlist::Print() {
//	multiset<Song*>::iterator it;
//	int i = 1;
//	std::cout << "List of songs in " << m_playlist_name << " playlist:" << std::endl;
//	for (it = m_songs.begin(); it != m_songs.end(); it++) {
//		std::cout << "(" << i << "). " << *(it) << std::endl; // it->second contains Song*
//		i++;
//	}
//}





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
int Playlist::count_song_name_appearences(std::string song_name) {

	return m_songs.count(song_name);
}

//todo: maybe to chabge to pick media
// get a specific song, even if there are few songs with the same name
Song* Playlist::get_song_by_name(std::string song_name)
{
	int appearences_number = count_song_name_appearences(song_name); //O(log n), distance is O(n). overall doesn't matter
	if (appearences_number == 0) {
		return nullptr;
	}
	if (appearences_number == 1) {
		return m_songs.find(song_name)->second;
	}
	std::cout << "There is more than one song with this name: " << std::endl;
	unordered_map<int, Song*> same_name_songs; // unordered_map (hash_table) to store same name songs, search: O(1) in the avg case
	int i = 1;
	multimap<string, Song*>::iterator it;
	for (it = m_songs.begin(); it != m_songs.end(); it++) {
		if (it->first == song_name) {
			same_name_songs.insert(make_pair(it->second->get_id(), it->second)); // unordered_map key: id, value: song.
			std::cout << "(" << i << "). " << *(it->second) << std::endl; // it->second contains Song*
			i++;
		}
	}
	bool invalid_id = true;
	while (invalid_id) {
		int answer;
		std::cout << "Please Choose a song." << std::endl;
		std::cout << "Enter the id of the chosen song: ";
		cin >> answer;
		std::cout << std::endl;
		auto song_found = same_name_songs.find(answer);
		if (song_found != same_name_songs.end()) {
			return song_found->second;
		}
		std::cout << "Invalid id! try again." << std::endl;
	}
}
*/








//************************************************************************************************************************
//********************************************* Methods That May Not Be Needed********************************************
//************************************************************************************************************************ 



//// check if a song exist in the playlist by name, return true if exist
//bool Playlist::check_if_song_exist_in_playlist_by_name(const std::string& song_name) {
//	if (m_songs.find(song_name) != m_songs.end()) { // if not found, find method returns '.end()', comlexity: O(log(n))
//		return true;
//	}
//	return false;
//}
//
//// check if multiple songs in the playlist have the same name
//bool Playlist::check_if_songs_have_same_names(const std::string& song_name) {
//	if (m_songs.count(song_name) > 1) { 
//		return true;
//	}
//	return false;
//}


//Song* Playlist::get_song_by_name(std::string song_name) {
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


//std::unordered_multimap<string, Song*>* Playlist::get_songs_with_same_name(const std::string& song_name) {
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

