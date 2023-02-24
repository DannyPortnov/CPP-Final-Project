#include "Most_Recent.h"
#include <algorithm>
#define max_recents 10

Most_Recent::Most_Recent(Library* library) : Automatic_Playlist(typeid(this).name(), library)
{

}

void Most_Recent::add_song_to_playlist(Song* song)
{
	Print_Cannot_Edit_Error();
}

void Most_Recent::remove_song_from_playlist(Song* song, bool make_sure)
{
	Print_Cannot_Edit_Error();
}

void Most_Recent::clear_all_playlist()
{
	Print_Cannot_Edit_Error();
}

//void Most_Recent::Add_To_Most_Recent(Song* song)
//{
//	Playlist::add_song_to_playlist(song); //Call the parent's method
//}

//void Most_Recent::Clear_Most_Recent()
//{
//	Playlist::clear_all_playlist();
//}

void Most_Recent::Update_Most_Recent()
{
	auto recently_played = Server::get_recently_played();
	Automatic_Playlist::Update_Most(recently_played, max_recents, recently_played->begin());

	#pragma region Previous implementation
	//auto recently_played = Server::get_recently_played();
	//int recents_size = recently_played->size();
	//Clear_Most_Recent();
	//int minimum = min(recents_size, max_recents); // using c++ algorithm
	//// Create an iterator for the list
	//auto it = recently_played->begin();
	//// Traverse through the list using the iterator
	//for (int i = 0; i < minimum; i++) {
	//	Add_To_Most_Recent(*it); // add to recent the updated recently_played linked_list
	//	it++;
	//}  
	#pragma endregion

}

void Most_Recent::Remove_From_Most_Recent(int id)
{
	auto song_to_remove = Server::find_song_by_id(id);
	Playlist::remove_song_from_playlist(song_to_remove, false); // removes from playlist, don't make sure to delete songs from PL
	Server::remove_from_recently_played(id); // removes from the data structure
	Update_Most_Recent();

}
