#include "Library.h"
#include <set>
#include <vector>
#include <algorithm>
//Library::Library() : m_songs_by_name(Server::get_songs_by_name()), m_playlists()
//{
//	// = new set<Song*>*; //is initialization needed here?
//	//m_playlists_map= &map<string, Playlist*>(m_playlists->get_user_playlists()->begin(), m_playlists->get_user_playlists()->end()); //map of playlists
//}

Library::Library() : m_favorites(new Playlist("favorites")), m_daily_mix(new Playlist("daily mix")),
m_deleted(new Playlist("deleted")), m_recent(new Playlist("recent")), m_most_played(new Playlist("most played")) {}

//todo: add getters for each playlists
Library::~Library() {
	delete m_daily_mix;
	delete m_deleted;
	delete m_favorites;
	delete m_most_played;
	delete m_recent;
	if (m_user_playlists.size()) {
		unordered_map<string, Playlist*>::iterator it;
		for (it = m_user_playlists.begin(); it != m_user_playlists.end(); it++) {
			delete it->second;
		}
	}
	m_user_playlists.clear();
	m_user_playlist_names.clear();
}

void Library::PrintSong(int id)
{
	cout << Server::find_song_by_id(id)<< endl;
}


void Library::PrintSong(string song_name)
{
	//niv
}

// return true if playlist exist, false if not.
bool Library::check_if_playlist_exist(const string& playlist_name) {
	if (m_user_playlists.find(playlist_name) != m_user_playlists.end()) { // if not found, find method returns '.end()' element
		return true;
	}
	return false;
}

// return true if playlist can be edited, false if not (user can edit favorites) 
bool Library::check_if_playlist_can_be_edited(const string& playlist_name) {
	if (playlist_name == m_recent->get_name() || playlist_name == m_daily_mix->get_name() ||
		playlist_name == m_most_played->get_name()) {
		return false;
	}
	return true;
}

// create a new playlist
void Library::create_playlist(const string& playlist_name) {
	if (check_if_playlist_exist(playlist_name)) {	
		cout << "A playlist with the name: " << playlist_name << " is already exists!" << endl; // todo: change to try, throw
	}
	else { //todo: check that the name is legal
		m_user_playlist_names.insert(playlist_name);
		Playlist* new_playlist = new Playlist(playlist_name);
	//	m_user_playlists.insert(make_pair(playlist_name, new_playlist));
		m_user_playlists[playlist_name] = new_playlist;
	}
}

//delete a playlist
void Library::delete_playlist(Playlist* playlist) {
	if (check_if_playlist_exist(playlist->get_name())) {
		m_user_playlist_names.erase(playlist->get_name());
		m_user_playlists.erase(playlist->get_name());
		playlist->clear_all_playlist();
		delete playlist;
		cout << "Playlist Was Successfully Deleted!" << endl;
	}
	// if m_deleted was selected, we need to empty this playlist.
	else if (playlist->get_name() == m_deleted->get_name()) {
		multimap<string, Song*>::iterator it;
		for (it = playlist->get_songs().begin(); it != playlist->get_songs().end(); it++) {
			Server::Permanent_Delete_Song(it->second); 									
		}
		playlist->clear_all_playlist();
	}
	//todo: maybe we don't need this else if:
	else if (check_if_playlist_can_be_edited(playlist->get_name())) {
		cout << "This Playlist Cannot Be Deleted!" << endl;
	}
	else {
		cout << "This Playlist Cannot Be Deleted!" << endl;
	}
}

// print all exisiting playlists (names only)
void Library::PrintPL() {
	int i = 1;
	cout << "List Of Playlists:" << endl;
	cout << "(" << i << "). " << m_favorites->get_name() << endl;
	i++;
	cout << "(" << i << "). " << m_most_played->get_name() << endl;
	i++;
	cout << "(" << i << "). " << m_recent->get_name() << endl;
	i++;
	set<string>::iterator it;
	for (it = m_user_playlist_names.begin(); it != m_user_playlist_names.end(); it++) { //m_user_playlist_names is in alphabet order
		cout << "(" << i << "). " << *it << endl;
		i++;
	}
	cout << "(" << i << "). " << m_deleted->get_name() << endl;
}

//todo: check if implementation needed, if so, change implementation
//set<Playlist*>* Library::get_user_playlists()
//{
//	return &user_playlists;
//}


