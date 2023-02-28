#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <map>

enum string_code {
	eError,
	eDailyMix,
	eSearch,
	eSearchByName,
	eSearchBySinger,
	eSearchByAlbum,
	eSearchByGenre,
	ePodcast,
	eLibrary,
	ePlaylists,
	ePlay,
	eDelete,
	eHelp,
	eBack,
	eExit,
	eMore,
	eList,
	eAdd,
	eUpdate,
	/*eUpdateSongByName,
	eUpdateSongById,
	eDeleteSongByName,
	eDeleteSongById,
	ePrintSongByName,*/
	ePrint,
	ePrintSong, //todo: need to think if we should use ePrint only
	eAdd2PL,
	eRemoveFromPL,
	ePrintPL,
	/*ePlaySongByName,
	ePlaySongById,*/
	ePlayAll,
	ePlayRandom,
	eRandom,
};

class Utilities
{
public:
	static string_code hashit(std::string const& inString);
	//Replaces all underscores with space and vice versa in each string in the vector.
	static void Replace_All(std::vector<std::string*>& params, bool is_serialization);
	//Checks if reached the end of the file to avoid empty lines
	static bool Is_End_Of_File(std::ios_base & fileToCheck); //works
	//If the file isn't valid, prints a message.
	static bool Is_File_Valid(std::ios & fileToCheck); //works
	static bool user_prompts_and_dialog(std::string& prompt_message, std::string& reject_message,
		std::string& accept_message);// create general prompt function for dialog with user- maybe move to utilities
	//Returns true if finished reading file/empty.
	static bool Is_End_Of_File_Or_Empty(std::ifstream& fileToCheck);
	
	//Returns vector of all values inside Map (which is a map of any kind)
	template <typename Map>
	static inline std::vector<typename Map::mapped_type>* Values(Map* map) {
		std::vector<typename Map::mapped_type>* values = new std::vector<typename Map::mapped_type>();
		values->reserve(map->size());
		std::transform(map->begin(), map->end(), std::back_inserter(*values),
			[](const auto& pair) { return pair.second; });
		return values;
	}

	//template <typename Map>
	//static inline auto* Values(Map* map) {
	//	auto* values = new std::vector<decltype(map->begin()->second)>();
	//	values->reserve(map->size());
	//	std::transform(map->begin(), map->end(), std::back_inserter(*values),
	//		[](const auto& pair) { return pair.second; });
	//	return values;
	//}
	//static std::vector<Song*> Values(std::multimap<std::string, Song*>* map);
};
#endif

