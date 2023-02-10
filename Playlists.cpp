#include "Playlists.h"

// todo: improve implementation, this one is super basic.
Playlists::Playlists(string name) {
	if (playlist_names.count(name) != 0) {
		std::cout << "Key " << name << " is already exists!" << endl; // todo: change to try, throw
	}
	else {
		m_name = name;
		playlist_names[name] = true;
	}

}