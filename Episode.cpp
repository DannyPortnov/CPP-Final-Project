#include "Episode.h"

int Episode::episode_code = 1;

Episode::Episode(std::string file_path, std::string episode_name, Podcast* podcast, std::string release_date, std::string duration)
	:AudioFile(episode_name, file_path, duration, release_date), m_podcast(podcast) {
	m_id_code = episode_code;
	episode_code++;
}

Podcast* Episode::Get_Podcast() const
{
	return m_podcast;
}

