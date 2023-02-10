#include "AudioCollection.h"
#include "Song.h"


class Playlist : AudioCollection<Song>
{
private:
	string m_name;
	Player m_player;

public:
	void Play();
	string get_name() { return m_name; }

};

