#ifndef AUTOMATICPLAYLIST_H
#define AUTOMATICPLAYLIST_H

//#include "Playlist.h"
class Playlist;
class Automatic_Playlist :
    public Playlist
{
    protected: //todo: continue here
        inline void Print_Cannot_Edit_Error();
        template <typename TCollection, typename TIter >
        void Update_Most(TCollection songs_collection, int max_items, TIter iter);
    public:
        Automatic_Playlist(string name, Library* library);

};

#endif// AUTOMATICPLAYLIST_H

