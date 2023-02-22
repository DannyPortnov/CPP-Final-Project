#ifndef DAILYMIX_H
#define DAILYMIX_H
#include "Server.h"
#include <time.h>
#include "Date.h"
#define max_songs 10

// use time.h, date classes in order to check the current date 

class DailyMix {
private:
	unordered_map<int, Song*> m_daily_mix; //first: id, second: song
	Date m_last_date_saved; // stores the last saved date (will change if the program will run in a different day)
	bool check_if_date_changed(Date& new_date); // checks if the date has changed.
public:
	DailyMix();
	void Play();
	void Play_Random();
	void generate_daily_mix(); // generate a random mix of 10 songs from the library/server
	void set_date(); // sets the date- checks if day was changed.
	void remove_song_from_mix(int id);
	void save_dailymix();
};


#endif// DAILYMIX_H