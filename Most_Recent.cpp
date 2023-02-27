#include "Library.h"
#include "Playlist.h"
#include "Automatic_Playlist.h"
#include "Most_Recent.h"
#include <algorithm>
#define max_recents 10

#define Most_Recent_Name "Most Recent"

Most_Recent::Most_Recent(Library* library, Server* server) : Automatic_Playlist(Most_Recent_Name, library, server)
{
	Update_Most_Recent();
}

//void Most_Recent::add_song_to_playlist(Song* song)
//{
//	Automatic_Playlist::Print_Cannot_Edit_Error();
//}
//
//void Most_Recent::remove_song_from_playlist(Song* song, bool make_sure)
//{
//	Automatic_Playlist::Print_Cannot_Edit_Error();
//}
//
//void Most_Recent::clear_all_playlist()
//{
//	Automatic_Playlist::Print_Cannot_Edit_Error();
//}

//void Most_Recent::Add_To_Most_Recent(Song* song)
//{
//	Playlist::add_song_to_playlist(song); //Call the parent's method
//}

//void Most_Recent::Clear_Most_Recent()
//{
//	Playlist::clear_all_playlist();
//}
// 
// 
//update recent songs playlist 
void Most_Recent::Update_Most_Recent()
{
	auto recently_played = m_server->get_recently_played();
	if (recently_played == nullptr) {
		return;
	}
	int recents_size = recently_played->size();
	Playlist::clear_all_playlist(false);
	int minimum = min(recents_size, max_recents); // using c++ algorithm
	// Create an iterator for the list
	auto it = recently_played->begin();
	// Traverse through the list using the iterator
	for (int i = 0; i < minimum; i++) {
		Playlist::add_song_to_playlist(*it, false); // add to recent the updated recently_played linked_list
		it++;
	}  
	#pragma region Previous implementation
	//auto recently_played = Server::get_recently_played();
	//auto itr = recently_played->begin();
	//Automatic_Playlist::Update_Most(recently_played, max_recents, recently_played->begin());

	#pragma endregion

}

void Most_Recent::Remove_From_Most_Recent(int id)
{
	auto song_to_remove = m_server->find_song_by_id(id);
	//Playlist::remove_song_from_playlist(song_to_remove, false); // Playlist is already cleared in Update_Most_Recent
	m_server->remove_from_recently_played(id); // removes from the data structure
	Update_Most_Recent();

}

void Most_Recent::Add_To_Most_Recent(int id)
{
	m_server->add_to_recently_played(id);
	Update_Most_Recent();
}

//void Most_Recent::restore_playlist() //todo: make maybe another parent class
//{
//	ifstream read_playlist("c:\\temp\\" + m_playlist_name + ".dat", ios::in);
//	if (!Utilities::Is_File_Valid(read_playlist)) {
//		return;
//	}
//	while (!read_playlist.eof()) {
//		int song_id;
//		read_playlist >> song_id;
//		m_library->Add2PL(song_id, m_playlist_name);
//		if (Utilities::Is_End_Of_File(read_playlist)) {
//			break;
//		}
//	}
//}

//void Most_Recent::save_playlist() //todo: make maybe another parent class
//{
//	ofstream write_playlist("c:\\temp\\" + m_playlist_name + ".dat", ios::in);
//	if (!Utilities::Is_File_Valid(write_playlist)) {
//		return;
//	}
//	for (auto& song : m_songs) {
//		write_playlist << song->get_id() << std::endl;
//	}
//}
