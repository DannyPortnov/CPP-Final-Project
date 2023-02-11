#ifndef AUDIOCOLLECTION_H
#define AUDIOCOLLECTION_H
#include <set>
#include <vector>
#include <string>
using namespace std;

template <class T>
class AudioCollection
{
protected:
	multiset<T*> audio_files;
public:
	virtual void Play() = 0;
};

#endif
