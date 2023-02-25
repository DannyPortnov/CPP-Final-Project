//#include "Library.h" //todo: check this one
#include "Playlist.h"
#include "Automatic_Playlist.h"

Automatic_Playlist::Automatic_Playlist(string name, Library* library) : Playlist(name, library) {}

inline void Automatic_Playlist::Print_Cannot_Edit_Error()
{
	cout << "This playlist cannot be edited!" << endl;
}

//template <typename TCollection >
//void Automatic_Playlist::Update_Most(TCollection songs_collection, int max_items, bool is_reverse) {
//	int playlist_size = songs_collection->size();
//	Playlist::clear_all_playlist(false); //false for not adding dialog
//	int minimum = min(playlist_size, max_items); // using c++ algorithm
//	auto iter = 
//	// Create an iterator for the list
//	// Traverse through the list using the iterator
//	for (int i = 0; i < minimum; i++) {
//		Playlist::add_song_to_playlist(*iter, false); // add to recent the updated data structure. false for not adding dialog
//		iter++;
//	}
//}

void Automatic_Playlist::restore_playlist() {
	return;
}

void Automatic_Playlist::add_song_to_playlist(Song* song, bool add_print) {
	Print_Cannot_Edit_Error();
}
void Automatic_Playlist::remove_song_from_playlist(Song* song, bool make_sure) {
	Print_Cannot_Edit_Error();
}
void Automatic_Playlist::clear_all_playlist(bool add_print) {
	Print_Cannot_Edit_Error();
}