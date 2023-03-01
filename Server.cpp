#include "Server.h"



//std::multimap<string, Song*>* Server::m_songs_by_alphabet_order;
//std::list<Song*>* Server::m_recently_played ;
//std::map<std::string, Podcast*>* Server::m_podcasts_by_alphabet_order ;
//std::unordered_map<int, Episode*>* Server::m_all_episodes_by_id;
//std::unordered_map<std::string, Episode*>* Server::m_all_episodes_by_name;
//std::unordered_map<std::string, Podcast*>* Server::m_all_podcasts;
//
//std::unordered_map<int, Song*>* Server::m_all_songs_by_id;
//std::unordered_set<std::string>* Server::m_songs_file_paths;
//std::unordered_multimap<std::string, Song*>* Server::m_all_songs_by_artist;
//std::unordered_multimap<std::string, Song*>* Server::m_all_songs_by_name ;
//std::unordered_multimap<std::string, Song*>* Server::m_all_songs_by_album;
//std::unordered_multimap<std::string, Song*>* Server::m_all_songs_by_genre ;
//std::unordered_map<int, Song*>* Server::m_recently_played_by_id;
//std::multimap<int, Song*>* Server::m_most_played ;

Server::Server()
{
	m_songs_by_alphabet_order = new std::multimap<std::string, Song*>();
	m_recently_played = new std::list<Song*>();
	m_podcasts_by_alphabet_order = new std::map<std::string, Podcast*>();
	m_all_episodes_by_id = new std::unordered_map<int, Episode*>();
	m_all_episodes_by_name = new std::unordered_map<std::string, Episode*>();
	m_all_podcasts = new std::unordered_map<std::string, Podcast*>();

	m_all_songs_by_id = new std::unordered_map<int, Song*>();
	m_songs_file_paths = new std::unordered_set<std::string>();
	m_all_songs_by_artist = new std::unordered_multimap<std::string, Song*>();
	m_all_songs_by_name = new std::unordered_multimap<std::string, Song*>();
	m_all_songs_by_album = new std::unordered_multimap<std::string, Song*>();
	m_all_songs_by_genre = new std::unordered_multimap<std::string, Song*>();
	m_recently_played_by_id = new std::unordered_map<int, Song*>();
	m_most_played = new std::multimap<int, Song*>();

	Restore_Songs(); //Must be here because serialization must happen before anything else
	Restore_Podcasts();
	Restore_Most_Recent(); 
	update_most_played_songs();//ALL songs ordered by their plays count
}

Server::~Server()
{
	Save_Songs(); //Do serialization in destructor (before removing all songs)
	Save_Podcasts();
	Save_Most_Recent(); 
	Destroy_All_Allocations();
}

void Server::Destroy_All_Allocations()
{
	Destory_Allocations(m_songs_by_alphabet_order);
	Destory_Allocations(m_podcasts_by_alphabet_order);
	Clear_And_Delete(m_all_episodes_by_id);
	Clear_And_Delete(m_all_episodes_by_name);
	Clear_And_Delete(m_all_podcasts);
	Clear_And_Delete(m_all_songs_by_id);
	Clear_And_Delete(m_all_songs_by_artist);
	Clear_And_Delete(m_all_songs_by_name);
	Clear_And_Delete(m_all_songs_by_album);
	Clear_And_Delete(m_all_songs_by_genre);
	Clear_And_Delete(m_recently_played_by_id);
	Clear_And_Delete(m_most_played);

	//if (m_recently_played != nullptr)
	//{
	//	for (auto elem : *m_recently_played) {
	//		delete elem;
	//	}
	//}
	Clear_And_Delete(m_recently_played);
	Clear_And_Delete(m_songs_file_paths);
}
//Song is unique!
void Server::Upload_Song(std::string song_name, std::string file_path,
	std::string artist,std::string album, std::string genre, std::string duration, std::string release_Date)
{
	auto song = new Song(song_name, file_path, album, artist, genre, release_Date);
	Add_Song_To_Collections(song);
}

