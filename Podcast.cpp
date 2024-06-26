#include "Podcast.h"
#include "Episode.h"

Podcast::Podcast(std::string podcast_name) : m_podcast_name(podcast_name), m_podcast_episodes() {}

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
	m_podcast_episodes.insert(episode);
}

void Podcast::Delete_Episode(Episode* episode)
{
	//If the episode doesn't exist then does nothing
	m_podcast_episodes.erase(episode);
	delete episode;
}

//Chooses the first episode and returns the podcast's name from that
const std::string& Podcast::Get_Podcast_Name()
{
	return m_podcast_name;
}

//Updates for each episode the series name
void Podcast::Set_Podcast_Name(const std::string& new_name)
{
	m_podcast_name = new_name;
}

void Podcast::Play()
{
	std::cout << "Playing " << m_podcast_name << std::endl;
	bool ask_to_continue = false;
	for (Episode* episode : m_podcast_episodes) {
		if (ask_to_continue)
		{
			std::cout << "Would you like to continue playing " << m_podcast_name << "? y if yes, anything else no ";
			char answer;
			cin >> answer;
			if (answer != 'y') {
				return;
			}
		}
		episode->Play();
		ask_to_continue = true; //only ask if there's more episodes to play. Don't ask before the first play!
	}
	std::cout << "Finished playing " << m_podcast_name << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Podcast& podcast)
{
	os << "Podcast name: " << podcast.m_podcast_name << std::endl;
	os << "Has the following episodes: " << std::endl;
	for (Episode* episode : podcast.m_podcast_episodes) {

		os << *episode << std::endl;
	}
	return os;
}
