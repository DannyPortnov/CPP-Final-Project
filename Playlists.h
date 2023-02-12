#ifndef PLAYLISTS_H
#define PLAYLISTS_H
#include "AudioCollection.h"
#include "Song.h"
#include <set>
#include "Playlist.h"

class Playlists
{
private:
	Playlist* m_favorites;
	Playlist* m_daily_mix;
	Playlist* m_recent;
	Playlist* m_most_played;
	Playlist* m_deleted;
	set<string> playlist_names; // check if a name exists in order to control the name through every playlist we create
	set<Playlist*> user_playlists; // store the user playlists

public:
	//Playlists(Playlist* playlist);
	Playlists();
	~Playlists();
	void Add(const string& playlist);
	void Delete(Playlist* playlist);
	Playlist* get_favorites();
	Playlist* get_daily_mix();
	Playlist* get_recent();
	Playlist* get_deleted();
	void Play();
	void Print();
	set<Playlist*>* get_user_playlists();

};
#endif// PLAYER_H




/*
The std::set class in the Standard Template Library (STL) in C++ is implemented as a balanced binary search tree.
It provides logarithmic time complexity for insertion, deletion, and search operations,
making it a good choice for situations where you need to maintain a sorted set of elements 
and need to perform these operations frequently.
Here's a summary of the time complexity for the operations provided by the std::set class:
Insertion: O(log n)
Deletion: O(log n)
Search: O(log n)
Access (i.e., iteration through the elements): O(n)

Note that n is the number of elements in the set.
In terms of the underlying data structure, the std::set class uses a balanced binary search tree,
such as a red-black tree, to store the elements. This allows the set to maintain the elements in sorted order while
providing efficient access to the elements.
*/


/*
* Hash table is not needed here, since we need the elements to be in alphabetically order.
The implementation described here:
A hash table can be implemented using the std::unordered_set class in the Standard Template Library (STL) in C++.
The std::unordered_set class is similar to the std::set class, 
but it is implemented as a hash table instead of a balanced binary search tree, 
which allows for faster insertion, deletion, and search operations.
*/
