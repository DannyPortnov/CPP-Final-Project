#pragma once
#include "Playlist.h"
class Automatic_Playlist :
    public Playlist
{
    protected: //todo: continue here
        Automatic_Playlist(string name, Library* library);
        inline void Print_Cannot_Edit_Error();
        template <typename TCollection, typename TIter >
        void Update_Most(TCollection songs_collection, int max_items, TIter iter);
    public:

};


