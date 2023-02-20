#include "Episode.h"

//const string& Episode::get_series_name() const
//{
//	return m_podcast_series_name;
//}
//
//void Episode::set_series_name(const string& podcast_name)
//{
//	m_podcast_series_name = podcast_name;
//}

Episode::Episode(string file_path, string episode_name, Podcast* podcast, int release_date, string duration)
	:AudioFile(episode_name, file_path, duration, release_date), m_podcast(podcast) { }

Podcast* Episode::Get_Podcast() const
{
	return m_podcast;
}
