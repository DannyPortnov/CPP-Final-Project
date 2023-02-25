#ifndef FAVORITES_H
#define FAVORITES_H
#define   _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

//#include "Playlist.h"
class Playlist;
class Favorites :
    public Playlist
{
public:
    Favorites(Library* library, Server* server);

    //void restore_playlist();
    //void save_playlist();
};

#endif// FAVORITES_H