#ifndef PODCAST_H
#define PODCAST_H
#include <string>
#include <iostream>
#include <unordered_set>
#include "Player.h"
#include "AudioFile.h"
#include "Episode.h"
using namespace std;

/*
A little explenation of how I understood Podcast & Podcasts:
A podcast includes a "playlist" of audio files (episodes), acts like a playlist of episodes.
Podcasts include a few Podcast object type elements in it- this class will act like playlists but with limited functionality.
Podcast will be a different class.
Questions:
1. How podcast is created/defined? is it in the server and when the user want to
   add it to his library (or to follow the podcast) he has an option to do so?
2. Can a user add a single episode to his library?
3. Does the user add the whole podcast? meaning to upload the path of all of the episodes?
   Or the path of a single episode?
   (If the last one is the right way to do it, dosen't it lose the whole point and meaning of a podcast?)
*/

//each podcast is a collection of episodes!
class Podcast 
{
private:
	unordered_set<Episode*> m_podcast; // podcast is a playlist of episodes (no need to be ordered)

public:
	//Podcast();
	//~Podcast();
	//Adds an episode to the podcast
	void Add_Episode(Episode* episode);
	string& Get_Podcast_Name();
	unordered_set<Episode*>* get_podcast() { return &m_podcast; } // getter
	//todo: check if a setter is needed (podcast should be set only by admin and not by user)
};



/*
* Hash table is not needed here, since we need the elements to be in alphabetically order.
The implementation described here:
A hash table can be implemented using the std::unordered_set class in the Standard Template Library (STL) in C++.
The std::unordered_set class is similar to the std::set class,
but it is implemented as a hash table instead of a balanced binary search tree,
which allows for faster insertion, deletion, and search operations.
*/

#endif// PODCAST_H
