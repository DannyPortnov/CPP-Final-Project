#include "Library.h"
#include "Playlist.h"


Playlist::Playlist(std::string name, Library* library, Server* server, bool restore_songs) : 
	m_playlist_name(name), m_library(library), m_server(server),
	m_songs(CompareSongsByName{}) //Inserts songs by alphabet order of their name!
{
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
	std::string accept_message = "Continue playing " + m_playlist_name;
	if (Utilities::user_prompts_and_dialog(prompt, reject_message, accept_message)) {
		return true;
	}
	return false;
}

// play the songs in alphabetical order
void Playlist::Play(bool shuffle) {
	std::vector<Song*> songs_vector(m_songs.begin(), m_songs.end()); //range constructor
	m_library->PlayAll(&songs_vector,"Playing " + m_playlist_name + ":", shuffle, false);
}

//returns the playlist
std::multiset<Song*, Playlist::CompareSongsByName> Playlist::get_songs() {
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
		song->remove_from_playlist(m_playlist_name);
	}
	// the printing of the results of the operation happans in make_sure_to_remove
}


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
}

void Playlist::save_playlist(std::string file_name, ios_base::openmode mode) {
	std::string address = file_name;
	Utilities::Format_Address(address);
	ofstream write_playlist(address, mode);
	if (!Utilities::Is_File_Valid(write_playlist, file_name)) {
		return;
	}
	std::vector<std::string*> params = { &m_playlist_name };
	Utilities::Replace_All(params, false);
	for (auto& song : m_songs) {
		write_playlist << m_playlist_name << " " << song->get_id() << std::endl;
	}
}
void Playlist::restore_playlist(std::string file_name) {
	std::string address = file_name;
	Utilities::Format_Address(address);
	std::ifstream read_user_playlists(address, ios::in);
	if (!Utilities::Is_File_Valid(read_user_playlists, file_name)) {
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
	} while (answer < 0 || answer > number_of_media_items); 
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