#ifndef MOSTRECENT_H
#define MOSTRECENT_H

class Automatic_Playlist;

class Most_Recent :
    public Automatic_Playlist
{
private:
    

public:
    Most_Recent(Library* library, Server* server);

    void Update_Automatic_Playlist();
    //remove a song from recents by using song id
    void Remove_From_Most_Recent(int id);
    //add a song to recents by using song id
    void Add_To_Most_Recent(int id);

};

#endif// MOSTRECENT_H