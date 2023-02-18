#include "Playlist.h"



Playlist::Playlist(string name) : m_playlist_name(name) {}

Playlist::~Playlist() {
	clear_all_playlist();
}

// play the songs in alphabetical order
void Playlist::Play() {
	multimap<string, Song*>::iterator it;
	for (it = m_songs.begin(); it != m_songs.end(); it++) {
		m_player.play((it->second)->get_path(), true); //todo: check if true is needed (not sure what is the purpose of wait)
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

	// Play the songs of the multimap in the shuffled order
	for (auto const& song : songs_vector) {
		m_player.play((song->second)->get_path(), true); //todo: check if true is needed (not sure what is the purpose of wait)
	}

	// probably don't needed, another implementation:

	//// Convert the set to a vector for shuffling
	//std::vector<string> songs_vector(m_songs.begin(), m_songs.end());

	//// Seed the random number generator with the current time
	//std::mt19937 generator(time(nullptr));

	//// Shuffle the vector
	//std::shuffle(songs_vector.begin(), songs_vector.end(), generator);

	//// Print the shuffled elements using an iterator
	//std::vector<Song*>::iterator it;
	//for (it = audio_files_vector.begin(); it != audio_files_vector.end(); it++) {
	//	m_player.play((*it)->get_path(), true); //todo: check if true is needed (not sure what is the purpose of wait)
	//}

	// todo: check if necessary
	// Free the dynamically allocated objects
	/*for (it = audio_files_vector.begin(); it != audio_files_vector.end(); it++) {
		delete* it;
	}*/

}


//print the playlist's content - songs in the playlist (organized alphabetically)
void Playlist::Print() {
	multimap<string, Song*>::iterator it;
	int i = 1;
	cout << "List Of Songs In " << m_playlist_name << " Playlist:" << endl;
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
	m_songs.insert(make_pair(song->get_name(), song));
}

// remove a song from the playlist. m_songs is a multimap
void Playlist::remove_song_from_playlist(Song* song) {
	m_songs.erase(song->get_name());
}

// remove all songs from the playlist. m_songs is a multimap
void Playlist::clear_all_playlist() {
	m_songs.clear();
}

// check if a song exist in the playlist
bool Playlist::check_if_song_exist_in_playlist(const string& song_name) {
	if (m_songs.find(song_name) != m_songs.end()) { // if not found, find method returns '.end()' element
		return true;
	}
	return false;
}

// check if multiple songs in the playlist have the same name
bool Playlist::check_if_songs_have_same_names(const string& song_name) {
	if (m_songs.count(song_name) > 1) { 
		return true;
	}
	return false;
}


Song* Playlist::get_song_by_name(string song_name) {
	if (check_if_songs_have_same_names(song_name) == false) {
		
	}
}

unordered_multimap<string, Song*>* Playlist::get_songs_with_same_name(const string& song_name) {
	if (check_if_songs_have_same_names(song_name)) {
		unordered_multimap<string, Song*>* same_name_songs = new unordered_multimap<string, Song*>; //todo: check memroy allocation, check if we need to delete.
		multimap<string, Song*>::iterator it;
		for (it = m_songs.begin(); it != m_songs.end(); ++it) {
			if (it->first == song_name) {
				same_name_songs->insert(make_pair(it->first, it->second));
			}
		}
		return same_name_songs;
	}
	
}


//returns true if the playlists name are in the right order.
bool operator<(const Playlist& a, const Playlist& b) {
	return (a.get_name() < b.get_name());
}

////compare playlists.
//bool operator!=(const Playlist& a, const Playlist& b) {
//	return (a.get_name() != b.get_name());
//}

////returns true if the playlists name are in the right order.
//bool operator<(Playlist* a, Playlist* b) {
//	return ((*a).get_name() < (*b).get_name());
//}

