#ifndef MOSTPLAYED_H
#define MOSTPLAYED_H
#define   _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

class Automatic_Playlist;

class Most_Played :
    public Automatic_Playlist
{
private:

public:
    Most_Played(Library* library, Server* server);
    void Update_Automatic_Playlist();
};

#endif// MOSTPLAYED_H