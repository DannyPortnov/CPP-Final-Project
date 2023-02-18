#pragma once
#include "AudioFile.h"

class Episode :
    public AudioFile
{
private:
    //string m_podcast_series_name;
public:
    ////Returns the podcast's name to which the episode belongs to
    //const string& get_series_name() const;
    //void set_series_name(const string& podcast_name);
    //Update the podcast's song
    Episode(string file_path, string episode_name, int release_date = 0, string duration = "");
    //Plays the episode
    void Play();
};

