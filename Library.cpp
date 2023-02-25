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

Library::Library() : m_server(new Server()), m_favorites(new Favorites(this, m_server)), m_deleted(new Trash(this, m_server)), 
m_recent(new Most_Recent(this, m_server)), m_most_played(new Most_Played(this, m_server)), m_daily_mix(new DailyMix(this, m_server)) 
/* :  m_favorites(new Favorites(this)), m_deleted(new Trash(this)), m_recent(new Most_Recent(this))
		, m_most_played(new Most_Played(this)), m_daily_mix(new DailyMix(this))*/
{
	/*delete m_daily_mix;*/
	//m_favorites = new Favorites(this);
	//m_deleted = new Trash(this);
	//m_recent = new Most_Recent(this);
	//m_most_played = new Most_Played(this);
	//m_daily_mix = new DailyMix(this);
	m_playlists.emplace(m_favorites->get_name(), m_favorites);
	m_playlists.emplace(m_daily_mix->get_name(), m_daily_mix);
	m_playlists.emplace(m_recent->get_name(), m_recent);
	m_playlists.emplace(m_most_played->get_name(), m_most_played);
	m_playlists.emplace(m_deleted->get_name(), m_deleted);
	//Begin_Serialization();
}

//todo: add getters for each playlists
Library::~Library() {
	//Begin_Deserialization();
	//delete m_deleted;
	//delete m_favorites;
	//delete m_most_played;
	//delete m_recent;
	
	
	if (m_playlists.size()) {
		unordered_map<string, Playlist*>::iterator it;
		for (it = m_playlists.begin(); it != m_playlists.end(); it++) {
			Playlist* playlist_to_delete = it->second;
			if (playlist_to_delete == m_daily_mix) {
				delete m_daily_mix;
			}
			else {
				delete playlist_to_delete;
			}
		}
	}
	delete m_server;
	//m_user_playlists.clear();
	m_user_playlist_names.clear();
	m_playlists.clear();
	//m_saved_playlist_names.clear();
}

void Library::PrintSong(int id)
{
	cout << "The song details are:" << endl;
	cout << m_server->find_song_by_id(id) << endl;
}

