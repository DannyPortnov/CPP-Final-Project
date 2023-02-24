#pragma once
#include "Automatic_Playlist.h"
class Most_Played :
    public Automatic_Playlist
{
private:

public:
    Most_Played();
    void add_song_to_playlist(Song* song); //works
    void remove_song_from_playlist(Song* song, bool make_sure = true);
    void clear_all_playlist();
    void Add_To_Most_Played(Song* song);
    void Clear_Most_Played();
    void Update_Most_Played();
};