void Server::Add_Song_To_Collections(Song*& song)
{
	const auto& name = song->get_name(); //avoids unnecessary copies of std::string objects
	const auto& album = song->get_album();
	const auto& artist = song->get_artist();
	const auto& genre = song->get_genre();

	m_songs_by_alphabet_order->emplace(name, song); //constructs the element directly in place
	m_all_songs_by_name->emplace(name, song); //constructs the element directly in place
	m_songs_file_paths->emplace(song->get_path());
	m_all_songs_by_id->emplace(song->get_id(), song);
	if (!album.empty()) {
		m_all_songs_by_album->emplace(album, song);
	}
	if (!artist.empty()) {
		m_all_songs_by_artist->emplace(artist, song);
	}
	if (!genre.empty()) {
		m_all_songs_by_genre->emplace(genre, song);
	}
}

void Server::Upload_Episode_To_Podcast(Podcast* podcast, std::string episode_name, std::string podcast_name, std::string file_path,
	std::string duration, std::string release_Date)
{
	if (podcast == nullptr) {//if podcast doesn't exist, creates the podcasts and adds the episode
		podcast = new Podcast(podcast_name); //deleted in Server
	}
	Episode* new_episode = new Episode(file_path, episode_name, podcast, release_Date, duration); //deleted in Podcast
	podcast->Add_Episode(new_episode); //Adds a UNIQUE episode to an EXISTING podcast
	m_all_podcasts->emplace(podcast_name, podcast);
	m_all_episodes_by_id->emplace(new_episode->get_id(), new_episode);
	m_all_episodes_by_name->emplace(new_episode->get_name(), new_episode);
	m_podcasts_by_alphabet_order->emplace(podcast_name, podcast);
}


void Server::Restore_Songs()
{
	std::string address = songs_file_name;
	Utilities::Format_Address(address);
	ifstream read(address, ios::in);
	if (!Utilities::Is_File_Valid(read, songs_file_name)) {
		return;
	}
	while (!Utilities::Is_End_Of_File_Or_Empty(read)) {
		std::string song_name, artist, album, genre, duration, release_date, file_path;
		int id, plays_count;
		read >> id >> song_name >> file_path >> artist >> album >> genre >> duration >> release_date >> plays_count;
		vector<std::string*> params = { &song_name, &artist, &album,&genre };
		Utilities::Replace_All(params,true );// replace all '_' to ' '
		auto song = new Song(id, song_name, file_path, album, artist, genre, release_date, duration, plays_count);
		Add_Song_To_Collections(song);
		if (Utilities::Is_End_Of_File(read)) {
			break;
		}
	}
	read.close();
}



void Server::Restore_Podcasts() {
	std::string address = podcasts_file_name;
	Utilities::Format_Address(address);
	ifstream read(address, ios::in);
	if (!Utilities::Is_File_Valid(read, podcasts_file_name)) {
		return;
	}
	while (!Utilities::Is_End_Of_File_Or_Empty(read)) {
		std::string episode_name,podcast_name, duration, release_date, file_path;
		read >> episode_name >> podcast_name >> file_path >> duration >> release_date;
		vector<std::string*> params = { &episode_name,&podcast_name };
		Utilities::Replace_All(params, true);// replace all '_' to ' '
		Podcast* podcast = nullptr;
		if (Does_Podcast_Exist(podcast_name)) {
			podcast = find_podcast_by_name(podcast_name);
		}
		Upload_Episode_To_Podcast(podcast, episode_name, podcast_name, file_path, duration, release_date);
		if (Utilities::Is_End_Of_File(read)) {
			break;
		}
	}
	read.close();
}