//Add a song by its ID to a playlist. Creates it if it doesn't exist
void Library::Add2PL(int id, const string& playlist_name)
{
	if (check_if_playlist_exist(playlist_name)) {
		if (m_user_playlists.find(playlist_name)->second->check_if_song_exist_in_playlist(Server::find_song_by_id(id)->get_name())) {
			cout << "This Song Is Already In The Playlist!" << endl;
		}
		else {
			m_user_playlists.find(playlist_name)->second->add_song_to_playlist(Server::find_song_by_id(id));
			cout << "Song Was Successfully Added!" << endl;
		}
	}
	else if (check_if_playlist_can_be_edited(playlist_name)) { //todo: check if creating a new playlist is nessecary
		if (playlist_name != m_favorites->get_name()) {
			create_playlist(playlist_name);
		}
		m_user_playlists.find(playlist_name)->second->add_song_to_playlist(Server::find_song_by_id(id));
		cout << "Song Was Successfully Added!" << endl;
	}
	else {
		cout << "This Playlist Cannot Be Edited!" << endl;
	}
}

// double checks with the user if the song should be deleted.
char Library::ask_user_to_remove_song(int id, const string& playlist_name) {
	cout << "You chose to remove the song: " << endl;
	PrintSong(id);
	cout << "Are you sure you want to remove this song from: " << playlist_name << "?" << endl;
	cout << "Press y/n: " << endl;
	char answer;
	cin >> answer;
	return answer;
}

// remove a song from the playlist by song's name.
void Library::RemoveFromPL(string& song_name, const string& playlist_name)
{
	if (check_if_playlist_can_be_edited(playlist_name)) { // return true if can be edited
		if (check_if_playlist_exist(playlist_name) || playlist_name == m_favorites->get_name()) {
			if (m_user_playlists.find(playlist_name)->second->check_if_song_exist_in_playlist(song_name)) {
				if (m_user_playlists.find(playlist_name)->second->check_if_songs_have_same_names(song_name)) {
					unordered_multimap<string, Song*>* same_name_songs =
						m_user_playlists.find(playlist_name)->second->get_songs_with_same_name(song_name);
					cout << "There are few songs with the same name:" << endl;
					unordered_multimap<string, Song*>::iterator it;
					int i = 1;
					for (it = (*same_name_songs).begin(); it != (*same_name_songs).end(); it++) {
						cout << "(" << i << "). " << *(it->second) << endl; // it->second contains Song*
						i++;
					}
					cout << "Which song do you want to remove from " << playlist_name << "?" << endl;
					cout << "Type the id of the song you would like to remove: ";
					bool id_not_found = true;
					//unordered_multimap<string, Song*>::iterator it;
					while (id_not_found) {
						int chosen_id;
						cin >> chosen_id;
						for (it = (*same_name_songs).begin(); it != (*same_name_songs).end(); it++) {
							if (it->second->get_id() == chosen_id) {
								id_not_found = false;
								if ((ask_user_to_remove_song(chosen_id, playlist_name)) == 'y') {
									m_user_playlists.find(playlist_name)->second->remove_song_from_playlist(it->first);
									cout << "Song Was Successfully Removed From Playlist!" << endl;
								}
								else
									cout << "The Song Wasn't Removed!" << endl;
							}
						}
						if (id_not_found)
							cout << "ID not found! please enter id again: " << endl;
					}
					delete same_name_songs; // maybe we need to delete here 
				}
				else {
					int song_id = m_user_playlists.find(playlist_name)->second->get_song_by_name(song_name)->get_id();
					if ((ask_user_to_remove_song(song_id, playlist_name)) == 'y') {
						m_user_playlists.find(playlist_name)->second->remove_song_from_playlist(song_name);
						cout << "Song Was Successfully Removed From Playlist!" << endl;
					}
					else
						cout << "The Song Wasn't Removed!" << endl;
				}
			}
			else {
				cout << "The song" << song_name << "is not in the playlist: " << playlist_name << "!" << endl;
			}
		}
		else {
			cout << "This Playlist Does Not Exist!" << endl;
		}
	}
	else {
		cout << "This Playlist Cannot Be Edited!" << endl;
	}
}



