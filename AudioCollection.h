#ifndef AUDIOCOLLECTION_H
#define AUDIOCOLLECTION_H
#include <set>
#include <vector>
#include <string>



template <class T>
class AudioCollection
{
protected:
	std::multiset<T*> audio_files;
public:
	virtual void Play() = 0;
};

#endif
