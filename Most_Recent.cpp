#include "Library.h"
#include "Playlist.h"
#include "Automatic_Playlist.h"
#include "Most_Recent.h"
#include <algorithm>
#define max_recents 10

#define Most_Recent_Name "Most Recent"

Most_Recent::Most_Recent(Library* library, Server* server) : Automatic_Playlist(Most_Recent_Name, library, server)
{
	Update_Automatic_Playlist();
}

//update recent songs playlist 
void Most_Recent::Update_Automatic_Playlist()
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

}

void Most_Recent::Remove_From_Most_Recent(int id)
{
	auto song_to_remove = m_server->find_song_by_id(id);
	m_server->remove_from_recently_played(id); // removes from the data structure
	Update_Automatic_Playlist();

}

void Most_Recent::Add_To_Most_Recent(int id)
{
	m_server->add_to_recently_played(id);
	Update_Automatic_Playlist();
}