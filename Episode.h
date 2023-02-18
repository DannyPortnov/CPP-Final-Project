#pragma once
#include "AudioFile.h"

class Episode :
    public AudioFile
{
private:
    string m_podcast_series_name;
public:
    string get_series_name();
};

