#include "Library.h"
#include "Playlist.h"
#include "Favorites.h"

#define Favorites_name "Favorites"

Favorites::Favorites(Library* library) : Playlist(Favorites_name, library, true) //cannot use typeid(*this) (at least in initalization list)
{
	//restore_playlist();
}

//void Favorites::restore_playlist() //todo: make maybe another parent class
//{
//	//ifstream read_playlists("c:\\temp\\" + m_playlist_name + ".dat", ios::in);
//	//if (!Utilities::Is_File_Valid(read_playlists)) { 
//	//	return;
//	//}
//	//while (!read_playlists.eof()) {
//	//	int song_id;
//	//	read_playlists >> song_id;
//	//	
//	//	if (Utilities::Is_End_Of_File(read_playlists)) {
//	//		break;
//	//	}
//	//}
//}
//
//void Favorites::save_playlist() //todo: make maybe another parent class
//{
//	ofstream write_playlist("c:\\temp\\" + m_playlist_name + ".dat", ios::in);
//	if (!Utilities::Is_File_Valid(write_playlist)) {
//		return;
//	}
//	for (auto& song : m_songs) {
//		write_playlist << song->get_id() << endl;
//	}
//}
