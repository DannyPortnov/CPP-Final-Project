#ifndef DAILYMIX_H
#define DAILYMIX_H
#include "Server.h"
#include <time.h>
#define max_songs 10

// use time.h, date classes in order to check the current date 

class DailyMix {
private:
	static unordered_multimap<int, Song*> m_daily_mix; //first: id, second: song


public:
	DailyMix();
	void Play();
	void Play_Random();
	void generate_daily_mix(); // generate a random mix of 10 songs from the library/server


};


#endif// DAILYMIX_H