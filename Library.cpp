#include "Library.h"
#include <set>
#include <vector>
#include <algorithm>
#include <limits>

Library::Library() : m_songs_by_name(Server::get_songs_by_name()), m_playlists()
{
	// = new set<Song*>*; //is initialization needed here?
	//m_playlists_map= &map<string, Playlist*>(m_playlists->get_user_playlists()->begin(), m_playlists->get_user_playlists()->end()); //map of playlists
}

void Library::PrintSong(int id)
{
	cout << m_songs_by_id->at(id) << endl;
}

void Library::PrintPL()
{

}

//Asks the user which song he meant and updates the choosen one.
void Library::Update(string song_name, string new_name, string artist,
	string album, string genre, string duration)
{
	auto picked_song = Pick_Song(song_name);
	if (picked_song != nullptr) {
		Update(picked_song->get_id(), new_name, artist, album, genre, duration);
	}


#pragma region deleting than adding
	//if (new_name.empty() && artist.empty() && album.empty()
//	&& genre.empty() && genre.empty() && duration.empty()) {
//	cout << "No information to update" << endl;
//	return;
//}
//auto& file_path_of_song = Pick_Song(song_name)->get_path();
//Delete(song_name);
//Add(new_name, file_path_of_song, artist, album, genre, duration);  
#pragma endregion

}
//Find choosen song and updates according to the parameters recieved
void Library::Update(int song_id, string new_name, string artist, string album, string genre, string duration)
{
	auto picked_song = Server::get_songs_by_id()[song_id];
	if (!new_name.empty()) {
		picked_song->set_name(new_name);
	}
	if (!artist.empty()) {
		picked_song->set_artist(artist);
	}
	if (!album.empty()) {
		picked_song->set_album(artist);
	}
	if (!genre.empty()) {
		picked_song->set_album(genre);
	}
	if (!duration.empty()) {
		picked_song->set_duration(duration);
	}
}

void Library::RemoveFromPL(string song_name, string playlist_name)
{
	Update_Playlists_Map();
	auto songs_by_names = m_playlists_map->at(playlist_name)->Get_Songs();
	multiset<Song*>::iterator start_of_songs_set; //iterator for any/all songs with that name
	multiset<Song*>::iterator end_of_songs_set;
	multiset<Song*>::iterator first_wanted_song;
	multiset<Song*>::iterator last_wanted_song;
	int number_of_songs = Count_Songs(&songs_by_names, song_name, &start_of_songs_set, &end_of_songs_set, &first_wanted_song, &last_wanted_song);
	switch (number_of_songs)
	{
	case 0: {
		cout << "No songs named " << song_name << " currently in " << playlist_name << "." << endl;
		return;
	}
	case 1: {
		m_playlists_map->at(playlist_name)->remove_song(*first_wanted_song); //or *last doesn't matter
		return;
	}
	default:
		break;
	}
	cout << "More than one song named " << song_name << ", choose which one you want to remove" << endl;
	//m_playlists_map->at(playlist_name)->remove_song(Pick_Song( )); //removes the selected song
}
//Add a song by its ID to a playlist. Creates it if it doesn't exist
void Library::Add2PL(int id, string playlist_name)
{
	m_playlists->Add(playlist_name); //add new playlist if doesn't exist
	Update_Playlists_Map();
	m_playlists_map->at(playlist_name)->add_song(m_songs_by_id->at(id)); //(add the song)
}