// print all songs with this name.
void Library::PrintSong(string song_name)
{
	auto songs = m_server->find_by_name(song_name);
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
bool Library::check_if_playlist_exist(const string& playlist_name) {

	if (m_playlists.find(playlist_name) != m_playlists.end()) { // if not found, find method returns '.end()' element
		return true;
	}
	return false;
}

// return true if playlist can be edited, false if not (user can edit favorites) 
bool Library::check_if_user_playlist(const string& playlist_name) {

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
void Library::create_playlist(const string& playlist_name, bool prints_enabled) {

	if (check_if_playlist_exist(playlist_name)) {
		if (prints_enabled)
			cout << "A playlist with the name: " << playlist_name << " is already exists!" << endl; // todo: change to try, throw
		return;
	}
	if (prints_enabled)
		cout << "A playlist with the name: " << playlist_name << " was created!" << endl;
	m_user_playlist_names.insert(playlist_name);
	Playlist* new_playlist = new Playlist(playlist_name, this, m_server); // need to delete!
	m_playlists[playlist_name] = new_playlist;
}

//delete a playlist
void Library::delete_playlist(string playlist_name) {
	Playlist* playlist = get_playlist_by_name(playlist_name);
	if (playlist != nullptr) {
		playlist->clear_all_playlist();//the dialog is inside this method. if dialog is not needed, send flase to this method
		if (check_if_user_playlist(playlist_name)) {
			m_user_playlist_names.erase(playlist_name);
			m_playlists.erase(playlist_name);
			delete playlist;
			//cout << "Playlist Was Successfully Deleted!" << endl; //in dialog
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
	//	cout << "This Playlist Cannot Be Deleted!" << endl;  
	#pragma endregion

}

// print all exisiting playlists (names only)
void Library::PrintPL() { //todo: make overload?

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
void Library::Add2PL(int id, const string& playlist_name, bool prints_enabled) //todo: COMPLETED! parameter for prints was added
{
	auto song_to_add = m_server->find_song_by_id(id);
	Playlist* playlist = get_playlist_by_name(playlist_name);
	if (playlist != nullptr) {
		playlist->add_song_to_playlist(song_to_add); // we check if a song exist in playlist in add_song_to_playlist
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
	//cout << "No such playlist was found!" << endl;
	//cout << "A playlist with the name: " << playlist_name << " was created!" << endl;
	create_playlist(playlist_name, prints_enabled); // creates the playlist automatically
	playlist = m_playlists[playlist_name];
	playlist->add_song_to_playlist(song_to_add); // adds a song to the created playlist
}

// remove a song from the playlist by song's name.
void Library::RemoveFromPL(const string& song_name, const string& playlist_name, bool make_sure) {
	auto playlist = get_playlist_by_name(playlist_name);
	if (playlist != nullptr) {
		playlist->remove_song_from_playlist(song_name); //if its one song, we want to make sure if the user want to delete
	}
	#pragma region Previous Implementation
	//if (check_if_user_playlist(playlist_name) && m_deleted->get_name() != playlist_name) {
	//	if (check_if_playlist_exist(playlist_name) || playlist_name == m_favorites->get_name()) {
	//		auto playlist = m_user_playlists.find(playlist_name)->second;
	//		auto song_to_remove = playlist->get_song_by_name(song_name); // this method check if a song exist in the playlist
	//		if (song_to_remove == nullptr) {
	//			cout << "The song" << song_name << "is not in the playlist: " << playlist_name << "!" << endl;
	//			return;
	//		}
	//		else
	//			playlist->remove_song_from_playlist(song_to_remove, make_sure); // all the checking is in playlist 
	//	}
	//	else

	//}
	//else
	//	cout << "This Playlist Cannot Be Edited!" << endl;
	#pragma endregion

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


bool Library::Are_All_Parameters_Empty(const string& param1,
	const string& param2, const string& param3 = "", const string& param4 = "", const string& param5 = "")
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
void Library::Update_Song(string song_name,
	string new_name, string artist, string album, string genre, string duration, string release_date)
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
void Library::Update_Song(int song_id, string new_name, string artist, string album, string genre, string duration, string release_date)
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

void Library::UpdatePodcast(string podcast_name, string new_name)
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

void Library::UpdateEpisode(int episode_id, string new_name, string duration, string release_date)
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

void Library::Print_Media_Exists_Error(std::string& new_name, const string& media_type)
{
	cout << media_type << " with name " << new_name << " already exists, please choose a unique name" << endl;
}

template <class T>
T* Library::Update_Media_By_Id(int media_id, T* (Server::* methodPtr)(int), string new_name, string duration, string release_date)
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


void Library::Print_Not_Found_By_Id_Error(int item_id, string item_type)
{
	cout << item_type << " with id " << item_id << " doesn't exist in server. Please add it first" << endl;
}

void Library::Print_No_Input_Parameters_Error()
{
	cout << "No paramters to updated" << endl;
}

//this method plays a song, updates most_played, most_recent stats 
//can be found in Play(song_name), Play(id). 
//since PlayAll and PlayAllRandom uses Play(id), we don't need to worry about the update of most_played, most_recent
void Library::play_song(Song* song) {
	cout << "Now playing: " << *song << endl;
	song->Play(); // here plays_counter updates
	//m_recent->Update_Most_Recent();
	m_recent->Add_To_Most_Recent(song->get_id());
	m_most_played->Update_Most_Played();
}

void Library::Play_Podcast(string podcast_name)
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

void Library::Play(string song_name)
{
	//todo: niv - add play playlist to library
	auto song = m_server->find_by_name(song_name);
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
	play_song(song_to_play);

}

void Library::Play(int id)
{
	auto song_to_play = m_server->find_song_by_id(id);
	play_song(song_to_play);
}

bool Library::check_if_continue_playing() {
	string prompt = "Would you like to continue playing? y/n: ";
	string reject_message = "Stopped playing";
	string accept_message = "Continue playing";
	if (Utilities::user_prompts_and_dialog(prompt, reject_message, accept_message)) {
		return true;
	}
	return false;
}

//todo: check if this function works
// Play all of the songs in the library
template <template<typename, typename> class MapType>
void Library::PlayAll(MapType<string, Song*>* songs_to_play) {
	if (songs_to_play->size() == 0) {
		cout << "There are no songs in the library." << endl;
		return;
	}
	cout << "Playing all library songs: " << endl;
	typename MapType<string, Song*>::iterator it;
	for (it = songs_to_play->begin(); it != songs_to_play->end(); it++) {
		cout << "Now playing: " << *it->second << endl;
		int id = it->second->get_id();
		Play(id);
		if (check_if_continue_playing() == false)
			return;
	}
	cout << "Finished playing all songs in the library." << endl;
}

// PlayAll with no function template
void Library::PlayAll() {
	auto songs_to_play = m_server->get_songs_sorted_by_alphabet();
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
	auto songs_to_play = m_server->get_songs_sorted_by_alphabet();
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

//return the playlist that needs to be played. return nullptr if no playlist was found
Playlist* Library::get_playlist_by_name(string playlist_name) {
	if (check_if_playlist_exist(playlist_name)) {
		auto playlist = m_playlists.find(playlist_name);
		return playlist->second;
	}
	//auto playlist = m_saved_playlist_names.find(playlist_name);
	//if (playlist != m_saved_playlist_names.end()) {
	//	return playlist->second;
	//}
	cout << "Playlist " << playlist_name << " doesn't exist!" << endl; //todo: make printing optional
	return nullptr;
}

//play a playlist by its name 
void Library::PlayPlaylist(string playlist_name) {
	auto playlist_to_play = get_playlist_by_name(playlist_name);
	if (playlist_to_play != nullptr) {
		playlist_to_play->Play();
	}
}

//play a playlist shuffled by its name 
void Library::PlayPlaylistShuffled(string playlist_name) {
	auto playlist_to_play = get_playlist_by_name(playlist_name);
	if (playlist_to_play != nullptr)
		playlist_to_play->Play_Random();
	
}


//called from the interface ("main")
void Library::Add_Song(string song_name, string file_path, string artist = "",
	string album = "", string genre = "", string duration = "", string release_date = "")
{
	if (m_server->Does_Song_Exist(file_path)) { //checks uniqueness
		cout << "Song was already added" << endl;
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

void Library::Add_Podcast_Episode(string episode_name, string podcast_name,
	string file_path, string duration, string release_Date)
{
	if (m_server->Does_Episode_Exist(episode_name)) { //checks uniqueness
		cout << "Episode was already added." << endl;
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
		string playlist_name;
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
		string file_name = playlist_pair.first;
		if (check_if_user_playlist(file_name)) {
			file_name = user_playlists_file_name;
			is_started_writing_user_playlists = true;
		}
		playlist_pair.second->save_playlist(file_name, mode);
		if (is_started_writing_user_playlists) {
			mode = ios::app;
		}
	}

	//unordered_map<string, Playlist*>::iterator itr;
	//for (itr = m_playlists.begin(); itr != m_playlists.end(); itr++)
	//{
	//	string playlist_name = itr->first;
	//	Playlist* current_playlist = itr->second;
	//	vector<string*> params = { &playlist_name };
	//	Utilities::Replace_All(params, false);
	//	for (auto& song : current_playlist->get_songs()) {
	//		write_playlists << " " << playlist_name << " " << song.second->get_id() << endl;
	//	}
	//}

	
}



Song* Library::Pick_Media(string media_name, unordered_multimap<string, Song*>* collection_to_search) {
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

//todo: we have a method like this in library, check if needed here also.
// double checks with the user if the song should be deleted, if yes- returns true.
bool Library::make_sure_to_delete_song(Song* song) {
	cout << "You chose to delete the song: " << endl;
	cout << "The song details are:" << endl;
	cout << *song << endl;
	while (true) { // when the user enter y/n, there is a return
		cout << "Are you sure you want to remove this song from library?" << endl;
		cout << "Press y/n: ";
		char answer;
		cin >> answer; cout << endl;
		if (answer == 'y') {
			return true;
		}
		if (answer == 'n') {
			return false;
		}
		cout << "Invalid answer! try again." << endl;
	}
}

//Adds song to m_deleted playlist
// method removes a specific song, used by the two Delete_Song methods
void Library::delete_song(Song* song_to_delete) { //todo: too many messages?
	auto playlist_appearances = song_to_delete->get_playlist_appearances(); //todo: check this
	if (playlist_appearances->size() > 0) {
		unordered_set<string>::iterator it;
		for (it = playlist_appearances->begin(); it != playlist_appearances->end(); it++) {
			RemoveFromPL(song_to_delete->get_name(), *it, false);// don't need to check with user here
			// false- don't need to make sure with the user if he wants to delete from this playlist
		}
	}
	// a song will stay in the playlists: most played, recents until it will be permanently deleted
	m_deleted->add_to_trash(song_to_delete); // we do want to check with the user if he's sure that he wants to delete song 
	//cout << "Song was successfully removed from the library and all of it's playlists!" << endl;
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
void Library::Delete_Song(string song_name)
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
		//todo: add check before delete
		m_server->Permanent_Delete_Podcast_Episode(m_server->find_episode_by_id(id));
	}
	catch (const std::exception&)
	{
		Print_Not_Found_By_Id_Error(id, typeid(Song).name());
	}
}

void Library::Delete_Episode(string episode_name)
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

void Library::Delete_Podcast(string podcast_name)
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
	cout << media_name << " isn't present in the server." << endl;
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


void Library::update_most_played() { 
	
	m_most_played->Update_Most_Played();
	//Server::update_most_played_songs();
	//auto most_played = Server::get_most_played();
	//int most_played_size = most_played->size();
	//m_most_played->clear_all_playlist();
	//int minimum = min(most_played_size, max_most_played); // using c++ algorithm
	//multimap<int, Song*>::iterator it = most_played->end();
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
		cout << itr->second << endl;
	}
	if (itr == m_server->get_songs_by_name()->end()) {
		os << "No more songs :(" << endl;
	}
	return os;
}

ostream& operator<<(ostream& os, const Library& lib)
{
	return lib.Print(os, 0, lib.num_of_songs_to_print);
}






//************************************************************************************************************************
//********************************************* Methods That May Not Be Needed********************************************
//************************************************************************************************************************ 

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

//// double checks with the user if the song should be deleted, if yes- removes the song.
//void Library::ask_user_to_remove_song(Song* song, Playlist* playlist) {
//	cout << "You chose to remove the song: " << endl;
//	PrintSong(song->get_id());
//	cout << "Are you sure you want to remove this song from: " << playlist->get_name() << "?" << endl;
//	cout << "Press y/n: ";
//	char answer;
//	cin >> answer;
//	cout << endl;
//	if (answer == 'y') {
//		playlist->remove_song_from_playlist(song->get_name());
//		cout << "Song Was Successfully Removed From Playlist!" << endl;
//	}
//	else
//		cout << "The Song Wasn't Removed!" << endl;
//}

//Library::Library() : m_songs_by_name(Server::get_songs_by_name()), m_playlists()
//{
//	// = new set<Song*>*; //is initialization needed here?
//	//m_playlists_map= &map<string, Playlist*>(m_playlists->get_user_playlists()->begin(), m_playlists->get_user_playlists()->end()); //map of playlists
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