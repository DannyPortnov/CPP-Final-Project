#ifndef PODCAST_H
#define PODCAST_H
#include "AudioFile.h"
#include "Player.h"
#include <string>
#include <iostream>
#include <unordered_set>

//each podcast is a collection of episodes!
class Episode;
class Podcast 
{
	friend std::ostream& operator<<(std::ostream& os, const Podcast& podcast);
private:
	std::unordered_set<Episode*> m_podcast_episodes;
	std::string m_podcast_name;
public:
	Podcast(std::string podcast_name);
	~Podcast();
	//Adds a UNIQUE episode to the podcast
	void Add_Episode(Episode* episode);
	//Removes an episode from Podcast and frees memory (if doesn't exist does nothing)
	void Delete_Episode(Episode* episode);
	const std::string& Get_Podcast_Name();
	//bool Is_Episode_In_Podcast(const std::string& episode_name);
	void Set_Podcast_Name(const std::string& new_name);
	unordered_set<Episode*>* get_podcast() { return &m_podcast_episodes; } // getter
	void Play();
};
std::ostream& operator<<(std::ostream& os, const Podcast& podcast);



#endif// PODCAST_H
