#pragma once
#include "Playlist.h"
class Favorites :
    public Playlist
{
public:
    Favorites(Library* library);

    void restore_playlist();
    void save_playlist();
};

