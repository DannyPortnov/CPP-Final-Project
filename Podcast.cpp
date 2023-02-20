#include "Podcast.h"

Podcast::Podcast(string podcast_name) : m_podcast_name(podcast_name), m_podcast_episodes() {}

Podcast::~Podcast()
{
	// Iterate over all episodes in the podcast
	for (auto& episode : m_podcast_episodes) {
		delete episode;
	}
	// Clear the collection.
	m_podcast_episodes.clear();
}

void Podcast::Add_Episode(Episode* episode)
{
	//if (m_podcast_episodes.empty()) {
	//	m_podcast_name = episode->get_series_name(); //sets the podcast's name
	//}
	m_podcast_episodes.insert(episode);
	m_episodes_file_paths.insert(episode->get_path());
}

//Chooses the first episode and returns the podcast's name from that
const string& Podcast::Get_Podcast_Name()
{
	return m_podcast_name;
}
bool Podcast::Is_Episode_In_Podcast(const string& file_path)
{
	return m_episodes_file_paths.count(file_path) > 0;
}
//Updates for each episode the series name
void Podcast::Set_Podcast_Name(const string& new_name)
{
	m_podcast_name = new_name;
	//for (auto itr = m_podcast_episodes.begin(); itr != m_podcast_episodes.end(); itr++)
	//{
	//	(*itr)->set_series_name(new_name);
	//}
}
