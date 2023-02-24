#include "Favorites.h"
Favorites::Favorites(Library* library) : Playlist(typeid(this).name(), library)
{
}

Favorites::Favorites() : Playlist(typeid(this).name())
{
}
