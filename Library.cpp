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
	m_playlists.emplace(m_favorites->get_name(), m_favorites); //Adds each playlist to m_playlists
	m_playlists.emplace(m_daily_mix->get_name(), m_daily_mix);
	m_playlists.emplace(m_recent->get_name(), m_recent);
	m_playlists.emplace(m_most_played->get_name(), m_most_played);
	m_playlists.emplace(m_deleted->get_name(), m_deleted);
	Begin_Deserialization();
}

Library::~Library() {
	Begin_Serialization();
	if (m_playlists.size()) {
		unordered_map<string, Playlist*>::iterator it;
		for (it = m_playlists.begin(); it != m_playlists.end(); it++) {
			delete it->second;
		}
	}
	m_user_playlist_names.clear();
	m_playlists.clear();
	delete m_server;
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
			return;
		}
	}
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

}

//Checks if all strings are empty
bool Library::Are_All_Parameters_Empty(const std::string& param1,
	const std::string& param2, const std::string& param3 = "", const std::string& param4 = "", const std::string& param5 = "")
{
	return param1.empty() && param2.empty() && param3.empty() && param4.empty();
}


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
		auto picked_song = Pick_Media(song_name, m_server->get_songs_by_name()); //Asks user which song he meant
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
		Print_Media_Exists_Error(new_name, typeid(Podcast).name()); //Don't allow to use a name that is already taken
		return;
	}
	try
	{
		auto picked_podcast = m_server->find_podcast_by_name(podcast_name); //throws exception if not found
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
		Print_Media_Exists_Error(new_name, typeid(Episode).name()); //Don't allow to use a name that is already taken
		return;
	}
	try
	{
		Update_Media_By_Id(episode_id, &Server::find_episode_by_id, new_name, duration, release_date); //Updates the episode
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
	//Only updates non-empty parameters
	auto picked_media = (m_server->*methodPtr)(media_id);
	if (!new_name.empty()) {
		picked_media->set_name(new_name);
	}
	if (!duration.empty()) {
		picked_media->set_duration(duration);
	}
	if (release_date != "") {
		picked_media->set_release_date(release_date);
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
	song->Play(); // here plays_counter updates
	m_recent->Add_To_Most_Recent(song->get_id());
	m_most_played->Update_Automatic_Playlist();
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
	auto songs = m_server->find_by_name(song_name);
	auto count = songs->count(song_name);
	PrintSong(song_name);
	try
	{
		auto song_to_play = Pick_Media(song_name, songs);
		if (song_to_play == nullptr) {
			std::cout << "You didn't choose a specific song to play." << std::endl;
			return;
		}
		play_song(song_to_play);
	}
	catch (const std::exception&)
	{
		return;
	}
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
		for (auto it : *songs_to_play) { //plays all songs
			if (!first_play && !check_if_continue_playing()) { //asks after first play
				break;
			}
			play_song(it); 
			first_play = false;
		}
		std::cout << "Finished playing all songs" << std::endl;
	}
	if (delete_ptr) { //Sometimes songs_to_play points to a local variable so deletion is not always needed
		delete songs_to_play;
	}
}

//Plays all songs in library unshuffled
void Library::PlayAll(bool shuffle) { 
	auto songs_to_play = m_server->get_songs_sorted_by_alphabet();
	const std::string message = "Playing all songs in the library: ";
	PlayAll(Utilities::Values(songs_to_play), message, shuffle, true);
}

//return the playlist that needs to be played. return nullptr if no playlist was found
Playlist* Library::get_playlist_by_name(std::string playlist_name, bool prints_enable ) {
	if (check_if_playlist_exist(playlist_name)) {
		auto playlist = m_playlists.find(playlist_name);
		return playlist->second;
	}
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


//Adds a song to the library with the parameters given
void Library::Add_Song(std::string song_name, std::string file_path, std::string artist = "",
	std::string album = "", std::string genre = "", std::string duration = "", std::string release_date = "")
{
	if (m_server->Does_Song_Exist(file_path)) { //checks uniqueness by it's path before creating it
		std::cout << "Song was already added" << std::endl;
		return;
	}
	m_server->Upload_Song(song_name, file_path, artist, album, genre, duration, release_date); //memory allocation happens here

}

//Adds an episode to the specified podcast. If podcast doesn't exists it creates it.
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
		picked_podcast = m_server->find_podcast_by_name(podcast_name); //Tries to find the podcast
	}
	catch (const std::exception&)
	{
		picked_podcast = nullptr; //if not found, pass a nullptr and continue
	}
	m_server->Upload_Episode_To_Podcast(picked_podcast, episode_name, podcast_name, file_path, duration, release_Date);

}


