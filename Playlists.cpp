#include "Playlists.h"

// todo: improve implementation, this one is super basic.
Playlists::Playlists() : m_favorites(new Playlist("favorites")), m_daily_mix(new Playlist("daily mix")),
m_deleted(new Playlist("deleted")) , m_recent(new Playlist("recent")), m_most_played(new Playlist("most played")) {}
//todo: add getters for each playlists
Playlists::~Playlists() {
	delete m_daily_mix;
	delete m_deleted;
	delete m_favorites;
	delete m_most_played;
	delete m_recent;
	if (user_playlists.size()) {
		set<Playlist*>::iterator it;
		for (it = user_playlists.begin(); it != user_playlists.end(); it++) {
			delete* it;
		}
	}
	playlist_names.clear();
	user_playlists.clear();
}

// add a new playlist
void Playlists::Add(const string& playlist_name) {
	if (playlist_names.count(playlist_name) != 0) {	// the count method returns 0 if ther's in no such element in the set.
		cout << "A playlist with the name: " << playlist_name << " is already exists!" << endl; // todo: change to try, throw
	}
	else {
		playlist_names.insert(playlist_name);
		Playlist* new_playlist = new Playlist(playlist_name);
		user_playlists.insert(new_playlist);
	}
}

//delete a playlist
void Playlists::Delete(Playlist* playlist) {
	if (playlist_names.count(playlist->get_name()) != 0) {
		playlist_names.erase(playlist->get_name());
		user_playlists.erase(playlist);
		delete playlist;
		cout << "Playlist Was Successfully Deleted!" << endl;

	}
	else if (playlist->get_name() == m_favorites->get_name() || playlist->get_name() == m_recent->get_name() ||
		playlist->get_name() == m_daily_mix->get_name() || playlist->get_name() == m_most_played->get_name()) {
		cout << "This Playlist Cannot Be Deleted!" << endl;
	}
	else if (playlist->get_name() == m_deleted->get_name()) {
		playlist->delete_playlist();
	}	
}

// print all of the playlists exist.
void Playlists::Print() {
	int i = 1;
	cout << "List Of Playlists:" << endl;
	cout << "(" << i << "). " << m_favorites->get_name() << endl;
	i++;
	cout << "(" << i << "). " << m_most_played->get_name() << endl;
	i++;
	cout << "(" << i << "). " << m_recent->get_name() << endl;
	i++;
	set<string>::iterator it;
	for (it = playlist_names.begin(); it != playlist_names.end(); it++) {
		cout << "(" << i << "). " << *it << endl;
		i++;
	}
	cout << "(" << i << "). " << m_deleted->get_name() << endl;
}

set<Playlist*>* Playlists::get_user_playlists()
{
	return &user_playlists;
}


