#ifndef MP3INTERFACE_H
#define MP3INTERFACE_H
#define NOMINMAX //HAS TO BE FIRST
#include "Utilities.h"
#include "MethodMap.h"
#include "Library.h"
#include <crtdbg.h>
#include <iostream>
#include <fstream>
#include <string>

class Library;
class Mp3Interface
{
private:
	Library* m_lib;
	Server* m_server;
	DailyMix* m_daily_mix;
	void Print_Invalid_Command_Error(const std::string& input);
	void DailyMix_Menu();
	void Search_Menu();
	void Main_Menu();
	void Podcasts_Menu();
	void Library_Menu();
	void Print_Library_Menu();
	void Print_Playlists_Menu();
	void Playlists_Menu();
public:
	Mp3Interface(Library* lib);
	void Run_Program();
};
#endif