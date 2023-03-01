#include "Library.h"
#include "Playlist.h"
#include "Trash.h"
#define Trash_Name "Trash"


Trash::Trash(Library* library, Server* server) : Playlist(Trash_Name, library, server, true) {}


void Trash::add_song_to_playlist(Song* song) {
	std::string prompt = "Did you mean to move this song to trash? y/n: ";
	std::string reject_message = "The song wasn't added to " + m_playlist_name + "!";
	std::string accept_message = "";
	if (Utilities::user_prompts_and_dialog(prompt, reject_message, accept_message)) {
		add_to_trash(song);
		return;
	}
}

// will be called when the user would like to restore a song from trash
void Trash::remove_song_from_playlist(Song* song) {
	cout << "You chose to restore the song: " << std::endl;
	cout << *song << std::endl;
	std::string prompt = "Are you sure that you want to restore this song? y/n: ";
	std::string reject_message = "Song is still in Trash";
	std::string accept_message = "Song was successfully restored!";
	if (Utilities::user_prompts_and_dialog(prompt, reject_message, accept_message)) {
		Playlist::remove_song_from_playlist(song, false);
		return;
	}
}

// called when trying to delete Deleted songs playlist
void Trash::clear_all_playlist() {
	std::string prompt = "Are you sure that you want to empty Trash and permanently delete it's content? y/n: ";
	std::string reject_message = "Trash list wasn't emptied!";
	std::string accept_message = "Trash is empty, all songs were permanently deleted!";
	if (Utilities::user_prompts_and_dialog(prompt, reject_message, accept_message)) {
		multiset<Song*>::iterator it;
		for (it = m_songs.begin(); it != m_songs.end(); it++) {
			m_server->Permanent_Delete_Song(*it);
			m_library->remove_from_daily_mix(*it);
			
		}
		m_library->update_most_recent();
		m_library->update_most_played();
		return;
	}
	m_songs.clear();
}

// called when we want to add song to the Trash
void Trash::add_to_trash(Song* song) {
	Playlist::add_song_to_playlist(song);

}
