#ifndef EPISODE_H
#define EPISODE_H
#include "AudioFile.h"
#include "Podcast.h"

class Episode :
    public AudioFile
{

private:
    static int episode_code;
    //The podcast to which the episode belongs
    Podcast* m_podcast;
public:
    //Update the podcast's song
    Episode(std::string file_path, std::string episode_name, Podcast* podcast, std::string release_date = "", std::string duration = "");
    Podcast* Get_Podcast() const;
};
#endif