void Server::Restore_Most_Recent()
{
	std::string address = most_recent_file_name;
	Utilities::Format_Address(address);
	ifstream read(address, ios::in);
	if (!Utilities::Is_File_Valid(read, most_recent_file_name)) {
		return;
	}
	while (!Utilities::Is_End_Of_File_Or_Empty(read)) {
		int song_id;
		//std::string playlist_name;
		//read >> playlist_name >> song_id;
		read >> song_id;
		add_to_recently_played(song_id);
		if (Utilities::Is_End_Of_File(read)) {
			break;
		}
	}
	read.close();
}

std::unordered_map<int, Episode*>* Server::get_episodes_by_id() {
	return m_all_episodes_by_id;
}

std::unordered_map<std::string, Podcast*>* Server::get_podcasts()
{
	return m_all_podcasts;
}


void Server::Save_Podcasts()
{
	std::string address = podcasts_file_name;
	Utilities::Format_Address(address);
	ofstream write(address, ios::out);
	std::unordered_map<int, Episode*>::iterator itr;
	auto all_episodes = Server::get_episodes_by_id();
	for (itr = all_episodes->begin(); itr != all_episodes->end(); itr++)
	{
		auto episode = itr->second;
		std::string episode_name, podcast_name;
		episode_name = episode->get_name();
		podcast_name = episode->Get_Podcast()->Get_Podcast_Name();
		vector<std::string*> params = { &episode_name, &podcast_name };
		Utilities::Replace_All(params, false);// replace all '_' to ' '
		write  << episode_name << " " << podcast_name << " " << episode->get_path() << " "
			  << episode->get_duration() << " " << episode->get_release_date() << std::endl;
	}
	write.close();
}

void Server::Save_Songs()
{
	std::string address = songs_file_name;
	Utilities::Format_Address(address);
	ofstream write(address, ios::out);
	std::unordered_map<int, Song*>::iterator itr;
	auto all_songs = Server::get_songs_by_id();
	for (itr = all_songs->begin(); itr != all_songs->end(); itr++)
	{
		auto song = itr->second;
		std::string song_name, artist, album, genre;
		song_name = song->get_name();
		artist = song->get_artist();
		album = song->get_album();
		genre = song->get_genre();
		vector<std::string*> params = { &song_name, &artist, &album, &genre };
		Utilities::Replace_All(params, false);// replace all '_' to ' '
		write << song->get_id() << " " << song_name << " " << song->get_path() << " " << artist << " " << album
			<< " " << genre << " " << song->get_duration() << " " << song->get_release_date() << " " << song->get_plays_count() << std::endl;
	}
	write.close();
}
//Saves the whole list of songs in order of last time played (could be more than 10 songs)
void Server::Save_Most_Recent() { 
	std::string address = most_recent_file_name;
	Utilities::Format_Address(address);
	ofstream write(address, ios::out);
	//if (!Utilities::Is_File_Valid(write, most_recent_file_name)) {
	//	return;
	//}
	std::list<Song*>::reverse_iterator itr;
	for (itr = m_recently_played->rbegin();itr != m_recently_played->rend();itr++)
	{
		auto song = *itr;
		write << song->get_id() << std::endl;
	}
	//for (auto& song : *m_recently_played) {
	//	write << song->get_id() << std::endl;
	//}
	write.close();
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
	remove_song_from_collection(m_most_played, song); 
	remove_from_recently_played(song->get_id());
	m_songs_file_paths->erase(song->get_path());
	
	
	delete song; 
}

void Server::Permanent_Delete_Podcast_Episode(Episode* episode, bool make_sure, bool delete_from_podcast)
{
	auto& episode_name = episode->get_name();
	std::string prompt = "Are you sure that you want to delete: " + episode_name + "? y/n: ";
	std::string reject_message = episode_name + " wasn't removed!";
	std::string accept_message = episode_name + " was successfully removed!";
	if (make_sure && !Utilities::user_prompts_and_dialog(prompt, reject_message, accept_message)) {
		return;
	}
	m_all_episodes_by_id->erase(episode->get_id());
	m_all_episodes_by_name->erase(episode_name);
	if (!delete_from_podcast)
	{
		return;
	}
	auto podcast = episode->Get_Podcast();
	podcast->Delete_Episode(episode);
	if (podcast->get_podcast()->size() == 0) {//deletes empty podcast
		Permanent_Delete_Podcast(podcast);
	}
}

