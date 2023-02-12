#include "Server.h"

Server::Server()
{
    auto compare_by_id = [](Song* a, Song* b) { //lambda function
        return a->get_id() < b->get_id();
    };
    set<int, decltype(compare_by_id)> songs_by_id; //insert here all the songs!
}

set<Song*>* Server::get_songs_by_id()
{
    return m_all_songs_by_id;

}

multiset<Song*>* Server::get_songs_by_artist()
{
    return nullptr;
}

multiset<Song*>* Server::get_songs_by_album()
{
    return nullptr;
}

multiset<Song*>* Server::get_songs_by_genre()
{
    return nullptr;
}

multiset<Song*>* Server::find_by_name(string name)
{
    return nullptr;
}
