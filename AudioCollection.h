#ifndef AUDIOCOLLECTION_H
#define AUDIOCOLLECTION_H
#include <map>
#include <string>
using namespace std;

template <class T>
class AudioCollection
{
private:
	multimap<string,T> audioFiles;
public:
	virtual void Play() = 0;
};

#endif