//Asks the user which song he meant and updates the choosen one.
void Library::Update_Song(string song_name, string new_name, string artist,
	string album, string genre, string duration)
{
	auto picked_song = Pick_Media(song_name, Server::get_songs_by_name());
	if (picked_song != nullptr) {
		Update_Song(picked_song->get_id(), new_name, artist, album, genre, duration);
		return;
	}
	cout << "No songs named " << song_name << " currently in the server. Please add it first" << endl;
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
void Library::Update_Song(int song_id, string new_name, string artist, string album, string genre, string duration)
{
	//todo: add check to see if id exists
	auto picked_song = Server::find_song_by_id(song_id);
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

void Library::Play(string song_name)
{
	//todo: niv
}

void Library::Play(int id)
{
	//todo: niv
}

#pragma region Old implementation of RemoveFromPL
//void Library::RemoveFromPL(string& song_name, const string& playlist_name)
//{
//	Update_Playlists_Map();
//	auto songs_by_names = m_playlists_map->at(playlist_name)->Get_Songs();
//	multiset<Song*>::iterator start_of_songs_set; //iterator for any/all songs with that name
//	multiset<Song*>::iterator end_of_songs_set;
//	multiset<Song*>::iterator first_wanted_song;
//	multiset<Song*>::iterator last_wanted_song;
//	int number_of_songs = Count_Songs(&songs_by_names, song_name, &start_of_songs_set, &end_of_songs_set, &first_wanted_song, &last_wanted_song);
//	switch (number_of_songs)
//	{
//	case 0: {
//		cout << "No songs named " << song_name << " currently in " << playlist_name << "." << endl;
//		return;
//	}
//	case 1: {
//		m_playlists_map->at(playlist_name)->remove_song(*first_wanted_song); //or *last doesn't matter
//		return;
//	}
//	default:
//		break;
//	}
//	cout << "More than one song named " << song_name << ", choose which one you want to remove" << endl;
//	//m_playlists_map->at(playlist_name)->remove_song(Pick_Song( )); //removes the selected song
//}  
#pragma endregion


//called from the interface ("main")
void Library::Add_Song(string song_name, string file_path, string artist = "",
	string album = "", string genre = "", string duration = "", int release_Date = 0)
{
	//todo: check if song exists already
	//todo: move new to server
	Server::Upload_Song(new Song(song_name, file_path, album, artist, genre, release_Date));
	//is deleted in Server::Permanent_Delete_Song

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

void Library::Add_Podcast_Episode(string episode_name, string podcast_name, string file_path,
	string duration, int release_Date )
{
	if (Server::Does_Episode_Exist(file_path)) { //checks uniqueness
		cout << "Episode was already added" << endl;
		return;
	}
	auto new_episode = new Episode(file_path, episode_name, release_Date, duration);
	auto picked_podcast = Pick_Media(podcast_name, Server::get_podcasts_by_name());
	if (picked_podcast == nullptr) {//if podcast doesn't exist, creates the podcasts and adds the episode
		picked_podcast = new Podcast(podcast_name);
		Server::Upload_Podcast_Series(picked_podcast);//Upload the new podcast
	}
	picked_podcast->Add_Episode(new_episode); //Adds a UNIQUE episode to an EXISTING podcast
}
template <class T>
T* Library::Pick_Media(string media_name, unordered_multimap<string, T*>* collection_to_search) {
	auto filtered_media = collection_to_search.equal_range(media_name);
	int number_of_media_items = collection_to_search.count(media_name); //O(log n), distance is O(n). overall doesn't matter
	if (number_of_media_items == 0) {
		return nullptr;
	}
	if (number_of_media_items == 1) {
		return filtered_media.first->second;
	}
	int i = 1;//can't define 2 variables of different type in for (you can but it's less readable)
	for (auto& iterator = filtered_media.first; iterator != filtered_media.second; ++iterator, ++i) {//loops over all podcasts
		cout << i << " - " << iterator->second << endl;//prints numbered songs
	}
	int answer;
	do {
		cout << "Please choose a number between 1 and " << number_of_media_items << " (0 to cancel): ";
		cin >> answer;
	} while (answer < 0 || answer > number_of_media_items); //todo: check string input
	if (answer == 0) {
		return nullptr; //todo: differentiate between not finding and canceling
	}
	auto& iterator = filtered_media.first;
	advance(iterator, answer - 1);
	return iterator->second;
}
#pragma region Previous implementation of pick_podcast and pick_song

//Podcast* Library::Pick_Podcast(string podcast_name) {
//	auto& all_podcasts = *Server::get_podcasts_by_name();
//	auto filtered_podcasts = all_podcasts.equal_range(podcast_name);
//	int number_of_songs = all_podcasts.count(podcast_name); //O(log n), distance is O(n). overall doesn't matter
//	if (number_of_songs == 0) {
//		return nullptr;
//	}
//	if (number_of_songs == 1) {
//		return filtered_podcasts.first->second;
//	}
//	int i = 1;//can't define 2 variables of different type in for (you can but it's less readable)
//	for (auto& iterator = filtered_podcasts.first; iterator != filtered_podcasts.second; ++iterator, ++i) {//loops over all podcasts
//		cout << i << " - " << iterator->second << endl;//prints numbered songs
//	}
//	int answer;
//	do {
//		cout << "Please choose a number between 1 and " << number_of_songs << " (0 to cancel): ";
//		cin >> answer;
//	} while (answer < 0 || answer > number_of_songs);
//	if (answer == 0) {
//		return nullptr;
//	}
//	auto& iterator = filtered_podcasts.first;
//	advance(iterator, answer - 1);
//	return iterator->second;
//}
//
//
//Song* Library::Pick_Song(string song_name) {
//	unordered_multimap<string, Song*> all_songs = *Server::get_songs_by_name();
//	auto filtered_songs = all_songs.equal_range(song_name);
//	int number_of_songs = all_songs.count(song_name); //O(log n), distance is O(n). overall doesn't matter
//	if (number_of_songs == 0) {
//		return nullptr; 
//	}
//	if (number_of_songs == 1) {
//		return filtered_songs.first->second;
//	}
//	int i = 1;//can't define 2 variables of different type in for (you can but it's less readable)
//	for (auto& iterator = filtered_songs.first; iterator != filtered_songs.second; ++iterator, ++i) {//loops over all songs to delete
//		cout << i << " - " << iterator->second << endl;//prints numbered songs
//	}
//	int answer;
//	do {
//		cout << "Please choose a number between 1 and " << number_of_songs << " (0 to cancel): ";
//		cin >> answer;
//	} while (answer < 0 || answer > number_of_songs);
//	if (answer == 0) {
//		return nullptr;
//	}
//	auto& iterator = filtered_songs.first;
//	advance(iterator, answer - 1);
//	return iterator->second;
//}  
#pragma endregion

int Library::Count_Songs(multiset<Song*>* songs, string song_name, multiset<Song*>::iterator* start, multiset<Song*>::iterator* end,
	multiset<Song*>::iterator* first_wanted_song, multiset<Song*>::iterator* last_wanted_song) const {

	auto predicate = [&](const Song* song) { return song->get_name() == song_name; }; //filter songs which have the desired name
	first_wanted_song = lower_bound(start, end, predicate);
	last_wanted_song = upper_bound(start, end, predicate);
	return count_if(start, end, predicate); //count songs with "song_name" name
}

void Library::Delete(string song_name)
{
	auto picked_song = Pick_Media(song_name, Server::get_songs_by_name());
	if (picked_song != nullptr) {
		Server::Permanent_Delete_Song(picked_song);
		return;
	}
	cout <<  song_name << " isn't present in the server." << endl; 
}
void Library::Delete(int id) 
{
	//todo: after inherting from server, avoid calling Pick_Media when Delete(string song_name) is called from Delete(int id) 
	Server::Permanent_Delete_Song(Server::find_song_by_id(id));
}

ostream& Library::Print(ostream& os, int begin, int end) const
{
	auto itr = Server::get_songs_by_name()->begin();
	for (int i = 0; i < begin && itr != Server::get_songs_by_name()->end(); i++, itr++) {} //if begin!=0, inc itr untill reached begin
	for (int i = begin; i < end && itr != Server::get_songs_by_name()->end(); i++, itr++)
	{
		cout << itr->second << endl;
	}
	if (itr == Server::get_songs_by_name()->end()) {
		os << "No more songs :(" << endl;
	}
	return os;
}

ostream& operator<<(ostream& os, const Library& lib)
{
	return lib.Print(os, 0, Library::num_of_songs_to_print);
}
