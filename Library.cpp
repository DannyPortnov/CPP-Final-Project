#include "Library.h"
#include <set>
#include <vector>

//Library::Library()
//{
//
//}

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
	auto songs_pointer = Server::get_songs_by_name();
	auto itr = songs_pointer->find(song_name); //iterator for any/all songs with that name
	//auto pair = songs_pointer->equal_range(song_name); //break the code appearntly?
	//auto& start_of_range = pair.first;
	//auto& end_of_range = pair.second;
	//if (m_songs_by_name.count(song_name) == 1) {
	//    m_songs_by_name.insert(pair<string, Song>(song_name, itr->second));
	//    return;
	//}
	int number_of_songs = songs_pointer->count(song_name);
	switch (number_of_songs)
	{
	case 0: {
		cout << "No songs named " << song_name << " currently in the server. Please download it first and place it in the folder" << endl;
		return;
	}
	case 1: {
		m_songs_by_name->insert(pair<string, Song>(song_name, itr->second));
		return;
	}
	default:
		break;
	}
	cout << "More than one song named " << song_name << ", choose which one you want to add" << endl;
	Song** filtered_songs = new Song * [number_of_songs]; //dynamic array of pointers
	for (int i = 1; itr != songs_pointer->end(); itr++, i++) {
		//filtered_songs.push_back(itr->second);
		cout << i << " - " << itr->second << endl;
	}
	int answer;
	cin >> answer;
	m_songs_by_name->insert(pair<string, Song>(song_name, *filtered_songs[answer - 1]));
	//int i = 1;
	//for (auto filtered_songs_itr = filtered_songs.begin(); filtered_songs_itr != filtered_songs.end(); filtered_songs_itr++) {
	//    if (i == answer) {
	//        m_songs_by_name.insert(pair<string, Song>(song_name, *filtered_songs_itr));
	//        return;
	//    }
	//    i++;
	//}
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
		cout << itr->second << endl;
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
