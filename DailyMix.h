#ifndef DAILYMIX_H
#define DAILYMIX_H
#include "Server.h"
#include <time.h>
#define max_songs 10

// use time.h, date classes in order to check the current date 

class DailyMix {
private:
	static unordered_multimap<string, Song*> daily_mix;


public:
	void Play();
	void Play_Random();



};


#endif// DAILYMIX_H