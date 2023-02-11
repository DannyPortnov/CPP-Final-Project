#include "Library.h"
#include <set>
#include <vector>
#include <algorithm>

Library::Library() : m_songs_by_name(Server::get_songs_by_name()), m_playlists()
{
	// = new set<Song*>*; //is initialization needed here?
}


void Library::PrintSong(int id)
{
	cout << m_songs_by_id->at(id) << endl;
}

void Library::PrintPL()
{

}

void Library::RemoveFromPL(string song_name, string playlist_name)
{
	map<string, Playlist*> playlists(m_playlists->get_custom_playlists()->begin(), m_playlists->get_custom_playlists()->end()); //map of playlists
	auto songs_by_names = playlists.at(playlist_name)->Get_Songs();
	multiset<Song*>::iterator start_of_songs_set; //iterator for any/all songs with that name
	multiset<Song*>::iterator end_of_songs_set;
	multiset<Song*>::iterator first_wanted_song;
	multiset<Song*>::iterator last_wanted_song;
	int number_of_songs = Count_Songs(&songs_by_names, song_name, &start_of_songs_set, &end_of_songs_set, &first_wanted_song, &last_wanted_song);
	switch (number_of_songs)
	{
		case 0: {
			cout << "No songs named " << song_name << " currently in " << playlist_name  <<"." << endl;
			return;
		}
		case 1: {
			playlists.at(playlist_name)->remove_song(*first_wanted_song); //or *last doesn't matter
			return;
		}
		default:
			break;
	}
	cout << "More than one song named " << song_name << ", choose which one you want to remove" << endl;
	playlists.at(playlist_name)->remove_song(Pick_Song(number_of_songs, &first_wanted_song, &last_wanted_song)); //removes the selected song
	//playlists.at(playlist_name)->remove_song(songs_by_names.)
	//for (int i = 0; i < m_playlists->get_custom_playlists()->size(); i++)
	//{

	//}
}
//Add a song by its ID to a playlist. Creates it if it doesn't exist
void Library::Add2PL(int id, string playlist_name)
{
	auto begin = m_playlists->get_custom_playlists()->begin();
	auto end = m_playlists->get_custom_playlists()->end();
	auto predicate = [&](Playlist pl) { return pl.get_name() == playlist_name; }; //filter songs which have the desired name
	auto itr_to_wanted_playlist = find(begin, end, predicate); //first element that satisfies condition
	if (itr_to_wanted_playlist == end && (*end)->get_name() != playlist_name) { //if playlist doesn't exist
		auto new_pl = new Playlist(playlist_name); //create a new one (ptr to playlist)
		new_pl->add_song(m_songs_by_id->at(id)); //(add the song)
		m_playlists->Add(new_pl);
		return;
	}
	(*itr_to_wanted_playlist)->add_song(m_songs_by_id->at(id)); //
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
