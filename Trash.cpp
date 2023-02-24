#include "Trash.h"

Trash::Trash(Library* library) : Playlist(typeid(this).name(), library) {}

Trash::Trash() : Playlist(typeid(this).name()) {}


void Trash::add_song_to_playlist(Song* song) {
	string prompt = "Did you mean to move this song to trash? y/n: ";
	string reject_message = "The song wasn't added to " + m_playlist_name + "!";
	string accept_message = "";
	if (Utilities::user_prompts_and_dialog(prompt, reject_message, accept_message)) {
		add_to_trash(song);
		return;
	}
}

// will be called when the user would like to restore a song from trash
void Trash::remove_song_from_playlist(Song* song) {
	cout << "You chose to restore the song: " << endl;
	cout << *song << endl;
	string prompt = "Are you sure that you want to restore this song? y/n: ";
	string reject_message = "Song is still in Trash";
	string accept_message = "Song was successfully restored!";
	if (Utilities::user_prompts_and_dialog(prompt, reject_message, accept_message)) {
		Playlist::remove_song_from_playlist(song, false);
		return;
	}
}

// called when trying to delete Deleted songs playlist
void Trash::clear_all_playlist() {
	string prompt = "Are you sure that you want to empty Trash and permanently delete it's content? y/n: ";
	string reject_message = "Trash list wasn't emptied!";
	string accept_message = "Trash is empty, all songs were permanently deleted!";
	if (Utilities::user_prompts_and_dialog(prompt, reject_message, accept_message)) {
		multiset<Song*>::iterator it;
		for (it = m_songs.begin(); it != m_songs.end(); it++) {
			Server::Permanent_Delete_Song(*it);
			//todo: need to remove song from most_played, moset_recent playlists
		}
		return;
	}
}

// called when we want to add song to deleted playlist
void Trash::add_to_trash(Song* song) {
	string prompt = "Are you sure that you want to move this song to trash? y/n: ";
	string reject_message = "The song wasn't added to " + m_playlist_name + "!";
	string accept_message = "The song was successfully added to " + m_playlist_name + "!";
	if (Utilities::user_prompts_and_dialog(prompt, reject_message, accept_message)) {
		Playlist::add_song_to_playlist(song);
		return;
	}
}
