#ifndef AUTOMATICPLAYLIST_H
#define AUTOMATICPLAYLIST_H

class Playlist;

class Automatic_Playlist :
    public Playlist
{
    protected: 
        inline void Print_Cannot_Edit_Error(bool make_sure);
        virtual void add_song_to_playlist(Song* song, bool add_print = true); 
        virtual void remove_song_from_playlist(Song* song, bool make_sure = true);
        virtual void clear_all_playlist(bool add_print = true);
        virtual void Update_Automatic_Playlist() = 0;
    public:
        Automatic_Playlist(std::string name, Library* library, Server* server);
        void save_playlist(std::string file_name, ios_base::openmode mode = ios::out);
};

#endif// AUTOMATICPLAYLIST_H

