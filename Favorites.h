#ifndef FAVORITES_H
#define FAVORITES_H

#include "Playlist.h"
class Favorites :
    public Playlist
{
public:
    Favorites(Library* library);

    void restore_playlist();
    void save_playlist();
};

#endif// FAVORITES_H