//called from the interface ("main")
void Library::Add(string song_name, string file_path, string artist = "",
	string album = "", string genre = "", string duration = "", int release_Date = 0)
{
	Server::Upload_Song(new Song(song_name, file_path, album, artist, genre, release_Date));

#pragma region Algorithm to find all songs by that name and choosing specific one
	//auto all_songs = Server::get_songs_by_name();
	//multiset<Song*>::iterator start_of_songs_set; //iterator for any/all songs with that name
	//multiset<Song*>::iterator end_of_songs_set;
	//multiset<Song*>::iterator first_wanted_song;
	//multiset<Song*>::iterator last_wanted_song;
	//int number_of_songs = Count_Songs(all_songs, song_name, &start_of_songs_set, &end_of_songs_set, &first_wanted_song, &last_wanted_song);
	//switch (number_of_songs)
	//{
	//case 0: {
	//	cout << "No songs named " << song_name << " currently in the server. Please download it first and place it in the folder" << endl;
	//	return;
	//}
	//case 1: {
	//	m_songs_by_name->insert(*first_wanted_song); //or *last doesn't matter
	//	return;
	//}
	//default:
	//	break;
	//}
	//cout << "More than one song named " << song_name << ", choose which one you want to add" << endl;
	//m_songs_by_name->insert(Pick_Song(number_of_songs, &first_wanted_song, &last_wanted_song));  
#pragma endregion
}

//Returns the choosen song. If wrong name returns nullptr!
Song* Library::Pick_Song(string song_name) {
	unordered_multimap<string, Song*> all_songs = Server::get_songs_by_name();
	auto filtered_songs = all_songs.equal_range(song_name);
	int number_of_songs = all_songs.count(song_name); //O(log n), distance is O(n). overall doesn't matter
	if (number_of_songs == 0) {
		cout << "No songs named " << song_name << " currently in the server. Please add it first" << endl;
		return; //what does that do?? 
	}
	if (number_of_songs == 1) {
		return filtered_songs.first->second;
	}
	int i = 1;//can't define 2 variables of different type in for (you can but it's less readable)
	for (auto& iterator = filtered_songs.first; iterator != filtered_songs.second; ++iterator, ++i) {//loops over all songs to delete
		cout << i << " - " << iterator->second << endl;//prints numbered songs
	}
	int answer;
	do {
		cout << "Please choose a number between 1 and " << number_of_songs << " (0 to cancel): ";
		cin >> answer;
	} while (answer < 0 || answer > number_of_songs);
	if (answer == 0) {
		return nullptr;
	}
	auto& iterator = filtered_songs.first;
	advance(iterator, answer - 1);
	return iterator->second;
}

void Library::Update_Playlists_Map()
{
	m_playlists_map = &map<string, Playlist*>(m_playlists->get_user_playlists()->begin(), m_playlists->get_user_playlists()->end()); //map of playlists
}



int Library::Count_Songs(multiset<Song*>* songs, string song_name, multiset<Song*>::iterator* start, multiset<Song*>::iterator* end,
	multiset<Song*>::iterator* first_wanted_song, multiset<Song*>::iterator* last_wanted_song) const {

	auto predicate = [&](const Song* song) { return song->get_name() == song_name; }; //filter songs which have the desired name
	first_wanted_song = lower_bound(start, end, predicate);
	last_wanted_song = upper_bound(start, end, predicate);
	return count_if(start, end, predicate); //count songs with "song_name" name
}

void Library::Delete(string song_name)
{
	auto picked_song = Pick_Song(song_name);
	if (picked_song != nullptr) {
		Server::Permanent_Delete_Song(picked_song);
	}
}
void Library::Delete(int id)
{
	Server::Permanent_Delete_Song(Server::get_songs_by_id()[id]);
}

ostream& Library::Print(ostream& os, int begin, int end) const
{
	auto itr = m_songs_by_name->begin();
	for (int i = 0; i < begin && itr != m_songs_by_name->end(); i++, itr++) {} //if begin!=0, inc itr untill reached begin
	for (int i = begin; i < end && itr != m_songs_by_name->end(); i++, itr++)
	{
		cout << *itr << endl;
	}
	if (itr == m_songs_by_name->end()) {
		os << "No more songs :(" << endl;
	}
	return os;
}

ostream& operator<<(ostream& os, const Library& lib)
{
	return lib.Print(os, 0, Library::num_of_songs_to_print);
}