void Server::Permanent_Delete_Podcast(Podcast* podcast, bool make_sure)
{

	auto& podcast_name = podcast->Get_Podcast_Name();
	std::string prompt = "Are you sure that you want to delete: " + podcast_name + "? y/n: ";
	std::string reject_message = podcast_name + " wasn't removed!";
	std::string accept_message = podcast_name + " was successfully removed!";
	if (make_sure && !Utilities::user_prompts_and_dialog(prompt, reject_message, accept_message)) {
		return;
	}
	m_podcasts_by_alphabet_order->erase(podcast->Get_Podcast_Name()); //removes once the podcast
	//auto copy = *m_podcasts_by_alphabet_order; //a copy is required to iterate over the collection
	//for (auto& name_podcast_pair : copy) {
	//	m_podcasts_by_alphabet_order->erase(name_podcast_pair.first); //removes listing of podcast
	//}

	auto all_episodes = podcast->get_podcast();
	for (auto itr = all_episodes->begin(); itr != all_episodes->end(); itr++) {
		Permanent_Delete_Podcast_Episode(*itr, false, false); //don't delete from podcast
	}
	m_all_podcasts->erase(podcast_name);//removes listing of podcast

	delete podcast; //deletes a podcast with all episodes inside
}

template <typename T>
void Server::remove_song_from_collection(T* songs_by_field, Song* song) {
	//typename T::iterator
	for (auto iter = songs_by_field->begin(); iter != songs_by_field->end(); ++iter)
	{
		if (iter->second == song) {
			songs_by_field->erase(iter);
			return;
		}

	}
}

// A template function that takes an unordered map/multimap 
// and deletes all the values 
template<class T>
void Server::Destory_Allocations(T* collection)
{
	if (collection != nullptr)
	{
		for (typename T::iterator it = collection->begin(); it != collection->end(); ++it) {
			delete it->second;
		}
	}
	Clear_And_Delete(collection);
}

template<class T>
void Server::Clear_And_Delete(T* collection)
{
	if (collection!=nullptr)
	{
		collection->clear();
	}
	delete collection;
}

//Returns a unique song based on its id. If doesn't exists throws exception
Song* Server::find_song_by_id(int id)
{
	return Find_Unique(id, m_all_songs_by_id);
}

Episode* Server::find_episode_by_id(int id)
{
	return Find_Unique(id, m_all_episodes_by_id);
}

Episode* Server::find_episode_by_name(std::string name)
{
	return Find_Unique(name, m_all_episodes_by_name);
}

Podcast* Server::find_podcast_by_name(std::string name)
{
	return Find_Unique(name, m_all_podcasts);
}

template < class TKey, class TValue>
TValue* Server::Find_Unique(TKey param, std::unordered_map<TKey, TValue*>* collection_to_Search) {
	if (collection_to_Search->count(param) > 0) {
		return collection_to_Search->at(param);
	}
	throw exception();
}

std::unordered_multimap<std::string, Song*>* Server::find_by_name(std::string& name)
{
	return find_all(name, m_all_songs_by_name);
}
std::unordered_multimap<std::string, Song*>* Server::find_by_artist(std::string& singer)
{
	return find_all(singer, m_all_songs_by_artist);
}
std::unordered_multimap<std::string, Song*>* Server::find_by_album(std::string& album)
{
	return find_all(album, m_all_songs_by_album);
}
std::unordered_multimap<std::string, Song*>* Server::find_by_genre(std::string& genre)
{
	return find_all(genre, m_all_songs_by_genre);
}
bool Server::Does_Song_Exist(const std::string& file_path)
{
	return m_songs_file_paths->count(file_path) > 0;
}
bool Server::Does_Episode_Exist(std::string& episode_name)
{
	return m_all_episodes_by_name->count(episode_name) > 0;
}
bool Server::Does_Podcast_Exist(std::string& podcast_name)
{
	return m_all_podcasts->count(podcast_name) > 0;
}
//Searches in given collection based on key, and returns filtered unordered_multiset 
std::unordered_multimap<std::string, Song*>* Server::find_all(std::string& key, std::unordered_multimap<std::string, Song*>* collection) {
	auto range = collection->equal_range(key); // range of values that match the given name
	std::unordered_multimap<std::string, Song*>* filtered_songs = new std::unordered_multimap<std::string, Song*>;
	for (auto& it = range.first; it != range.second; ++it) {
		filtered_songs->emplace(it->first, it->second); //inserts each value into the filtered set.
	}
	return filtered_songs;
}

