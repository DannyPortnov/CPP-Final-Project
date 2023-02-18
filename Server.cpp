#include "Server.h"

//Server::Server()
//{
//	auto compare_by_id = [](Song* a, Song* b) { //lambda function
//		return a->get_id() < b->get_id();
//	};
//	set<int, decltype(compare_by_id)> songs_by_id; //insert here all the songs!
//}

//adds to all data structures
void Server::Upload_Song(Song* song)
{
	const auto& name = song->get_name(); //avoids unnecessary copies of string objects
	const auto& album = song->get_album();
	const auto& artist = song->get_artist();
	const auto& genre = song->get_genre();

	m_songs_by_alphabet_order.emplace(name, song); //constructs the element directly in place
	m_all_songs_by_name.emplace(name, song); //constructs the element directly in place
	m_all_songs_by_id.emplace(song->get_id(), song);
	if (!album.empty()) {
		m_all_songs_by_album.emplace(album, song);
	}
	if (!artist.empty()) {
		m_all_songs_by_artist.emplace(artist, song);
	}
	if (!genre.empty()) {
		m_all_songs_by_genre.emplace(genre, song);
	}
}

void Server::Upload_Podcast_Series(Podcast* podcast)
{
	auto podcast_to_upload = pair<string, Podcast*>(podcast->Get_Podcast_Name(), podcast);
	m_all_podcasts.insert(podcast_to_upload);
	m_podcasts_by_alphabet_order.insert(podcast_to_upload);
}

void Server::Permanent_Delete_Song(Song* song)
{
	#pragma region loop for m_songs_by_alphabet_order 
		//// Find the range of songs with the same name as the given song
	//auto range = m_songs_by_alphabet_order.equal_range(song->get_name());

	//// Loop through relevant songs in m_songs_by_alphabet_order 
	//for (auto& iterator = range.first; iterator != range.second; ++iterator) {
	//	if (iterator->second == song) {  // Check if the song pointer matches
	//		m_songs_by_alphabet_order.erase(iterator);  // Erase the song from the multimap
	//		break;  // Exit the loop once the song is found and removed
	//	}
	//}  
	#pragma endregion

	remove_song_from_collection(m_all_songs_by_album, song); //check that that works
	remove_song_from_collection(m_all_songs_by_genre, song);
	remove_song_from_collection(m_all_songs_by_artist, song);
	remove_song_from_collection(m_all_songs_by_name, song);
	remove_song_from_collection(m_songs_by_alphabet_order, song);
	remove_song_from_collection(m_all_songs_by_id, song);
}

template <typename T>
void Server::remove_song_from_collection(T& songs_by_field, Song* song) {
	auto it = songs_by_field.find(song); 
	if (it != songs_by_field.end() && it->second == song) {
		songs_by_field.erase(it);
	}
}

//void Server::Update(string song_name, string new_name, string artist, string album, string genre, string duration)
//{
//
//}

//Returns a unique song based on its id
Song* Server::find_song_by_id(int id)
{
	return m_all_songs_by_id[id];
}

unordered_multiset<Song*>* Server::find_by_name(string& name)
{
	find(name, m_all_songs_by_name);
}
unordered_multiset<Song*>* Server::find_by_artist(string& singer)
{
	find(singer, m_all_songs_by_artist);
}
unordered_multiset<Song*>* Server::find_by_album(string& album)
{
	find(album, m_all_songs_by_album);
}
unordered_multiset<Song*>* Server::find_by_genre(string& genre)
{
	find(genre, m_all_songs_by_genre);
}
//Searches in given collection based on key, and returns filtered unordered_multiset 
unordered_multiset<Song*>* Server::find(string& key, unordered_multimap<string, Song*>& collection) {
	auto range = collection.equal_range(key); // range of values that match the given name
	unordered_multiset<Song*>* filtered_songs = new unordered_multiset<Song*>;
	for (auto& it = range.first; it != range.second; ++it) {
		filtered_songs->insert(it->second); //inserts each value into the filtered set.
	}
	return filtered_songs;
}
