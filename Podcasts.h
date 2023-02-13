#ifndef PODCASTS_H
#define PODCASTS_H
#include "Podcast.h"

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


class Podcasts {

private:
	unordered_set<Podcast*> Podcasts; //todo: maybe it should be a part of the library? maybe a part of the server?


public:

	void delete_podcast(); //todo: delete from library? i don't think we can delete from the server..
	void play(); // play podcast // todo: add a feature that the user will be able to choose an episode
								 // (just like picking a song from a playlist) 
	

};





#endif// PODCASTS_H
