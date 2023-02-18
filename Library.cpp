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
m_deleted(new Playlist("deleted")), m_recent(new Playlist("recent")), m_most_played(new Playlist("most played")) 
{
	m_saved_playlist_names.insert("favorites");
	m_saved_playlist_names.insert("daily mix");
	m_saved_playlist_names.insert("deleted");
	m_saved_playlist_names.insert("recent");
	m_saved_playlist_names.insert("most played");

}

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
	cout << "The song details are:" << endl;
	cout << Server::find_song_by_id(id)<< endl;
}

// print all songs with this name.
void Library::PrintSong(string song_name)
{
	auto songs = Server::find_by_name(song_name);
	auto count = songs->count(song_name);
	if (count == 0) {
		cout << "Song wasn't found in the database!" << endl;
		return;
	}
	if (count > 1) {
		cout << "Few songs with the name: " << song_name << " were found: ";
	}
	unordered_multimap<string, Song*>::iterator it;
	int i = 1;
	for (it = songs->begin(); it != songs->end(); it++) {
		cout << "(" << i << "). " << *(it->second) << endl; // it->second contains Song*
		i++;
	}
}

// return true if playlist exist, false if not.
bool Library::check_if_user_playlist_exist(const string& playlist_name) {
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
	if (check_if_user_playlist_exist(playlist_name)) {	
		cout << "A playlist with the name: " << playlist_name << " is already exists!" << endl; // todo: change to try, throw
		return;
	}
	else if (m_saved_playlist_names.find(playlist_name) != m_saved_playlist_names.end()) {
		cout << "A playlist with the name: " << playlist_name << " is already exists!" << endl; // todo: change to try, throw
		return;
	}
	m_user_playlist_names.insert(playlist_name);
	Playlist* new_playlist = new Playlist(playlist_name); // need to delete!
	m_user_playlists[playlist_name] = new_playlist;
}

