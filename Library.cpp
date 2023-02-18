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
	cout << m_songs_by_id->at(id) << endl;
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
	else {
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
	//todo: need to delete all songs from the server.
	else if (playlist->get_name() == m_deleted->get_name()) {
		multimap<string, Song*>::iterator it;
		for (it = playlist->get_songs().begin(); it != playlist->get_songs().end(); it++) {
			Server::permanent_delete_song(it->second); //todo: implement in server class
													//todo: check if it is better for library to inherit from server
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

// print all of the playlists exist.
void Library::print_all_playlists() {
	int i = 1;
	cout << "List Of Playlists:" << endl;
	cout << "(" << i << "). " << m_favorites->get_name() << endl;
	i++;
	cout << "(" << i << "). " << m_most_played->get_name() << endl;
	i++;
	cout << "(" << i << "). " << m_recent->get_name() << endl;
	i++;
	set<string>::iterator it;
	for (it = m_user_playlist_names.begin(); it != m_user_playlist_names.end(); it++) {
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
	else if (check_if_playlist_can_be_edited(playlist_name)) {
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

char Library::ask_user_to_remove_song(Song* song, const string& playlist_name) {

}


void Library::RemoveFromPL(string song_name, const string& playlist_name)
{
	if (check_if_playlist_can_be_edited(playlist_name) == false) { // return true if can be edited
		cout << "This Playlist Cannot Be Edited!" << endl;
	}
	else {
		if (check_if_playlist_exist(playlist_name) || playlist_name == m_favorites->get_name()) {
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
				unordered_multimap<string, Song*>::iterator it;
				while (id_not_found) {
					int chosen_id;
					cin >> chosen_id;
					for (it = (*same_name_songs).begin(); it != (*same_name_songs).end(); it++) {
						if (it->second->get_id() == chosen_id) {
							id_not_found = false;
							cout << "You chose to remove the song: " << endl;
							cout << *(it->second) << endl;
							cout << "Are you sure you want to remove this song from: " << playlist_name << "?" << endl;
							cout << "Press y/n: " << endl;
							char answer;
							cin >> answer;
							if (answer == 'y') {
								m_user_playlists.find(playlist_name)->second->remove_song_from_playlist(it->second);
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
				cout << "You chose to remove the song: " << endl;
				cout << (m_user_playlists.find(playlist_name)->second->) << endl; //todo: check if print is ok
				cout << "Are you sure you want to remove this song from: " << playlist_name << "?" << endl;
				cout << "Press y/n: " << endl;
				char answer;
				cin >> answer;
				if (answer == 'y') {
					m_user_playlists.find(playlist_name)->second->remove_song_from_playlist();
					cout << "Song Was Successfully Removed From Playlist!" << endl;
				}
				else
					cout << "The Song Wasn't Removed!" << endl;

			}
		}
	}


	//Update_Playlists_Map();
	//auto songs_by_names = m_playlists_map->at(playlist_name)->Get_Songs();
	//multiset<Song*>::iterator start_of_songs_set; //iterator for any/all songs with that name
	//multiset<Song*>::iterator end_of_songs_set;
	//multiset<Song*>::iterator first_wanted_song;
	//multiset<Song*>::iterator last_wanted_song;
	//int number_of_songs = Count_Songs(&songs_by_names, song_name, &start_of_songs_set, &end_of_songs_set, &first_wanted_song, &last_wanted_song);
	//switch (number_of_songs)
	//{
	//	case 0: {
	//		cout << "No songs named " << song_name << " currently in " << playlist_name  <<"." << endl;
	//		return;
	//	}
	//	case 1: {
	//		m_playlists_map->at(playlist_name)->remove_song(*first_wanted_song); //or *last doesn't matter
	//		return;
	//	}
	//	default:
	//		break;
	//}
	//cout << "More than one song named " << song_name << ", choose which one you want to remove" << endl;
	//m_playlists_map->at(playlist_name)->remove_song(Pick_Song(number_of_songs, &first_wanted_song, &last_wanted_song)); //removes the selected song
}




void Library::PrintPL()
{

}


void Library::Add(string song_name)
{
	auto all_songs = Server::get_songs_by_name();
	multiset<Song*>::iterator start_of_songs_set; //iterator for any/all songs with that name
	multiset<Song*>::iterator end_of_songs_set;
	multiset<Song*>::iterator first_wanted_song;
	multiset<Song*>::iterator last_wanted_song;
	int number_of_songs = Count_Songs(all_songs, song_name, &start_of_songs_set, &end_of_songs_set, &first_wanted_song, &last_wanted_song);
	switch (number_of_songs)
	{
		case 0: {
			cout << "No songs named " << song_name << " currently in the server. Please download it first and place it in the folder" << endl;
			return;
		}
		case 1: {
			m_songs_by_name->insert(*first_wanted_song); //or *last doesn't matter
			return;
		}
		default:
			break;
	}
	cout << "More than one song named " << song_name << ", choose which one you want to add" << endl;
	m_songs_by_name->insert(Pick_Song(number_of_songs, &first_wanted_song, &last_wanted_song));
}




Song* Library::Pick_Song(int number_of_songs, multiset<Song*>::iterator* start, multiset<Song*>::iterator* end) {
	Song** filtered_songs = new Song * [number_of_songs]; //dynamic array of pointers
	for (int i = 0; start != end; start++, i++)
	{
		filtered_songs[i] = **start;
	}
	for (int i = 0; i < number_of_songs; i++) {
		cout << i + 1 << " - " << filtered_songs[i] << endl;
	}
	int answer;
	std::cin >> answer;
	return filtered_songs[answer - 1];
}

void Library::Update_Playlists_Map()
{
	m_playlists_map = &map<string, Playlist*>(m_playlists->get_user_playlists()->begin(), m_playlists->get_user_playlists()->end()); //map of playlists
}


int Library::Count_Songs(multiset<Song*>* songs, string song_name, multiset<Song*>::iterator* start, multiset<Song*>::iterator* end,
	multiset<Song*>::iterator* first_wanted_song, multiset<Song*>::iterator* last_wanted_song) const{

	auto predicate = [&](const Song* song) { return song->get_name() == song_name; }; //filter songs which have the desired name
	first_wanted_song = lower_bound(start, end, predicate);
	last_wanted_song = upper_bound(start, end, predicate);
	return count_if(start, end, predicate); //count songs with "song_name" name
}

void Library::Delete(string song_name)
{
	//m_playlists << 
}
void Library::Delete(int id)
{

}

ostream& Library::print(ostream& os, int begin, int end) const
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
	return lib.print(os, 0, Library::num_of_songs_to_print);
}
