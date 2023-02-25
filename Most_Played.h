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

//#include "Automatic_Playlist.h"
//class Playlist;
class Automatic_Playlist;

class Most_Played :
    public Automatic_Playlist
{
private:

public:
    Most_Played(Library* library, Server* server);
    //void add_song_to_playlist(Song* song);
    //void remove_song_from_playlist(Song* song, bool make_sure = true);
    //void clear_all_playlist();
    void Add_To_Most_Played(Song* song);
    //void Clear_Most_Played();
    //update most played songs playlist
    void Update_Most_Played();
    //void restore_playlist();
   /* void save_playlist();*/
};

#endif// MOSTPLAYED_H