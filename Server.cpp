#include "Server.h"

//Server::Server()
//{
//	auto compare_by_id = [](Song* a, Song* b) { //lambda function
//		return a->get_id() < b->get_id();
//	};
//	set<int, decltype(compare_by_id)> songs_by_id; //insert here all the songs!
//}

//Song is unique!
void Server::Upload_Song(string song_name, string file_path, string artist = "",
	string album = "", string genre = "", string duration = "", int release_Date = 0)
{
	auto song = new Song(song_name, file_path, album, artist, genre, release_Date);
	const auto& name = song->get_name(); //avoids unnecessary copies of string objects
	const auto& album = song->get_album();
	const auto& artist = song->get_artist();
	const auto& genre = song->get_genre();

	m_songs_by_alphabet_order.emplace(name, song); //constructs the element directly in place
	m_all_songs_by_name.emplace(name, song); //constructs the element directly in place
	m_songs_file_paths.emplace(song->get_path());
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

void Server::Upload_Episode_To_Podcast(Podcast* podcast, string episode_name, string podcast_name, string file_path,
	string duration, int release_Date)
{
	auto new_episode = new Episode(file_path, episode_name, release_Date, duration); //deleted in Podcast
	if (podcast == nullptr) {//if podcast doesn't exist, creates the podcasts and adds the episode
		podcast = new Podcast(podcast_name); //deleted in Server
	}
	podcast->Add_Episode(new_episode); //Adds a UNIQUE episode to an EXISTING podcast

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
	delete song; //???
}

template <typename T>
void Server::remove_song_from_collection(T& songs_by_field, Song* song) {
	auto it = songs_by_field.find(song);
	if (it != songs_by_field.end() && it->second == song) {
		songs_by_field.erase(it);
	}
}
// A template function that takes an unordered map/multimap 
// and deletes all the values 
template<class T>
void Server::Destory_Allocations(T& collection)
{
	// Iterate over the key-value pairs in the collection.
	for (auto& pair : collection) {
		// Delete the value associated with the key.
		delete pair.second;
	}
	// Clear the collection.
	collection.clear();
}


//Returns a unique song based on its id. If doesn't exists throws exception
Song* Server::find_song_by_id(int id)
{
	if (m_all_songs_by_id.count(id) > 0) {
		return m_all_songs_by_id[id];
	}
	throw exception();
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
bool Server::Does_Song_Exist(const string& file_path)
{
	return m_songs_file_paths.count(file_path) > 0;
}
bool Server::Does_Episode_Exist(const string& file_path)
{
	for (auto itr = m_all_podcasts.begin(); itr != m_all_podcasts.end(); itr++)
	{
		auto temp = itr->second;
		if (temp->Is_Episode_In_Podcast(file_path)) {
			return true;
		}
	}
	return false;
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

Server::~Server()
{
	Destory_Allocations(m_all_podcasts);
	Destory_Allocations(m_all_songs_by_id);
}

