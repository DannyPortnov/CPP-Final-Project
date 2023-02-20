#ifndef EPISODE_H
#define EPISODE_H
#include "AudioFile.h"
#include "Podcast.h"

class Episode :
    public AudioFile
{
private:
    //The podcast to which the episode belongs
    Podcast* m_podcast;
public:
    ////Returns the podcast's name to which the episode belongs to
    //const string& get_series_name() const;
    //void set_series_name(const string& podcast_name);
    //Update the podcast's song
    Episode(string file_path, string episode_name, Podcast* podcast, int release_date = 0, string duration = "");
    Podcast* Get_Podcast() const;
    //Plays the episode
    void Play();
};
#endif

