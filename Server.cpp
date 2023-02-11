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
