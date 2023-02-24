#pragma once
#include "Automatic_Playlist.h"
class Most_Recent :
    public Automatic_Playlist
{
private:
    

public:
    Most_Recent(Library* library);
    void add_song_to_playlist(Song* song); //works
    void remove_song_from_playlist(Song* song, bool make_sure = true);
    void clear_all_playlist();
 /*   void Add_To_Most_Recent(Song* song);
    void Clear_Most_Recent();*/
    void Update_Most_Recent();
    void Remove_From_Most_Recent(int id);
};

