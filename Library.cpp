#include "Library.h"

//Library::Library()
//{
//
//}

void Library::PrintSong(int id)
{

}

void Library::PrintPL()
{
}
void Library::RemoveFromPL(string song_name, string playlist_name)
{
}

void Library::Add2PL(int id, string playlist_name)
{
}

void Library::Add(string song_name)
{
    multimap<int, int>::iterator itr;
    //for (itr = m_songs_by_name.begin(); itr != m_songs_by_name.end(); ++itr) {
    //    cout << '\t' << itr-> << '\t' << itr->second
    //        << '\n';
    //}
    if (m_songs_by_name.count(song_name) == 1) { //if there's exactly one song
        //m_songs_by_name[song_name] = Server::get_songs_by_name()[song_name];
    }
    cout << endl;
}

void Library::Delete(int id)
{

}

