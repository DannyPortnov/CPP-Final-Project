#ifndef EPISODE_H
#define EPISODE_H
#include "AudioFile.h"
#include "Podcast.h"

class Episode :
    public AudioFile
{

private:
    friend ostream& operator<<(ostream& os, const Episode& episode);
    static int episode_code;
    //The podcast to which the episode belongs
    Podcast* m_podcast;
public:
    ////Returns the podcast's name to which the episode belongs to
    //const std::string& get_series_name() const;
    //void set_series_name(const std::string& podcast_name);
    //Update the podcast's song
    Episode(std::string file_path, std::string episode_name, Podcast* podcast, std::string release_date = "", std::string duration = "");
    Podcast* Get_Podcast() const;
    //Plays the episode
    void Play();
};
ostream& operator<<(ostream& os, const Episode& episode);
#endif

