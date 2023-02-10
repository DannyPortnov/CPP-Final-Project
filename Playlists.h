#ifndef PLAYLISTS_H
#define PLAYLISTS_H


#include "AudioCollection.h"
#include "Song.h"

class Playlists : AudioCollection<Song>
{
private:

public:
	void Play();
};
#endif

