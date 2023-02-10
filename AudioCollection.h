#ifndef AUDIOCOLLECTION_H
#define AUDIOCOLLECTION_H
#include <map>
#include <string>
using namespace std;

template <class T>
class AudioCollection
{
protected:
	multimap<string,T*> audio_files;
public:
	virtual void Play() = 0;
};

#endif