//Reads the data of playlists from the files to the data structures.
void Library::Begin_Deserialization()
{
	std::string address = playlists_file_name;
	Utilities::Format_Address(address);
	ifstream read_user_playlists(address, ios::in);
	if (!Utilities::Is_File_Valid(read_user_playlists, playlists_file_name)) {
		return;
	}
	while (!Utilities::Is_End_Of_File_Or_Empty(read_user_playlists)) {
		std::string playlist_name;
		int song_id;
		read_user_playlists >> playlist_name >> song_id; //reads from the file to variables
		vector<string*> params = { &playlist_name };
		Utilities::Replace_All(params, true); //formats the name
		Add2PL(song_id, playlist_name, false);// false for disable prints
		if (Utilities::Is_End_Of_File(read_user_playlists)) {
			break;
		}
	}
}
//Writes to the playlist's file the playlists' data
void Library::Begin_Serialization()
{
	bool is_started_writing_user_playlists = false; //When writing user playslists, we want to append data from the 2nd playlist onwards
	ios_base::openmode mode = ios::out; //Because all user playlists' are in the same file
	for (auto& playlist_pair : m_playlists) { //for other playlists, we're always overriding the entire file
		std::string file_name = playlist_pair.first; //The loop goes over each playlist.
		if (check_if_user_playlist(file_name)) {
			file_name = user_playlists_file_name;
			is_started_writing_user_playlists = true;
		}
		playlist_pair.second->save_playlist(file_name, mode);
		if (is_started_writing_user_playlists) { 
			mode = ios::app;
		}
	}
}

mt19937 Library::Get_Gen() //To use it in DailyMix class
{
	return m_gen;
}
// This function searches for a song in a collection of media by name
// and allows the user to choose from a list of matching songs if there are multiple matches.
Song* Library::Pick_Media(std::string media_name, unordered_multimap<string, Song*>* collection_to_search) {
	// Find all songs with the given name in the collection
	auto filtered_media = collection_to_search->equal_range(media_name); 
	// Count how many songs were found
	int number_of_media_items = collection_to_search->count(media_name); 
	// If no songs were found, return nullptr
	if (number_of_media_items == 0) {
		return nullptr;
	}
	// If only one song was found, return it immediately
	if (number_of_media_items == 1) {
		return filtered_media.first->second;
	}
	// If multiple songs were found, print a numbered list of them for the user to choose from
	int i = 1;// Initialize a counter for the song numbers
	for (auto& iterator = filtered_media.first; iterator != filtered_media.second; ++iterator, ++i) {
		//loops over all songs with the given name
		std::cout << i << " - " << iterator->second << std::endl;//prints numbered songs
	}
	// Prompt the user to choose a song from the list by entering a number
	int answer;
	do {

		std::cout << "Please choose a number between 1 and " << number_of_media_items << " (0 or anything else to cancel): ";
		// Take user input
		cin >> answer; //inputs 0 in case of strings
	} while (answer < 0 || answer > number_of_media_items);// Keep prompting until cancellation/ valid input
	if (answer == 0) {// If the user chose to cancel, throw an exception
		throw exception();
	}
	// Otherwise, find the selected song in the list and return a pointer to it
	auto& iterator = filtered_media.first;
	advance(iterator, answer - 1); //Moved the iterator further as needed
	return iterator->second;
}

//Adds song to m_deleted playlist
// method removes a specific song, used by the two Delete_Song methods
void Library::delete_song(Song* song_to_delete) {
	auto trash_name = m_deleted->get_name();
	std::string prompt = "Are you sure that you want to move this song to trash? y/n: ";
	std::string reject_message = "The song wasn't added to " + trash_name + "!";
	std::string accept_message = "";
	if (!Utilities::user_prompts_and_dialog(prompt, reject_message, accept_message)) { //Asks the user
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
			return;
		}
		Print_Not_Found_By_Name_Error(song_name);
	}
	catch (const std::exception&)
	{
		return;
	}
}

//Delete episode by ID
void Library::Delete_Episode(int id)
{
	try
	{
		m_server->Permanent_Delete_Podcast_Episode(m_server->find_episode_by_id(id), true);
	}
	catch (const std::exception&)
	{
		Print_Not_Found_By_Id_Error(id, typeid(Song).name());
	}
}

//Delete episode by name (unqiue)
void Library::Delete_Episode(std::string episode_name)
{
	try
	{
		m_server->Permanent_Delete_Podcast_Episode(m_server->find_episode_by_name(episode_name), true); //if episode doesn't exist, throws exception
	}
	catch (const std::exception&)
	{
		Print_Not_Found_By_Name_Error(episode_name);
	}
}
//Delete podcast by name (unqiue)
void Library::Delete_Podcast(std::string podcast_name)
{
	try
	{
		m_server->Permanent_Delete_Podcast(m_server->find_podcast_by_name(podcast_name), true); //if podcast doesn't exist, throws exception
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

void Library::update_most_recent() {
	m_recent->Update_Automatic_Playlist();
}

void Library::remove_from_daily_mix(Song* song) {
	m_daily_mix->remove_song_from_playlist(song);
}


DailyMix* Library::Get_DailyMix() {
	return m_daily_mix;
}


void Library::update_most_played() {
	m_most_played->Update_Automatic_Playlist();
}


ostream& Library::Print(ostream& os, int begin, int end) const
{
	if (begin > m_server->get_songs_by_name()->size()) { // check if the begin num is greater than the num of songs in server
		os << "No more songs." << std::endl;
		return os;
	}
	auto itr = m_server->get_songs_by_name()->begin();
	advance(itr, begin);//if begin!=0, move itr untill reached begin
	for (int i = begin; i < end && itr != m_server->get_songs_by_name()->end(); i++, itr++)
	{
		std::cout << *itr->second << std::endl;
	}
	if (itr == m_server->get_songs_by_name()->end()) {
		os << "No more songs." << std::endl;
	}
	return os;
}

ostream& operator<<(ostream& os, const Library& lib)
{
	return lib.Print(os, 0, lib.num_of_songs_to_print);
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