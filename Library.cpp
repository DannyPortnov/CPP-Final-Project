#include "Library.h"
#include <set>
#include <vector>
#include <algorithm>

Library::Library() : m_songs_by_name(Server::get_songs_by_name())
{
	//m_playlists
}


void Library::PrintSong(int id)
{

}

void Library::PrintPL()
{
}
void Library::RemoveFromPL(string song_name, string playlist_name)
{
}

void Library::Add2PL(int id, string playlist_name)
{
}



void Library::Add(string song_name)
{

	//for (itr = m_songs_by_name.begin(); itr != m_songs_by_name.end(); ++itr) {
	//    cout << '\t' << itr-> << '\t' << itr->second
	//        << '\n';
	//}
	//if (m_songs_by_name.count(song_name) == 1) { //if there's exactly one song
	//    multimap<string, Song>::iterator itr = m_songs_by_name.find(song_name);
	//    
	//}
	auto all_songs = Server::get_songs_by_name();
	auto start_of_songs_set = all_songs->begin(); //iterator for any/all songs with that name
	auto end_of_songs_set = all_songs->begin(); //iterator for any/all songs with that name
	//auto pair = songs_pointer->equal_range(song_name); //break the code appearntly?
	//auto& start_of_range = pair.first;
	//auto& end_of_range = pair.second;
	//if (m_songs_by_name.count(song_name) == 1) {
	//    m_songs_by_name.insert(pair<string, Song>(song_name, itr->second));
	//    return;
	//}
	auto predicate = [&](Song song) { return song.get_name() == song_name; }; //filter songs which have the desired name
	auto first_wanted_song = lower_bound(start_of_songs_set, end_of_songs_set, predicate);
	auto last_wanted_song = upper_bound(start_of_songs_set, end_of_songs_set, predicate);
	int number_of_songs = count_if(start_of_songs_set, end_of_songs_set, predicate); //count songs with "song_name" name
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
	Song** filtered_songs = new Song * [number_of_songs]; //dynamic array of pointers
	for (int i = 0; first_wanted_song != last_wanted_song; first_wanted_song++,i++)
	{
		filtered_songs[i] = *start_of_songs_set;
	}
	for (int i = 0; i< number_of_songs;  i++) {
		//filtered_songs.push_back(itr->second);
		cout << i + 1 << " - " << filtered_songs[i] << endl;
	}
	int answer;
	std::cin >> answer;
	m_songs_by_name->insert(filtered_songs[answer - 1]);
	//int i = 1;
	//for (auto filtered_songs_itr = filtered_songs.begin(); filtered_songs_itr != filtered_songs.end(); filtered_songs_itr++) {
	//    if (i == answer) {
	//        m_songs_by_name.insert(pair<string, Song>(song_name, *filtered_songs_itr));
	//        return;
	//    }
	//    i++;
	//}
}

void Library::Delete(string song_name)
{

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
