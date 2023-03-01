#include "Library.h"
#include "Playlist.h"
#include "Favorites.h"

#define Favorites_name "Favorites"

Favorites::Favorites(Library* library, Server* server) : Playlist(Favorites_name, library, server, true) 
{
	
}

