#ifndef TRASH_H
#define TRASH_H
#include "Playlist.h"



class Trash : public Playlist
{
private:



public:
	Trash();
	void add_song_to_playlist(Song* song); 
	void remove_song_from_playlist(Song* song);
	void clear_all_playlist();
	void add_to_trash(Song* song);

};

#endif// TRASH_H