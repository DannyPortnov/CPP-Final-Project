
#include "AudioCollection.h"
#include "Song.h"
#include <unordered_map>


class Playlists : AudioCollection<Song>
{
private:
	string m_name;
	static unordered_map<string, bool> playlist_names; // hash table for the names of the playlist- to check if a name exists
													   // static: in order to control the name thorough every plalist we create
public:
	Playlists(string name);
	void add_playlist(string name);
	void delete_playlist(string name);
	void Play();
	
};

/*
If you want to use a hash table in C++ to search for the keys only and not the values,
you can use the count method of the unordered_map class.
The count method returns the number of elements in the map with the specified key,
so if the count is not zero, that means the key is present in the map.
*/
//#include <iostream>
//#include <string>
//
//int main() {
//	std::unordered_map<std::string, int> map;
//	map["apple"] = 1;
//	map["banana"] = 2;
//	map["cherry"] = 3;
//
//	std::string key = "banana";
//	if (map.count(key) != 0) {
//		std::cout << "Key " << key << " is present in the map" << std::endl;
//	}
//	else {
//		std::cout << "Key " << key << " is not present in the map" << std::endl;
//	}
//
//	return 0;
//}

