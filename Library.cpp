#include "Library.h"
#include "Playlist.h"
#include "DailyMix.h"
#include "Favorites.h"
#include "Trash.h"
#include "Automatic_Playlist.h"
#include "Most_Played.h"
#include "Most_Recent.h"

#include <set>
#include <vector>
#include <algorithm>

#define user_playlists_file_name "playlists"


mt19937 Library::m_gen((random_device())()); //static so initialization happens only once

Library::Library() : m_server(new Server()), m_favorites(new Favorites(this, m_server)), m_deleted(new Trash(this, m_server)),
m_recent(new Most_Recent(this, m_server)), m_most_played(new Most_Played(this, m_server)), m_daily_mix(new DailyMix(this, m_server))
{
	m_playlists.emplace(m_favorites->get_name(), m_favorites);
	m_playlists.emplace(m_daily_mix->get_name(), m_daily_mix);
	m_playlists.emplace(m_recent->get_name(), m_recent);
	m_playlists.emplace(m_most_played->get_name(), m_most_played);
	m_playlists.emplace(m_deleted->get_name(), m_deleted);
	Begin_Serialization();
}

Library::~Library() {
	Begin_Deserialization();
	if (m_playlists.size()) {
		unordered_map<string, Playlist*>::iterator it;
		for (it = m_playlists.begin(); it != m_playlists.end(); it++) {
			delete it->second;
		}
	}
	m_user_playlist_names.clear();
	m_playlists.clear();
	delete m_server;
	//m_user_playlists.clear();
	//m_saved_playlist_names.clear();
}



void Library::PrintSong(int id)
{
	std::cout << "The song details are:" << std::endl;
	std::cout << m_server->find_song_by_id(id) << std::endl;
}

// print all songs with this name.
void Library::PrintSong(std::string song_name)
{
	auto songs = m_server->find_by_name(song_name);
	auto count = songs->count(song_name);
	if (count == 0) {
		std::cout << "Song wasn't found in the database!" << std::endl;
		return;
	}
	if (count > 1) {
		std::cout << "Few songs with the name: " << song_name << " were found: ";
	}
	unordered_multimap<string, Song*>::iterator it;
	int i = 1;
	for (it = songs->begin(); it != songs->end(); it++) {
		std::cout << "(" << i << "). " << *(it->second) << std::endl; // it->second contains Song*
		i++;
	}
}

// return true if playlist exist, false if not.
bool Library::check_if_playlist_exist(const std::string& playlist_name) {

	if (m_playlists.find(playlist_name) != m_playlists.end()) { // if not found, find method returns '.end()' element
		return true;
	}
	return false;
}

// return true if playlist can be edited, false if not (user can edit favorites) 
bool Library::check_if_user_playlist(const std::string& playlist_name) {

	if (m_user_playlist_names.find(playlist_name) != m_user_playlist_names.end()) {
		return true;
	}
	return false;

#pragma region Previous implementation
	//if (playlist_name == m_recent->get_name() || playlist_name == m_most_played->get_name()) { //todo: if daily mix is a different class, needs to change implementation
	//	return false;
	//}
	//return true;  
#pragma endregion

}

// create a new playlist
void Library::create_playlist(const std::string& playlist_name, bool prints_enabled) {

	if (check_if_playlist_exist(playlist_name)) {
		if (prints_enabled)
			std::cout << "A playlist with the name: " << playlist_name << " is already exists!" << std::endl;
		return;
	}
	if (prints_enabled)
		std::cout << "A playlist with the name: " << playlist_name << " was created!" << std::endl;
	m_user_playlist_names.insert(playlist_name);
	Playlist* new_playlist = new Playlist(playlist_name, this, m_server); // dynamic allocation (need to free memory)!
	m_playlists[playlist_name] = new_playlist;
}

