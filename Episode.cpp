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

Episode::Episode(string file_path, string episode_name, int release_date, string duration)
	:AudioFile(episode_name, file_path, duration, release_date) { }
