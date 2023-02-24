#ifndef TRASH_H
#define TRASH_H
//#include "Playlist.h"


//class Song;
//class Library;
class Playlist;
class Trash : public Playlist
{
private:



public:
	Trash(Library* library);
	void add_song_to_playlist(Song* song); 
	void remove_song_from_playlist(Song* song);
	void clear_all_playlist();
	void restore_playlist();
	void save_playlist();
	void add_to_trash(Song* song, bool add_print = true);

};

#endif// TRASH_H