//delete a playlist
void Library::delete_playlist(Playlist* playlist) {
	if (check_if_user_playlist_exist(playlist->get_name())) {
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
	else 
		cout << "This Playlist Cannot Be Deleted!" << endl;
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

//todo: maybe to move the checks of if the song is in a playlist to the add_to playlist method in playlsits.
//Add a song by its ID to a playlist. Creates it if it doesn't exist
void Library::Add2PL(int id, const string& playlist_name)
{
	if (check_if_user_playlist_exist(playlist_name)) {
		if (m_user_playlists.find(playlist_name)->second->check_if_song_exist_in_playlist(Server::find_song_by_id(id)->get_name())) {
			cout << "This Song Is Already In The Playlist!" << endl;
		}
		else {
			m_user_playlists.find(playlist_name)->second->add_song_to_playlist(Server::find_song_by_id(id));
			cout << "Song Was Successfully Added!" << endl;
		}
	}
	else if (check_if_playlist_can_be_edited(playlist_name)) { 
		if (playlist_name != m_favorites->get_name()) {
			cout << "No Such Playlist Was Found!" << endl;
			cout << "A Playlist With The Name: " << playlist_name << " Was Created!" << endl;
			create_playlist(playlist_name); // creates the playlist automatically
		}
		m_user_playlists.find(playlist_name)->second->add_song_to_playlist(Server::find_song_by_id(id));
		cout << "Song Was Successfully Added!" << endl;
	}
	else {
		cout << "This Playlist Cannot Be Edited!" << endl;
	}
}

// double checks with the user if the song should be deleted, if yes- removes the song.
void Library::ask_user_to_remove_song(Song* song, Playlist* playlist) {
	cout << "You chose to remove the song: " << endl;
	PrintSong(song->get_id());
	cout << "Are you sure you want to remove this song from: " << playlist->get_name() << "?" << endl;
	cout << "Press y/n: ";
	char answer;
	cin >> answer;
	cout << endl;
	if (answer == 'y') {
		playlist->remove_song_from_playlist(song->get_name());
		cout << "Song Was Successfully Removed From Playlist!" << endl;
	}
	else
		cout << "The Song Wasn't Removed!" << endl;
}

//todo: maybe to move the checks of if the song is in a playlist to the remove from playlist method in playlsits.
// remove a song from the playlist by song's name.
void Library::RemoveFromPL(string& song_name, const string& playlist_name) {
	if (check_if_playlist_can_be_edited(playlist_name) && m_deleted->get_name() != playlist_name) {
		if (check_if_user_playlist_exist(playlist_name) || playlist_name == m_favorites->get_name()) {
			auto playlist = m_user_playlists.find(playlist_name)->second;
			if (playlist->check_if_song_exist_in_playlist(song_name)) {
				if (playlist->check_if_songs_have_same_names(song_name)) {
					unordered_multimap<string, Song*>* same_name_songs = playlist->get_songs_with_same_name(song_name);
					cout << "There are few songs with the same name:" << endl;
					cout << "Which song do you want to remove from " << playlist_name << "?" << endl;
					auto song_to_remove = Pick_Media(song_name, same_name_songs);
					ask_user_to_remove_song(song_to_remove, playlist);
					delete same_name_songs; // maybe we need to delete here 
				}
				else {
					auto song_to_remove = playlist->get_song_by_name(song_name);
					ask_user_to_remove_song(song_to_remove, playlist);
				}
			}
			else
				cout << "The song" << song_name << "is not in the playlist: " << playlist_name << "!" << endl;
		}
		else
			cout << "This Playlist Does Not Exist!" << endl;
	}
	else
		cout << "This Playlist Cannot Be Edited!" << endl;
}


//Asks the user which song he meant and updates the choosen one.
void Library::Update_Song(string song_name, string new_name,
 string artist, string album, string genre, string duration, int release_date)
{
	try
	{
		if (Are_All_Parameters_Empty(artist, album, genre, duration, new_name) && release_date == 0) {
			Print_No_Input_Parameters_Error();
		}
		auto picked_song = Pick_Media(song_name, Server::get_songs_by_name());
		if (picked_song != nullptr) {
			Update_Song(picked_song->get_id(), new_name, artist, album, genre, duration);
			return;
		}
		Print_Not_Found_By_Name_Error(song_name);
	}
	catch (const std::exception&) //if caught, user canceled picking the song
	{
		return;
	}
}

////Asks the user which song he meant and updates the choosen one.
//void Library::Update_Episode(string episode_name, string new_name, string duration, int release_date)
//{
//	try
//	{
//		if (Are_All_Parameters_Empty(new_name, duration) && release_date == 0) {
//			Print_No_Input_Parameters_Error();
//		}
//		auto picked_episode = Pick_Media(episode_name, Server::get_());
//		if (picked_song != nullptr) {
//			Update_Song(picked_song->get_id(), new_name, artist, album, genre, duration);
//			return;
//		}
//		Print_Not_Found_By_Name_Error(song_name);
//	}
//	catch (const std::exception&) //if caught, user canceled picking the song
//	{
//		return;
//	}
//}


bool Library::Are_All_Parameters_Empty(const string & param1,
 const string & param2, const string & param3 = "", const string & param4 = "", const string & param5 = "")
{
	return param1.empty() && param2.empty() && param3.empty() && param4.empty();
}

//void Library::Are_All_Parameters_Empty(std::string& artist, std::string& album, std::string& genre, std::string& duration)
//{
//	std::vector<std::string> params = { artist, album, genre,duration };
//	bool allEmpty = std::all_of(std::begin(params),
//		std::end(params),
//		[](const std::string& str) {
//			return str.empty();
//		});
//}

//Find choosen song and updates according to the parameters recieved
void Library::Update_Song(int song_id, string new_name, string artist, string album, string genre, string duration, int release_date)
{
	try
	{
		auto picked_song= Update_Media_By_Id(song_id, Server::find_song_by_id, new_name, duration, release_date);
		if (!artist.empty()) {
			picked_song->set_artist(artist);
		}
		if (!album.empty()) {
			picked_song->set_album(artist);
		}
		if (!genre.empty()) {
			picked_song->set_album(genre);
		}
	}
	catch (const std::exception&)
	{
		Print_Not_Found_By_Id_Error(song_id, typeid(Song).name());
	}
}

void Library::UpdatePodcast(string podcast_name, string new_name)
{
	try
	{
		if (new_name.empty()) {
			Print_No_Input_Parameters_Error();
		}
		auto picked_podcast = Pick_Media(podcast_name, Server::get_podcasts_by_name());
		if (picked_podcast != nullptr) {
			picked_podcast->Set_Podcast_Name(new_name); //not empty!
			return;
		}
		Print_Not_Found_By_Name_Error(podcast_name);
	}
	catch (const std::exception&) //if caught, user canceled picking the song
	{
		return;
	}
}

void Library::UpdateEpisode(int episode_id, string new_name, string duration, int release_date)
{
	try
	{
		Update_Media_By_Id(episode_id, Server::find_episode_by_id, new_name, duration, release_date);
	}
	catch (const std::exception&)
	{
		Print_Not_Found_By_Id_Error(episode_id, typeid(Episode).name());
	}
	
}

template <class T>
T* Library::Update_Media_By_Id(int media_id, T* (*find_media_by_id)(int), string new_name, string duration, int release_date)
{
	//Here if all paramters are empty it does nothing
	auto picked_media = find_media_by_id(media_id);
	if (!new_name.empty()) {
		picked_media->set_name(new_name);
	}
	if (!duration.empty()) {
		picked_media->set_duration(duration);
	}
	if (release_date != 0) {
		picked_media->set_release_date(release_date);
	}
	return picked_media;
}


void Library::Print_Not_Found_By_Id_Error(int item_id, string item_type)
{
	cout << item_type << " with id " << item_id << " doesn't exist in server. Please add it first" << endl;
}

void Library::Print_No_Input_Parameters_Error()
{
	cout << "No paramters to updated" << endl;
}

void Library::Play(string song_name)
{
	auto song = Server::find_by_name(song_name);
	auto count = song->count(song_name);
	PrintSong(song_name);
	if (count == 0) {
		cout << "Song could not be played." << endl;
		return;
	}
	auto song_to_play = Pick_Media(song_name, song);
	if (song_to_play == nullptr) {
		cout << "You didn't choose a specific song to play." << endl;
		return;
	}
	cout << "Now playing: " << *song_to_play << endl;
	song_to_play->Play();

}

void Library::Play(int id)
{
	auto song_to_play = Server::find_song_by_id(id);
	cout << "Now playing: " << *song_to_play << endl;
	song_to_play->Play();
}

bool Library::check_if_continue_playing() {
	bool invalid_char = true;
	while (invalid_char) {
		cout << "Would you like to continue playing? y/n: ";
		char answer;
		cin >> answer; cout << endl;
		if (answer == 'n') {
			cout << "Stopped playing" << endl;
			invalid_char = false;
			return false;
		}
		else if (answer == 'y') {
			cout << "Continue playing" << endl;
			invalid_char = false;
			return true;
		}
		else {
			cout << "Invalid character entered, try again: " << endl;
		}
	}
}

// Play all of the songs in the library
void Library::PlayAll() {
	auto songs_to_play = Server::get_songs_sorted_by_alphabet();
	if (songs_to_play->size() == 0) {
		cout << "There are no songs in the library." << endl;
		return;
	}
	cout << "Playing all library songs: " << endl;
	multimap<string, Song*>::iterator it;
	for (it = songs_to_play->begin(); it != songs_to_play->end(); it++) {
		cout << "Now playing: " << *it->second << endl;
		int id = it->second->get_id();
		Play(id);
		if (check_if_continue_playing() == false)
			return;
	}
	cout << "Finished playing all songs in the library." << endl;
}

// Play all of the songs in the library, shuffled
void Library::PlayRandom() {
	auto songs_to_play = Server::get_songs_sorted_by_alphabet();
	if (songs_to_play->size() == 0) {
		cout << "There are no songs in the library." << endl;
		return;
	}
	// Create a vector of iterators to the elements in the multimap
	vector<multimap<string, Song*>::iterator> songs_vector;
	for (auto it = songs_to_play->begin(); it != songs_to_play->end(); ++it) {
		songs_vector.push_back(it);
	}
	// Shuffle the keys of the multimap randomly
	random_device rd;
	mt19937 generator(rd());
	shuffle(songs_vector.begin(), songs_vector.end(), generator);
	
	cout << "Playing all library songs, shuffled: " << endl; 

	// Play the songs of the multimap in the shuffled order
	for (auto const& song : songs_vector) {
		cout << "Now playing: " << *(song->second) << endl; 
		int id = song->second->get_id();
		Play(id);
		if (check_if_continue_playing() == false)
			return;
	}

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
	if (Server::Does_Song_Exist(file_path)) { //checks uniqueness
		cout << "Song was already added" << endl;
		return;
	}
	Server::Upload_Song(song_name, file_path, artist, album, genre, duration, release_Date);

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
	string duration, int release_Date)
{
	if (Server::Does_Episode_Exist(file_path)) { //checks uniqueness
		cout << "Episode was already added." << endl;
		return;
	}
	try
	{
		auto picked_podcast = Pick_Media(podcast_name, Server::get_podcasts_by_name());
		Server::Upload_Episode_To_Podcast(picked_podcast, episode_name, podcast_name, file_path, duration, release_Date);
	}
	catch (const std::exception&) //if caught, return to main.
	{
		return;
	}

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
		throw exception();
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

void Library::Delete_Song(string song_name)
{
	try
	{
		auto picked_song = Pick_Media(song_name, Server::get_songs_by_name());
		if (picked_song != nullptr) {
			Server::Permanent_Delete_Song(picked_song);
			return;
		}
		Print_Not_Found_By_Name_Error(song_name);
	}
	catch (const std::exception&)
	{
		return;
	}
}
void Library::Print_Not_Found_By_Name_Error(std::string& song_name)
{
	cout << song_name << " isn't present in the server." << endl;
}
void Library::Delete_Song(int id)
{
	try
	{
		Server::Permanent_Delete_Song(Server::find_song_by_id(id));
	}
	catch (const std::exception&)
	{
		Print_Not_Found_By_Id_Error(id, typeid(Song).name());
	}
}

// update by using song id
void Library::update_recent(int id) {
	Server::update_recently_played(id);
	auto recently_played = Server::get_recently_played();
	m_recent->clear_all_playlist();
	// Create an iterator for the list
	list<Song*>::iterator it;
	// Traverse through the list using the iterator
	for (it = recently_played->begin(); it != recently_played->end(); it++) {
		m_recent->add_song_to_playlist(*it); // add to recent the updated recently_played linked_list
	}
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
