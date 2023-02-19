#include "DailyMix.h"

DailyMix::DailyMix() {

}

// generate a random mix of 10 songs from the library/server
void DailyMix::generate_daily_mix() {
	auto songs_to_shuffle = Server::get_songs_by_id();
	// Create a vector of iterators to the elements in the multimap
	vector<unordered_multimap<int, Song*>::iterator> shuffled_songs_vector;
	for (auto it = songs_to_shuffle->begin(); it != songs_to_shuffle->end(); ++it) {
		shuffled_songs_vector.push_back(it);
	}
	// Shuffle the keys of the multimap randomly
	random_device rd;
	mt19937 generator(rd());
	shuffle(shuffled_songs_vector.begin(), shuffled_songs_vector.end(), generator);

	//add 10 songs from shuffled_songs_vector to m_daily_mix 
	int i = 1;
	for (int i = 0; i < max_songs || i < shuffled_songs_vector.size(); i++) { // if library has less than 10 songs, mix them also.
		auto song = shuffled_songs_vector[i];
		int id = song->second->get_id();
		m_daily_mix.insert(make_pair(id, song->second));
	}
}


