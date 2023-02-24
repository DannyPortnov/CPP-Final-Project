#include "Most_Played.h"
#include <algorithm>

#define max_most_played 10

Most_Played::Most_Played() : Automatic_Playlist(typeid(this).name())
{

}

void Most_Played::add_song_to_playlist(Song* song)
{
	Print_Cannot_Edit_Error();
}

void Most_Played::remove_song_from_playlist(Song* song, bool make_sure)
{
	Print_Cannot_Edit_Error();
}

void Most_Played::clear_all_playlist()
{
	Print_Cannot_Edit_Error();
}

//void Most_Played::Add_To_Most_Played(Song* song)
//{
//	Playlist::add_song_to_playlist(song); //Call the parent's method
//}
//
//void Most_Played::Clear_Most_Played()
//{
//	Playlist::clear_all_playlist();
//}

void Most_Played::Update_Most_Played() //todo: make maybe another parent class
{
	Server::update_most_played_songs();
	auto most_played = Server::get_most_played();
	#pragma region Reversing most played
	//multimap<int, Song*> reversed_most_played;
	//for (auto it = most_played->rbegin(); it != most_played->rend(); ++it) { 
	//	reversed_most_played.emplace(it->first, it->second);
	//}  
	#pragma endregion
	Automatic_Playlist::Update_Most(most_played, max_most_played, most_played->rbegin()); 
	//rbegin is a reverse iterator that starts at the last element most_played
	//and iterates backward through the elements until it reaches the reverse end of the vector (the beginning)
	#pragma region Previous implementation
	//auto most_played = Server::get_most_played();
	//int most_played_size = most_played->size();
	//Clear_Most_Played();
	//int minimum = min(most_played_size, max_most_played); // using c++ algorithm
	//multimap<int, Song*>::iterator it = most_played->end();
	//for (int i = 0; i < minimum; i++) {
	//	it--;
	//	Add_To_Most_Played(it->second);
	//}  
	#pragma endregion

}