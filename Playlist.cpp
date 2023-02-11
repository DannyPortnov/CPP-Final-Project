#include "Playlist.h"



Playlist::Playlist(string name) : m_name(name) {}

Playlist::~Playlist() {
	audio_files.clear();
}

// play the songs in alphabetical order
void Playlist::Play() {
	set<Song*>::iterator it;
	for (it = audio_files.begin(); it != audio_files.end(); it++) {
		m_player.play((*it)->get_path(), true); //todo: check if true is needed (not sure what is the purpose of wait)
	}
}

// play the songs randomly
void Playlist::Play_Random() {
	// Convert the set to a vector for shuffling
	std::vector<Song*> audio_files_vector(audio_files.begin(), audio_files.end());

	// Seed the random number generator with the current time
	std::mt19937 generator(time(nullptr));

	// Shuffle the vector
	std::shuffle(audio_files_vector.begin(), audio_files_vector.end(), generator);

	// Print the shuffled elements using an iterator
	std::vector<Song*>::iterator it;
	for (it = audio_files_vector.begin(); it != audio_files_vector.end(); it++) {
		m_player.play((*it)->get_path(), true); //todo: check if true is needed (not sure what is the purpose of wait)
	}

	// todo: check if necessary
	// Free the dynamically allocated objects
	/*for (it = audio_files_vector.begin(); it != audio_files_vector.end(); it++) {
		delete* it;
	}*/

}


// print the songs in the playlist (organized alphabetically)
void Playlist::Print() {
	set<Song*>::iterator it;
	int i = 1;
	cout << "Playlist- " << m_name << ". Songs:";
	for (it = audio_files.begin(); it != audio_files.end(); it++) {
		cout << "(" << i << "). " << (*it)->get_name() << endl; 
		i++;
	}
}

//returns the songs in the playlist
multiset<Song*> Playlist::Get_Songs() const {
	return audio_files;
}

// add a song to the playlist. audio files is a multiset
void Playlist::add_song(Song* song) {
	audio_files.insert(song);
}

// remove a song to the playlist. audio files is a multiset
void Playlist::remove_song(Song* song) {
	audio_files.erase(song);

}





//returns true if the playlists name are in the right order.
bool operator<(const Playlist& a, const Playlist& b) {
	return (a.get_name() < b.get_name());

}


////returns true if the playlists name are in the right order.
//bool operator<(Playlist* a, Playlist* b) {
//	return ((*a).get_name() < (*b).get_name());
//}

