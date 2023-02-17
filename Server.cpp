#include "Server.h"

Server::Server()
{
    auto compare_by_id = [](Song* a, Song* b) { //lambda function
        return a->get_id() < b->get_id();
    };
    set<int, decltype(compare_by_id)> songs_by_id; //insert here all the songs!
}

//adds to all data structures
void Server::upload_song(Song* song)
{
    const auto& name = song->get_name(); //avoids unnecessary copies of string objects
    const auto& album = song->get_album();
    const auto& artist = song->get_artist();
    const auto& genre = song->get_genre();

    m_songs_by_alphabet_order.emplace(name, song); //constructs the element directly in place
    m_all_songs_by_id.emplace(song->get_id(), song);
    if (!album.empty()) {
        m_all_songs_by_album.emplace(album, song);
    }
    if (!artist.empty()) {
        m_all_songs_by_artist.emplace(artist, song);
    }
    if (!genre.empty()) {
        m_all_songs_by_genre.emplace(genre, song);
    }
}

void Server::upload_podcast_episode(Episode* episode)
{
    //search for the podcast's name in m_all_podcasts. if exists: add the episode
    //else: create new podcast and add it to m_all_podcasts
}
