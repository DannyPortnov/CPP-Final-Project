#ifndef PLAYLIST_H
#define PLAYLIST_H
#include <algorithm>
#include <random>
#include <ctime>
#include "AudioCollection.h"
#include "Song.h"


class Playlist : AudioCollection<Song>
{

private:
	string m_name;
	Player m_player;

public:
	Playlist(string name);
	void add_song(Song* song);
	void remove_song(Song* song);
	string get_name() const { return m_name; }
	void Play();
	void Play_Random();
	void Print();

//friend bool operator<(Playlist* a, Playlist* b);

friend bool operator<(const Playlist& a, const Playlist& b);


};

// todo: implement operator < in order set stl will be able to sort
//bool operator<(Playlist* a, Playlist* b);
bool operator<(const Playlist& a, const Playlist& b);


#endif// PLAYER_H