std::list<Song*>* Server::get_recently_played() {
	return m_recently_played;
}

std::multimap<int, Song*>* Server::get_most_played() {
	return m_most_played;
}


// updates recently played songs
//save all songs by a certain oreder. the last song that was played will be the first in the list.
// if a song is already in recents, and needs to be in the front, we'll remove it first and than push in front
// there will be no limit of elements in this list.
void Server::add_to_recently_played(int id) {
	auto song = find_song_by_id(id);
	if (m_recently_played_by_id->find(id) != m_recently_played_by_id->end()) {
		remove_from_recently_played(id);
	}
	m_recently_played->push_front(song); // the most recent song will be at the front
	m_recently_played_by_id->emplace(id, song);
}

// remove a song from recently played songs data structure
void Server::remove_from_recently_played(int id) {
	auto song = find_song_by_id(id);
	m_recently_played->remove(song); // removes the song from the list // complexity O(n)- n= number of songs in m_recently_played
	m_recently_played_by_id->erase(id);
	/*std::list<Song*>::iterator it;
	for (it = m_recently_played.begin(); it != m_recently_played.end(); it++) {
		if (*it == song) {
			m_recently_played.erase(it);
		}
	}*/ // to use if m_recently_played.remove(song) doesn't work
}

/*
void Server::update_recently_played(int id) {
	auto song = find_song_by_id(id);
	if (m_recently_played_by_id.find(id) != m_recently_played_by_id.end()) { //if a song is already in recents, needs to be in the front
		remove_from_recently_played(id);
	}
	else if (m_recently_played.size() == max_recents) {
		auto less_recent = m_recently_played.back();
		m_recently_played_by_id.erase(less_recent->get_id());
		m_recently_played.pop_back();
	}
	m_recently_played.push_front(song); // the most recent song will be at the front
	m_recently_played_by_id.emplace(id, song);
}
*/



// update the multimap that holds the songs by the order of the last played (search O(1)).
// when a song is deleted from server, we can just call this method and update the data structure
void Server::update_most_played_songs() {
	if (m_most_played == nullptr) {
		return;
	}
	m_most_played->clear(); // clear most played and than add to multimap after, plays_count updated.
	std::unordered_map<int, Song*>::iterator it; // goes over all songs organized by id
	for (it = m_all_songs_by_id->begin(); it != m_all_songs_by_id->end(); it++) {
		m_most_played->emplace(it->second->get_plays_count(), it->second); // start:least played, end: most played
	}
}


std::unordered_map<int, Song*>* Server::get_songs_by_id()
{
	return m_all_songs_by_id;
}

std::unordered_multimap<std::string, Song*>* Server::get_songs_by_name()
{
	return m_all_songs_by_name;
}

std::multimap<std::string, Song*>* Server::get_songs_sorted_by_alphabet()
{
	return m_songs_by_alphabet_order;
}

std::unordered_multimap<std::string, Song*>* Server::get_songs_by_artist()
{
	return m_all_songs_by_artist;
}

std::unordered_multimap<std::string, Song*>* Server::get_songs_by_album()
{
	return m_all_songs_by_album;
}

std::unordered_multimap<std::string, Song*>* Server::get_songs_by_genre()
{
	return m_all_songs_by_genre;
}

