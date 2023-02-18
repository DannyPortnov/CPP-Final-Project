#include "Podcast.h"

void Podcast::Add_Episode(Episode* episode)
{
	m_podcast.insert(episode);
}

string& Podcast::Get_Podcast_Name()
{
	auto episode = *(m_podcast.begin());
	auto name = episode
}
