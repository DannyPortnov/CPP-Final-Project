#ifndef AUTOMATICPLAYLIST_H
#define AUTOMATICPLAYLIST_H

//#include "Playlist.h"
class Playlist;
//class Most_Recent;
//class Most_Played;

class Automatic_Playlist :
    public Playlist
{
    protected: //todo: continue here
        inline void Print_Cannot_Edit_Error();
        /*template <typename TCollection >
        void Update_Most(TCollection songs_collection, int max_items, bool is_reverse);*/
        //virtual void restore_playlist(); //todo: implement here, even a simple implementation
        virtual void add_song_to_playlist(Song* song, bool add_print = true); //works
        virtual void remove_song_from_playlist(Song* song, bool make_sure = true);
        virtual void clear_all_playlist(bool add_print = true);
    public:
        Automatic_Playlist(string name, Library* library, Server* server);
        void save_playlist(string file_name, ios_base::openmode mode = ios::out);
        //todo: think of an implementation ffor all of Playlist's virtual methods
};

#endif// AUTOMATICPLAYLIST_H

