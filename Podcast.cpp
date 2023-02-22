#include "Podcast.h"
#include "Episode.h"

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
	//m_episodes_file_paths.insert(episode->get_path());
}

void Podcast::Delete_Episode(Episode* episode)
{
	//if (m_podcast_episodes.count(episode) == 0) {
	//	throw exception();
	//}
	m_podcast_episodes.erase(episode);
	delete episode;
}

//Chooses the first episode and returns the podcast's name from that
const string& Podcast::Get_Podcast_Name()
{
	return m_podcast_name;
}
//bool Podcast::Is_Episode_In_Podcast(const string& episode_name)
//{
//	return m_episode_names.count(episode_name) > 0;
//}
//Updates for each episode the series name
void Podcast::Set_Podcast_Name(const string& new_name)
{
	m_podcast_name = new_name;
	//for (auto itr = m_podcast_episodes.begin(); itr != m_podcast_episodes.end(); itr++)
	//{
	//	(*itr)->set_series_name(new_name);
	//}
}

void Podcast::Play()
{
	for (Episode* episode : m_podcast_episodes) {
		episode->Play();
		cout << "Would you like to continue playing " << m_podcast_name << "? y if yes, anything else no ";
		char answer;
		cin >> answer;
		if (answer != 'y') {
			return;
		}
	}
}
