#ifndef AUDIOCOLLECTION_H
#define AUDIOCOLLECTION_H
#include <set>
#include <vector>
#include <string>
using namespace std;

//todo: add Podcast class, will inherit from AudioCollection
//todo: add more meat here
//todo: podcasts will be like playlists

template <class T>
class AudioCollection
{
protected:
	multiset<T*> audio_files;
public:
	virtual void Play() = 0;
};

#endif
