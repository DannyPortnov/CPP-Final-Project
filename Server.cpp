#include "Server.h"

Server::Server()
{
    auto compare_by_id = [](Song* a, Song* b) { //lambda function
        return a->get_id() < b->get_id();
    };
    set<int, decltype(compare_by_id)> songs_by_id; //insert here all the songs!
}

void Server::upload_podcast_episode(Episode* episode)
{
    //search for the podcast's name in m_all_podcasts. if exists: add the episode
    //else: create new podcast and add it to m_all_podcasts
}
