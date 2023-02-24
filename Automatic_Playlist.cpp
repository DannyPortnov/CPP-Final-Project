#include "Automatic_Playlist.h"

inline void Automatic_Playlist::Print_Cannot_Edit_Error()
{
	cout << "This playlist cannot be edited!" << endl;
}

Automatic_Playlist::Automatic_Playlist(string name) : Playlist(name)
{

}

template <typename TCollection, typename TIter >
void Automatic_Playlist::Update_Most(TCollection songs_collection, int max_items, TIter iter) {
	int playlist_size = songs_collection->size();
	Playlist::clear_all_playlist();
	int minimum = min(playlist_size, max_items); // using c++ algorithm
	// Create an iterator for the list
	// Traverse through the list using the iterator
	for (int i = 0; i < minimum; i++) {
		Playlist::add_song_to_playlist(*iter); // add to recent the updated recently_played linked_list
		iter++;
	}
}