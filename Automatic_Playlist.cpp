#include "Playlist.h"
#include "Automatic_Playlist.h"

Automatic_Playlist::Automatic_Playlist(std::string name, Library* library, Server* server) : Playlist(name, library, server) {}

inline void Automatic_Playlist::Print_Cannot_Edit_Error(bool make_sure)
{
	if (make_sure)
	{
		std::cout << "This playlist cannot be edited!" << std::endl;
	}
}

void Automatic_Playlist::save_playlist(std::string file_name, ios_base::openmode mode) {
	return;
}

void Automatic_Playlist::add_song_to_playlist(Song* song, bool add_print) {
	Print_Cannot_Edit_Error(add_print);
}
void Automatic_Playlist::remove_song_from_playlist(Song* song, bool make_sure) {
	Print_Cannot_Edit_Error(make_sure);
}
void Automatic_Playlist::clear_all_playlist(bool add_print) {
	Print_Cannot_Edit_Error(add_print);
}