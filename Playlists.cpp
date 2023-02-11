#include "Playlists.h"

// todo: improve implementation, this one is super basic.
Playlists::Playlists(Playlist* playlist) {
	Add(playlist);
}

// add a new playlist
void Playlists::Add(Playlist* playlist) {
	if (playlist_names.count(playlist) != 0) {	// the count method returns 0 if ther's in no such element in the set.
		cout << "A playlist with the name: " << playlist->get_name() << " is already exists!" << endl; // todo: change to try, throw
	}
	else {
		playlist_names.insert(playlist);
	}
}

void Playlists::Delete(Playlist* playlist) {
	playlist_names.erase(playlist);
}