//delete a playlist
void Library::delete_playlist(std::string playlist_name) {
	Playlist* playlist = get_playlist_by_name(playlist_name);
	if (playlist != nullptr) {
		playlist->clear_all_playlist();//the dialog is inside this method. if dialog is not needed, send flase to this method
		if (check_if_user_playlist(playlist_name)) {
			m_user_playlist_names.erase(playlist_name);
			m_playlists.erase(playlist_name);
			delete playlist;
			//std::cout << "Playlist Was Successfully Deleted!" << std::endl; //in dialog
			return;
		}
	}



#pragma region Previous implementation
	//// if m_deleted was selected, we need to empty this playlist. all songs need to be permanently deleted!
	//else if (playlist->get_name() == m_deleted->get_name()) { //todo: move to Deleted class
	//	multimap<string, Song*>::iterator it;
	//	for (it = playlist->get_songs().begin(); it != playlist->get_songs().end(); it++) {
	//		Server::Permanent_Delete_Song(it->second);

	//		//now when a song was permanently deleted, it needs to be removed from recents and most played playlists
	//		remove_from_most_recent(it->second->get_id()); //todo: maybe to move to Permanent_Delete_Song in Server
	//		// the update of most recent happans in remove_from_most_recent() method.
	//	}
	//	update_most_played(); // update most_played after deleting songs //todo: maybe to move to Permanent_Delete_Song in Server
	//	playlist->clear_all_playlist();
	//}
	//else
	//	std::cout << "This Playlist Cannot Be Deleted!" << std::endl;  
#pragma endregion

}

// print all exisiting playlists (names only)
void Library::PrintPL() { 

	int i = 1;
	std::cout << "List Of Playlists:" << std::endl;
	std::cout << "(" << i << "). " << m_favorites->get_name() << std::endl;
	i++;
	std::cout << "(" << i << "). " << m_most_played->get_name() << std::endl;
	i++;
	std::cout << "(" << i << "). " << m_recent->get_name() << std::endl;
	i++;
	set<string>::iterator it;
	for (it = m_user_playlist_names.begin(); it != m_user_playlist_names.end(); it++) { //m_user_playlist_names is in alphabet order
		std::cout << "(" << i << "). " << *it << std::endl;
		i++;
	}
	std::cout << "(" << i << "). " << m_deleted->get_name() << std::endl;
}


//Add a song by its ID to a playlist. Creates it if it doesn't exist
void Library::Add2PL(int id, const std::string& playlist_name, bool prints_enabled)
{
	auto song_to_add = m_server->find_song_by_id(id);
	Playlist* playlist = get_playlist_by_name(playlist_name, prints_enabled);
	if (playlist != nullptr) {
		playlist->add_song_to_playlist(song_to_add, prints_enabled); // we check if a song exist in playlist in add_song_to_playlist
		return;
	}
	#pragma region Previous implementation
	//if (check_if_user_playlist(playlist_name)) { //todo: check that goes to right method for each playlist
	//	m_saved_playlist_names.find(playlist_name)->second->add_song_to_playlist(song_to_add);
	//	
		//if (playlist_name == m_favorites->get_name()) {
		//	add_to_favorites(song_to_add);
		//}  


	//	return;
	//}
#pragma endregion
	//std::cout << "No such playlist was found!" << std::endl;
	//std::cout << "A playlist with the name: " << playlist_name << " was created!" << std::endl;
	create_playlist(playlist_name, prints_enabled); // creates the playlist automatically
	playlist = m_playlists[playlist_name];
	playlist->add_song_to_playlist(song_to_add, prints_enabled); // adds a song to the created playlist
}

// remove a song from the playlist by song's name.
void Library::RemoveFromPL(const std::string& song_name, const std::string& playlist_name, bool make_sure) {
	auto playlist = get_playlist_by_name(playlist_name, make_sure);
	if (playlist != nullptr) {
		playlist->remove_song_from_playlist(song_name, make_sure); //if its one song, we want to make sure if the user want to delete
	}
	#pragma region Previous Implementation
	//if (check_if_user_playlist(playlist_name) && m_deleted->get_name() != playlist_name) {
	//	if (check_if_playlist_exist(playlist_name) || playlist_name == m_favorites->get_name()) {
	//		auto playlist = m_user_playlists.find(playlist_name)->second;
	//		auto song_to_remove = playlist->get_song_by_name(song_name); // this method check if a song exist in the playlist
	//		if (song_to_remove == nullptr) {
	//			std::cout << "The song" << song_name << "is not in the playlist: " << playlist_name << "!" << std::endl;
	//			return;
	//		}
	//		else
	//			playlist->remove_song_from_playlist(song_to_remove, make_sure); // all the checking is in playlist 
	//	}
	//	else

	//}
	//else
	//	std::cout << "This Playlist Cannot Be Edited!" << std::endl;
#pragma endregion

}

