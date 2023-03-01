#include "Library.h"
#include "Playlist.h"
#include "Automatic_Playlist.h"
#include "Most_Played.h"
#include <algorithm>

#define max_most_played 10
#define Most_Played_Name "Most Played"

Most_Played::Most_Played(Library* library, Server* server) : Automatic_Playlist(Most_Played_Name, library, server)
{
	Update_Automatic_Playlist();
}

// This function updates a playlist with the most played songs,
//  using Server's method to retrieve the list of songs.
// The function clears the existing playlist, 
//and then adds the most played songs up to a maximum number specified by max_most_played.
void Most_Played::Update_Automatic_Playlist() 
{
	// Update the most played songs on the server
	m_server->update_most_played_songs();
	// Retrieve the most played songs from the server
	auto most_played = m_server->get_most_played();
	// Get the number of most played songs
	int most_played_size = most_played->size();
	// Clear the existing playlist without notifying the user
	Playlist::clear_all_playlist(false);
	// Determine the minimum number of most played songs to add to the playlist
	int minimum = min(most_played_size, max_most_played); // using c++ algorithm
	// Use a reverse iterator to iterate through the most played songs in descending order
	std::multimap<int, Song*>::reverse_iterator it = most_played->rbegin();
	for (int i = 0; i < minimum; i++) {
		// Add each most played song to the playlist without notifying the user
		Playlist::add_song_to_playlist(it->second, false);
		it++;
	}  
}