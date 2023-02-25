#ifndef DAILYMIX_H
#define DAILYMIX_H
#define   _CRTDBG_MAP_ALLOC
#include "Date.h"
#include <crtdbg.h>
#include <time.h>
#include <random>
#include <stdbool.h> 
//#include "Playlist.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG


#define max_songs 10

// use time.h, date classes in order to check the current date 
class Playlist;
class DailyMix : public Playlist 
{
private:
	//unordered_map<int, Song*> m_daily_mix; //first: id, second: song
	ifstream m_dailymix_file;
	Date m_last_date_saved; // stores the last saved date (will change if the program will run in a different day)
	bool check_if_date_changed(Date& new_date); // checks if the date has changed.
	Date& get_date_from_file();
public:
	DailyMix(Library* library, Server* server); // Partially tested and works
	//void Play(); from playlist
	//void Play_Random(); from playlist
	void generate_daily_mix(); //works // generate a random mix of 10 songs from the library/server
	void set_date(); // sets the date- checks if day was changed.
	void remove_song_from_playlist(Song* song, bool make_sure = true);
	//void save_dailymix();
	void save_playlist(string file_name, ios_base::openmode mode = ios::out);
	void restore_playlist();
};


#endif// DAILYMIX_H