////Asks the user which song he meant and updates the choosen one.
//void Library::Update_Episode(std::string episode_name, std::string new_name, std::string duration, int release_date)
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


bool Library::Are_All_Parameters_Empty(const std::string& param1,
	const std::string& param2, const std::string& param3 = "", const std::string& param4 = "", const std::string& param5 = "")
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
//Asks the user which song he meant and updates the choosen one.
void Library::Update_Song(std::string song_name,
	std::string new_name, std::string artist, std::string album, std::string genre, std::string duration, std::string release_date)
{
	if (Are_All_Parameters_Empty(artist, album, genre, duration, new_name) && release_date == "") {
		Print_No_Input_Parameters_Error();
		return;
	}
	try
	{

		auto picked_song = Pick_Media(song_name, m_server->get_songs_by_name());
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

//Find choosen song and updates according to the parameters recieved
void Library::Update_Song(int song_id, std::string new_name, std::string artist, std::string album, std::string genre, std::string duration, std::string release_date)
{
	try
	{
		//move updating to Song
		auto picked_song = Update_Media_By_Id(song_id, &Server::find_song_by_id, new_name, duration, release_date);
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

void Library::UpdatePodcast(std::string podcast_name, std::string new_name)
{
	if (new_name.empty()) {
		Print_No_Input_Parameters_Error();
		return;
	}
	if (m_server->Does_Podcast_Exist(new_name)) {
		Print_Media_Exists_Error(new_name, typeid(Podcast).name());
		return;
	}
	try
	{
		auto picked_podcast = m_server->find_podcast_by_name(podcast_name); //throws exception
		if (picked_podcast != nullptr) {
			picked_podcast->Set_Podcast_Name(new_name); //not empty!
			return;
		}
		Print_Not_Found_By_Name_Error(podcast_name);
	}
	catch (const std::exception&)
	{
		return;
	}
}

void Library::UpdateEpisode(int episode_id, std::string new_name, std::string duration, std::string release_date)
{
	if (m_server->Does_Episode_Exist(new_name)) {
		Print_Media_Exists_Error(new_name, typeid(Episode).name());
		return;
	}
	try
	{
		Update_Media_By_Id(episode_id, &Server::find_episode_by_id, new_name, duration, release_date);
	}
	catch (const std::exception&)
	{
		Print_Not_Found_By_Id_Error(episode_id, typeid(Episode).name());
	}

}

void Library::Print_Media_Exists_Error(std::string& new_name, const std::string& media_type)
{
	std::cout << media_type << " with name " << new_name << " already exists, please choose a unique name" << std::endl;
}

template <class T>
T* Library::Update_Media_By_Id(int media_id, T* (Server::* methodPtr)(int), std::string new_name, std::string duration, std::string release_date)
{
	//Here if all paramters are empty it does nothing
	auto picked_media = (m_server->*methodPtr)(media_id);
	if (!new_name.empty()) {
		picked_media->set_name(new_name);
	}
	if (!duration.empty()) { //Might throw exception, has to be caught by the main loop!
		picked_media->set_duration(duration);
	}
	if (release_date != "") {
		picked_media->set_release_date(Date(release_date));
	}
	return picked_media;
}


void Library::Print_Not_Found_By_Id_Error(int item_id, std::string item_type)
{
	std::cout << item_type << " with id " << item_id << " doesn't exist in server. Please add it first" << std::endl;
}

void Library::Print_No_Input_Parameters_Error()
{
	std::cout << "No paramters to updated" << std::endl;
}

//this method plays a song, updates most_played, most_recent stats 
//can be found in Play(song_name), Play(id). 
//since PlayAll and PlayAllRandom uses Play(id), we don't need to worry about the update of most_played, most_recent
void Library::play_song(Song* song) {
	//std::cout << "Now playing: " << *song << std::endl; //Moved this inside Play()!
	song->Play(); // here plays_counter updates
	//m_recent->Update_Most_Recent();
	m_recent->Add_To_Most_Recent(song->get_id());
	m_most_played->Update_Most_Played();
}

void Library::Play_Podcast(std::string podcast_name)
{
	try
	{
		m_server->find_podcast_by_name(podcast_name)->Play(); //throws exception if not found
	}
	catch (const std::exception&)
	{
		Print_Not_Found_By_Name_Error(podcast_name);
	}
}

void Library::Play(std::string song_name)
{
	auto song = m_server->find_by_name(song_name);
	auto count = song->count(song_name);
	PrintSong(song_name);
	if (count == 0) {
		std::cout << "Song could not be played." << std::endl;
		return;
	}
	auto song_to_play = Pick_Media(song_name, song);
	if (song_to_play == nullptr) {
		std::cout << "You didn't choose a specific song to play." << std::endl;
		return;
	}
	play_song(song_to_play);

}

void Library::Play(int id)
{
	auto song_to_play = m_server->find_song_by_id(id);
	play_song(song_to_play);
}

bool Library::check_if_continue_playing() {
	std::string prompt = "Would you like to continue playing? y/n: ";
	std::string reject_message = "Stopped playing";
	std::string accept_message = "Continue playing";
	if (Utilities::user_prompts_and_dialog(prompt, reject_message, accept_message)) {
		return true;
	}
	return false;
}

// Play all of the songs given
void Library::PlayAll(std::vector<Song*>* songs_to_play, const std::string& message, bool shuffle, bool delete_ptr) {
	if (songs_to_play->empty()) {
		std::cout << "There are no songs to play." << std::endl;
	}
	else {
		std::cout << message << std::endl;
		bool first_play = true;
		if (shuffle) {
			std::shuffle(songs_to_play->begin(), songs_to_play->end(), m_gen);
		}
		for (auto it : *songs_to_play) {
			if (!first_play && !check_if_continue_playing()) { //asks after first play
				break;
			}
			//int id = it->second->get_id();
			play_song(it);
			first_play = false;
		}
		std::cout << "Finished playing all songs" << std::endl;
	}
	if (delete_ptr) {
		delete songs_to_play;
	}
}

//Plays all songs in library unshuffled
void Library::PlayAll() { 
	auto songs_to_play = m_server->get_songs_sorted_by_alphabet();
	const std::string message = "Playing all songs in the library: ";
	PlayAll(Utilities::Values(songs_to_play), message, false, true);
	//for (auto& pair : *songs_to_play) {
	//	songs_vector.push_back(pair.second);
	//}
	//Utilities::Values(nullptr);
	#pragma region Previous implementation
	//if (songs_to_play->size() == 0) {
	//	std::cout << "There are no songs in the library." << std::endl;
	//	return;
	//}
	//std::cout << "Playing all library songs: " << std::endl;
	//multimap<string, Song*>::iterator it;
	//for (it = songs_to_play->begin(); it != songs_to_play->end(); it++) {
	//	std::cout << "Now playing: " << *it->second << std::endl;
	//	int id = it->second->get_id();
	//	Play(id);
	//	if (check_if_continue_playing() == false)
	//		return;
	//}
	//std::cout << "Finished playing all songs in the library." << std::endl;  
	#pragma endregion

}

// Play all of the songs in the library, shuffled
void Library::PlayRandom() { //todo: remove this
	auto songs_to_play = m_server->get_songs_sorted_by_alphabet();
	if (songs_to_play->size() == 0) {
		std::cout << "There are no songs in the library." << std::endl;
		return;
	}
	// Create a vector of iterators to the elements in the multimap
	std::vector<std::multimap<string, Song*>::iterator> songs_vector;
	for (auto it = songs_to_play->begin(); it != songs_to_play->end(); ++it) {
		songs_vector.push_back(it);
	}
	// Shuffle the keys of the multimap randomly
	std::random_device rd;
	std::mt19937 generator(rd());
	shuffle(songs_vector.begin(), songs_vector.end(), generator);

	std::cout << "Playing all library songs, shuffled: " << std::endl;

	// Play the songs of the multimap in the shuffled order
	for (auto const& song : songs_vector) {
		std::cout << "Now playing: " << *(song->second) << std::endl;
		int id = song->second->get_id();
		Play(id);
		if (check_if_continue_playing() == false)
			return;
	}

}

//return the playlist that needs to be played. return nullptr if no playlist was found
Playlist* Library::get_playlist_by_name(std::string playlist_name, bool prints_enable ) {
	if (check_if_playlist_exist(playlist_name)) {
		auto playlist = m_playlists.find(playlist_name);
		return playlist->second;
	}
	//auto playlist = m_saved_playlist_names.find(playlist_name);
	//if (playlist != m_saved_playlist_names.end()) {
	//	return playlist->second;
	//}
	if (prints_enable)
	{
		std::cout << "Playlist " << playlist_name << " doesn't exist!" << std::endl;
	}
	return nullptr;
}

// print a specific playlist data
void Library::PrintPlaylist(std::string playlist_name) {
	auto playlist_to_play = get_playlist_by_name(playlist_name);
	if (playlist_to_play != nullptr) {
		std::cout << playlist_to_play;
	std:cout << endl;
	}
}

//play a playlist by its name 
void Library::PlayPlaylist(std::string playlist_name) {
	auto playlist_to_play = get_playlist_by_name(playlist_name);
	if (playlist_to_play != nullptr) {
		playlist_to_play->Play(false);
	}
}

//play a playlist shuffled by its name 
void Library::PlayPlaylistShuffled(std::string playlist_name) {
	auto playlist_to_play = get_playlist_by_name(playlist_name);
	if (playlist_to_play != nullptr)
		playlist_to_play->Play(true);

}


//called from the interface ("main")
void Library::Add_Song(std::string song_name, std::string file_path, std::string artist = "",
	std::string album = "", std::string genre = "", std::string duration = "", std::string release_date = "")
{
	if (m_server->Does_Song_Exist(file_path)) { //checks uniqueness
		std::cout << "Song was already added" << std::endl;
		return;
	}
	m_server->Upload_Song(song_name, file_path, artist, album, genre, duration, release_date);

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
	//	std::cout << "No songs named " << song_name << " currently in the server. Please download it first and place it in the folder" << std::endl;
	//	return;
	//}
	//case 1: {
	//	m_songs_by_name->insert(*first_wanted_song); //or *last doesn't matter
	//	return;
	//}
	//default:
	//	break;
	//}
	//std::cout << "More than one song named " << song_name << ", choose which one you want to add" << std::endl;
	//m_songs_by_name->insert(Pick_Song(number_of_songs, &first_wanted_song, &last_wanted_song));  
#pragma endregion
}

void Library::Add_Podcast_Episode(std::string episode_name, std::string podcast_name,
	std::string file_path, std::string duration, std::string release_Date)
{
	if (m_server->Does_Episode_Exist(episode_name)) { //checks uniqueness
		std::cout << "Episode was already added." << std::endl;
		return;
	}
	Podcast* picked_podcast;
	try
	{
		picked_podcast = m_server->find_podcast_by_name(podcast_name);
	}
	catch (const std::exception&)
	{
		picked_podcast = nullptr;
	}
	m_server->Upload_Episode_To_Podcast(picked_podcast, episode_name, podcast_name, file_path, duration, release_Date);

}

void Library::Begin_Serialization()
{
	//Server::Restore_Songs();
	//for (auto& special_playlist : m_playlists) {
	//	special_playlist.second->restore_playlist();
	//}
	ifstream read_user_playlists("c:\\temp\\playlists.dat", ios::in);
	if (!Utilities::Is_File_Valid(read_user_playlists)) {
		return;
	}
	while (!Utilities::Is_End_Of_File_Or_Empty(read_user_playlists)) {
		std::string playlist_name;
		int song_id;
		read_user_playlists >> playlist_name >> song_id;
		vector<string*> params = { &playlist_name };
		Utilities::Replace_All(params, true);
		Add2PL(song_id, playlist_name, false);// false for disable prints
		if (Utilities::Is_End_Of_File(read_user_playlists)) {
			break;
		}
	}
	/*m_recent->Update_Most_Recent();
	m_most_played->Update_Most_Played();*/
}

void Library::Begin_Deserialization()
{
	bool is_started_writing_user_playlists = false;
	ios_base::openmode mode = ios::out;
	for (auto& playlist_pair : m_playlists) {
		std::string file_name = playlist_pair.first;
		if (check_if_user_playlist(file_name)) {
			file_name = user_playlists_file_name;
			is_started_writing_user_playlists = true;
		}
		playlist_pair.second->save_playlist(file_name, mode);
		if (is_started_writing_user_playlists) {
			mode = ios::app;
		}
	}

	//std::unordered_map<string, Playlist*>::iterator itr;
	//for (itr = m_playlists.begin(); itr != m_playlists.end(); itr++)
	//{
	//	std::string playlist_name = itr->first;
	//	Playlist* current_playlist = itr->second;
	//	vector<string*> params = { &playlist_name };
	//	Utilities::Replace_All(params, false);
	//	for (auto& song : current_playlist->get_songs()) {
	//		write_playlists << " " << playlist_name << " " << song.second->get_id() << std::endl;
	//	}
	//}


}

mt19937 Library::Get_Gen()
{
	return m_gen;
}



Song* Library::Pick_Media(std::string media_name, unordered_multimap<string, Song*>* collection_to_search) {
	auto filtered_media = collection_to_search->equal_range(media_name);
	int number_of_media_items = collection_to_search->count(media_name); //O(log n), distance is O(n). overall doesn't matter
	if (number_of_media_items == 0) {
		return nullptr;
	}
	if (number_of_media_items == 1) {
		return filtered_media.first->second;
	}
	int i = 1;//can't define 2 variables of different type in for (you can but it's less readable)
	for (auto& iterator = filtered_media.first; iterator != filtered_media.second; ++iterator, ++i) {//loops over all podcasts
		std::cout << i << " - " << iterator->second << std::endl;//prints numbered songs
	}
	int answer;
	do {
		std::cout << "Please choose a number between 1 and " << number_of_media_items << " (0 or anything else to cancel): ";
		cin >> answer; //inputs 0 in case of strings
	} while (answer < 0 || answer > number_of_media_items);
	if (answer == 0) {
		throw exception();
	}
	auto& iterator = filtered_media.first;
	advance(iterator, answer - 1);
	return iterator->second;
}

//Adds song to m_deleted playlist
// method removes a specific song, used by the two Delete_Song methods
void Library::delete_song(Song* song_to_delete) {
	auto trash_name = m_deleted->get_name();
	std::string prompt = "Are you sure that you want to move this song to trash? y/n: ";
	std::string reject_message = "The song wasn't added to " + trash_name + "!";
	std::string accept_message = "";
	if (!Utilities::user_prompts_and_dialog(prompt, reject_message, accept_message)) {
		return;
	}
	auto playlist_appearances = song_to_delete->get_playlist_appearances(); 
	if (playlist_appearances->size() > 0) {
		unordered_set<string>::iterator it;
		std::unordered_set<std::string> copy(*playlist_appearances);
		for (it = copy.begin(); it != copy.end(); it++) {
			RemoveFromPL(song_to_delete->get_name(), *it, false);// don't need to check with user here
			// false- don't need to make sure with the user if he wants to delete from this playlist
		}
	}
	// a song will stay in the playlists: most played, recents until it will be permanently deleted
	m_deleted->add_to_trash(song_to_delete); // we do want to check with the user if he's sure that he wants to delete song 
	//std::cout << "Song was successfully removed from the library and all of it's playlists!" << std::endl;
}

//delete song by id
void Library::Delete_Song(int id)
{
	try
	{
		auto song_to_delete = m_server->find_song_by_id(id);
		delete_song(song_to_delete);
	}
	catch (const std::exception&)
	{
		Print_Not_Found_By_Id_Error(id, typeid(Song).name());
	}
}

//delete song by name
void Library::Delete_Song(std::string song_name)
{
	try
	{
		auto song_to_delete = Pick_Media(song_name, m_server->get_songs_by_name());
		if (song_to_delete != nullptr) {
			delete_song(song_to_delete);
			//Server::Permanent_Delete_Song(picked_song);
			return;
		}
		Print_Not_Found_By_Name_Error(song_name);
	}
	catch (const std::exception&)
	{
		return;
	}
}

void Library::Delete_Episode(int id)
{
	try
	{
		//todo: add check before delete - from Utilities
		m_server->Permanent_Delete_Podcast_Episode(m_server->find_episode_by_id(id));
	}
	catch (const std::exception&)
	{
		Print_Not_Found_By_Id_Error(id, typeid(Song).name());
	}
}

void Library::Delete_Episode(std::string episode_name)
{
	try
	{
		m_server->Permanent_Delete_Podcast_Episode(m_server->find_episode_by_name(episode_name)); //if episode doesn't exist, throws exception
	}
	catch (const std::exception&)
	{
		Print_Not_Found_By_Name_Error(episode_name);
	}
}

void Library::Delete_Podcast(std::string podcast_name)
{
	try
	{
		m_server->Permanent_Delete_Podcast(m_server->find_podcast_by_name(podcast_name)); //if podcast doesn't exist, throws exception
	}
	catch (const std::exception&)
	{
		Print_Not_Found_By_Name_Error(podcast_name);
	}
}

void Library::Print_Not_Found_By_Name_Error(std::string& media_name)
{
	std::cout << media_name << " isn't present in the server." << std::endl;
}

//void Library::add_to_most_recent(int id) {
//	Server::add_to_recently_played(id);
//	update_most_recent();
//}
//
//void Library::update_most_recent() {
//	m_recent->Update_Most_Recent();
//	//auto recently_played = Server::get_recently_played();
//	//int recents_size = recently_played->size();
//	//m_recent->clear_all_playlist();
//	//int minimum = min(recents_size, max_most_played); // using c++ algorithm
//	//// Create an iterator for the list
//	//auto it = recently_played->begin();
//	//// Traverse through the list using the iterator
//	//for (int i = 0; i < minimum; i++) {
//	//	m_recent->add_song_to_playlist(*it); // add to recent the updated recently_played linked_list
//	//	it++;
//	//}
//}
void Library::remove_from_most_recent(int id) {
	m_recent->Remove_From_Most_Recent(id);
	//auto song_to_remove = Server::find_song_by_id(id);
	//m_recent->remove_song_from_playlist(song_to_remove, false); // removes from playlist, don't make sure to delete songs from PL
	//Server::remove_from_recently_played(id); // removes from the data structure
	//update_most_recent();
}

void Library::remove_from_daily_mix(Song* song) {
	m_daily_mix->remove_song_from_playlist(song);
}


DailyMix* Library::Get_DailyMix() {
	return m_daily_mix;
}


void Library::update_most_played() {

	m_most_played->Update_Most_Played();
	//Server::update_most_played_songs();
	//auto most_played = Server::get_most_played();
	//int most_played_size = most_played->size();
	//m_most_played->clear_all_playlist();
	//int minimum = min(most_played_size, max_most_played); // using c++ algorithm
	//std::multimap<int, Song*>::iterator it = most_played->end();
	//for (int i = 0; i < minimum; i++) {
	//	it--;
	//	m_most_played->add_song_to_playlist(it->second);
	//}
}


ostream& Library::Print(ostream& os, int begin, int end) const
{
	auto itr = m_server->get_songs_by_name()->begin();
	advance(itr, begin);//if begin!=0, inc itr untill reached begin
	//for (int i = 0; i < begin && itr != Server::get_songs_by_name()->end(); i++, itr++) {} 
	for (int i = begin; i < end && itr != m_server->get_songs_by_name()->end(); i++, itr++)
	{
		std::cout << *itr->second << std::endl;
	}
	if (itr == m_server->get_songs_by_name()->end()) {
		os << "No more songs :(" << std::endl;
	}
	return os;
}

ostream& operator<<(ostream& os, const Library& lib)
{
	return lib.Print(os, 0, lib.num_of_songs_to_print);
}

void Library::Example_Func_For_MethodMap() {}
void Library::Example_Func_For_MethodMap(std::string temp) {

}

void Library::Example2_Func_For_MethodMap(std::string temp) {

}

Favorites* Library::Get_Favorites() {
	return m_favorites;
}

Most_Played* Library::Get_Most_Played() {
	return m_most_played;
}

Most_Recent* Library::Get_Most_Recent() {
	return m_recent;
}

Trash* Library::Get_Deleted() {
	return m_deleted;
}

Server* Library::Get_Server() {
	return m_server;
}

//************************************************************************************************************************
//********************************************* Methods That May Not Be Needed********************************************
//************************************************************************************************************************ 

////Asks the user which song he meant and updates the choosen one.
//void Library::Update_Episode(std::string episode_name, std::string new_name, std::string duration, int release_date)
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

//// double checks with the user if the song should be deleted, if yes- removes the song.
//void Library::ask_user_to_remove_song(Song* song, Playlist* playlist) {
//	std::cout << "You chose to remove the song: " << std::endl;
//	PrintSong(song->get_id());
//	std::cout << "Are you sure you want to remove this song from: " << playlist->get_name() << "?" << std::endl;
//	std::cout << "Press y/n: ";
//	char answer;
//	cin >> answer;
//	std::cout << std::endl;
//	if (answer == 'y') {
//		playlist->remove_song_from_playlist(song->get_name());
//		std::cout << "Song Was Successfully Removed From Playlist!" << std::endl;
//	}
//	else
//		std::cout << "The Song Wasn't Removed!" << std::endl;
//}

//Library::Library() : m_songs_by_name(Server::get_songs_by_name()), m_playlists()
//{
//	// = new set<Song*>*; //is initialization needed here?
//	//m_playlists_map= &std::map<string, Playlist*>(m_playlists->get_user_playlists()->begin(), m_playlists->get_user_playlists()->end()); //map of playlists
//}

//void Library::Are_All_Parameters_Empty(std::string& artist, std::string& album, std::string& genre, std::string& duration)
//{
//	std::vector<std::string> params = { artist, album, genre,duration };
//	bool allEmpty = std::all_of(std::begin(params),
//		std::end(params),
//		[](const std::string& str) {
//			return str.empty();
//		});
//}
#pragma region Previous implementation of pick_podcast and pick_song

//Podcast* Library::Pick_Podcast(std::string podcast_name) {
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
//		std::cout << i << " - " << iterator->second << std::endl;//prints numbered songs
//	}
//	int answer;
//	do {
//		std::cout << "Please choose a number between 1 and " << number_of_songs << " (0 to cancel): ";
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
//Song* Library::Pick_Song(std::string song_name) {
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
//		std::cout << i << " - " << iterator->second << std::endl;//prints numbered songs
//	}
//	int answer;
//	do {
//		std::cout << "Please choose a number between 1 and " << number_of_songs << " (0 to cancel): ";
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

#pragma region Old implementation of RemoveFromPL
//void Library::RemoveFromPL(std::string& song_name, const std::string& playlist_name)
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
//		std::cout << "No songs named " << song_name << " currently in " << playlist_name << "." << std::endl;
//		return;
//	}
//	case 1: {
//		m_playlists_map->at(playlist_name)->remove_song(*first_wanted_song); //or *last doesn't matter
//		return;
//	}
//	default:
//		break;
//	}
//	std::cout << "More than one song named " << song_name << ", choose which one you want to remove" << std::endl;
//	//m_playlists_map->at(playlist_name)->remove_song(Pick_Song( )); //removes the selected song
//}  
#pragma endregion