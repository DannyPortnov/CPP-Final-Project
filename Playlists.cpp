#include "Playlists.h"

// todo: improve implementation, this one is super basic.
Playlists::Playlists(Playlist* playlist) {
	Add(playlist);
}

Playlists::Playlists()
{
}

// add a new playlist
void Playlists::Add(Playlist* playlist) {
	if (playlist_names.count(playlist) != 0) {
		cout << "A playlist with the name: " << playlist->get_name() << " is already exists!" << endl; // todo: change to try, throw
	}
	else {
		playlist_names[playlist] = true;
	}
}

void Playlists::Delete(Playlist* playlist) {
	playlist_names.erase(playlist);
}