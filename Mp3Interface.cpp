#include "Mp3Interface.h"
#include "Library.h"
#include "Playlist.h"
#include "DailyMix.h"

Mp3Interface::Mp3Interface()
	: m_lib(), m_server(m_lib.Get_Server()), m_daily_mix(m_lib.Get_DailyMix()) {}


void Mp3Interface::Main_Menu() {
	std::cout << "Main Menu: " << std::endl;
	std::cout << " Daily Mix" << std::endl;
	std::cout << " Search" << std::endl;
	std::cout << " Podcast" << std::endl;
	std::cout << " Library" << std::endl;
	std::cout << " Playlists" << std::endl;
	std::cout << " Help" << std::endl;
	std::cout << " Exit\n" << std::endl;
}


void Mp3Interface::Run_Program() {
	std::string answer;
	bool run_program = true;
	while (run_program) {
		std::cout << "Welcome To mp3player :)" << std::endl;
		std::cout << std::endl;
		Main_Menu();
		std::getline(std::cin, answer, '\n');
		switch (Utilities::hashit(answer))
		{
		case(eDailyMix): {
			DailyMix_Menu();
			continue;
		}
		case(eSearch): {
			Search_Menu();
			continue;
		}
		case(ePodcast): {
			Podcasts_Menu();
			continue;
		}
		case(eLibrary): {
			Library_Menu();
			continue;
		}
		case(ePlaylists): {
			Playlists_Menu();
			continue;
		}
		case(eHelp): {
			Main_Menu();
			continue;
		}
		case(eExit): {
			run_program = false;
			continue;
		}
		}
		std::cout << answer << " isn't a valid command." << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

void Mp3Interface::Print_Playlists_Menu() {
	std::cout << "Playlist Menu:" << std::endl;
	std::cout << "> Add <playlist_name>" << std::endl;
	std::cout << "> Delete <playlist_name>" << std::endl;
	std::cout << "> Play <playlist_name>" << std::endl;
	std::cout << "> PlayRandom <playlist_name>" << std::endl;
	std::cout << "> Print <playlist_name>" << std::endl;
	std::cout << "> Help" << std::endl;
	std::cout << "> Back" << std::endl;
}

void Mp3Interface::Playlists_Menu() {
	bool repeat = true;
	while (repeat) {
		Print_Playlists_Menu();
		std::cout << std::endl;
		std::string answer, command, playlist_name;
		// Create a regex pattern to match the input string and capture the command and the rest of the string
		std::regex pattern(R"(^(Back$|Help$|Add|Delete|Print|Play|PlayRandom)\s*(.*)$)");
		//ignore has to be OUTSIDE the loop!
		//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //Add #define NOMINMAX first thing in header (good practice)
		std::cout << "Type your selection:" << std::endl;
		std::getline(std::cin, answer);
		smatch matches; // Match the input string against the regex pattern
		if (regex_match(answer, matches, pattern)) {
			// Extract the command and the rest of the input string
			command = matches[1];
			playlist_name = matches[2];
			switch (Utilities::hashit(command))
			{
				// the parameter is the name of the playlist in all cases
			case(eAdd): {
				m_lib.create_playlist(playlist_name);
				continue;
			}
			case(eDelete): {
				m_lib.delete_playlist(playlist_name);
				continue;
			}
			case(ePlay): {
				m_lib.PlayPlaylist(playlist_name);
				continue;
			}
			case(ePlayRandom): {
				m_lib.PlayPlaylistShuffled(playlist_name);
				continue;
			}
			case(ePrint): {
				m_lib.PrintPlaylist(playlist_name); // this function uses operator overload << for playlist, check if the playlist exist
				continue;
			}
			case(eHelp): {
				Print_Playlists_Menu();
				continue;
			}
			case(eBack): {
				repeat = false;
				continue;
			}
			}
		}
		Print_Invalid_Command_Error(answer);
	}
}


void Mp3Interface::Print_Library_Menu() {
	std::cout << "Library Menu:" << std::endl;
	std::cout << m_lib << std::endl; // print the first 10 songs in alphabetically order using operator overload <<
	std::cout << "> More" << std::endl;
	std::cout << "> List" << std::endl;
	std::cout << "> Add filename_fullpath song_name singer=<singer> album=<album> genre=<genre> duration=<mm:ss> release_date=<dd/mm/yyyy>" << std::endl;
	//std::cout << "> AddEpisode filename_fullpath episode_name podcast_name duration=<mm:ss> release_date=<dd/mm/yyyy>" << std::endl;
	std::cout << "> Update song_name name = <name> singer=<singer> album=<album> genre=<genre> duration=<mm:ss> release_date=<dd/mm/yyyy>" << std::endl;
	std::cout << "> Update song_id name = <name> singer=<singer> album=<album> genre=<genre> duration=<mm:ss> release_date=<dd/mm/yyyy>" << std::endl;
	std::cout << "> Delete <unique_id>" << std::endl;
	std::cout << "> Delete <song_name>" << std::endl;
	std::cout << "> PrintSong <unique_id>" << std::endl;
	std::cout << "> PrintSong <song_name>" << std::endl;
	std::cout << "> Add2PL <unique_id> <playlist_name>" << std::endl;
	std::cout << "> RemoveFromPL song=<song_name> playlist=<playlist_name>" << std::endl;
	std::cout << "> PrintPL" << std::endl;
	std::cout << "> Play <song_name>" << std::endl;
	std::cout << "> Play <song_id>" << std::endl;
	std::cout << "> PlayAll" << std::endl;
	std::cout << "> PlayRandom" << std::endl;
	std::cout << "> Help" << std::endl;
	std::cout << "> Back" << std::endl;

}

#define number_of_songs_to_print 10

void Mp3Interface::Library_Menu() { //todo: move some make_sure text to here
	bool repeat = true;
	int begin = 0, end = number_of_songs_to_print;
	while (repeat) {
		Print_Library_Menu();
		std::cout << std::endl;
		std::string answer, command, parameters;
		// Create a regex pattern to match the input string and capture the command and the rest of the string
		//std::regex pattern(R"(^((Back$)|(Help$)|([^Back\s)][^(Help\s)]\w+))\s+(.*)$)");
		std::regex pattern(R"(^(Back$|Help$|More$|List$|Add|Update|Delete|PrintSong|Add2PL|PrintPL|RemoveFromPL|Play|PlayAll|PlayRandom)\s*(.*)$)");
		//regex pattern(R"(^\s*(\w+)\s*(.*)$)");//Match zero or more spaces,
		//captures one or more word chars, then match zero or more spaces and capture everything untill the end
		//ignore has to be OUTSIDE the loop!
		//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //Add #define NOMINMAX first thing in header (good practice)
		std::cout << "Type your selection:" << std::endl;
		std::getline(std::cin, answer);
		smatch matches; // Match the input string against the regex pattern
		if (regex_match(answer, matches, pattern)) {
			// Extract the command and the rest of the input string
			command = matches[1];
			parameters = matches[2];
			switch (Utilities::hashit(command))
			{
				case(eMore): {
					begin += number_of_songs_to_print;
					end += number_of_songs_to_print;
					//std::cout << " here are another 10 songs from library:" << std::endl;
					m_lib.Print(std::cout, begin, end); // print the first 10 songs in alphabetically order using operator overload <<
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //add bool for first print and add to the beginning of loop
					continue;
				}
				case(eList): {
					begin = 0;
					end = number_of_songs_to_print;
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					continue;
				}
				case(eAdd): {
					begin = 0;
					end = number_of_songs_to_print;
					std::string rest_of_string, song_name, file_path, artist, album, genre, duration, release_date;
					// Define the regular expression pattern
					std::regex pattern("^(.+\\.mp3)\\s(.+)$");// works for seperatring file path from rest of the string
					// Extract the different parts of the input string using regex
					smatch matches;
					if (regex_match(parameters, matches, pattern)) {
						file_path = matches[1].str();
						rest_of_string = matches[2].str();
						// extract artist name
						regex pattern1("^(.*?)\\s*(singer=\\s*(.*?))?\\s*(album=\\s*(.*?))?\\s*(genre=\\s*(.*?))?\\s*(duration=\\s*(.*?))?\\s*(release_date=\\s*(.*?))?$");
						//matches;
						if (regex_match(rest_of_string, matches, pattern1)) {
							song_name = matches[1].str();
							artist = matches[3].str();
							album = matches[5].str();
							genre = matches[7].str();
							duration = matches[9].str();
							release_date = matches[11].str();
							m_lib.Add_Song(song_name, file_path, artist, album, genre, duration, release_date);
						}
					}
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					continue;
				}
				case(eUpdate): {
					begin = 0;
					end = number_of_songs_to_print;
					std::string update_by_name_or_id, new_name, file_path, artist, album, genre, duration, release_date;
					// Define regex pattern
					std::regex pattern("^(.*?)\\s*(name=\\s*(.*?))?\\s*(singer=\\s*(.*?))?\\s*(album=\\s*(.*?))?\\s*(genre=\\s*(.*?))?\\s*(duration=\\s*(.*?))?\\s*(release_date=\\s*(.*?))?$");
					// Create regex match object
					// Execute regex search
					if (regex_match(parameters, matches, pattern)) {
						update_by_name_or_id = matches[1].str();
						new_name = matches[3].str();
						artist = matches[5].str();
						album = matches[7].str();
						genre = matches[9].str();
						duration = matches[11].str();
						release_date = matches[13].str();
						try {
							int id = std::stoi(update_by_name_or_id);
							m_lib.Update_Song(id, new_name, artist, album, genre, duration, release_date);
						}
						catch (std::invalid_argument& e) {
							// Handle the exception if the input string is not a valid integer-> call the overload function
							m_lib.Update_Song(update_by_name_or_id, new_name, artist, album, genre, duration, release_date);
						}
					}
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					continue;
				}
				case(eDelete): {
					begin = 0;
					end = number_of_songs_to_print;
					// Define regex pattern
					std::regex pattern("^([a-zA-Z0-9 _]+)$");
					// Create regex match object
					std::smatch match;

					// Execute regex search
					if (std::regex_search(parameters, match, pattern)) {
						std::string delete_by_name_or_id = match[1]; // Extract command
						try {
							int id = std::stoi(delete_by_name_or_id);
							m_lib.Delete_Song(id);
						}
						catch (std::invalid_argument& e) {
							// Handle the exception if the input string is not a valid integer-> call the overload function
							m_lib.Delete_Song(delete_by_name_or_id);
						}
					}
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					continue;
				}
				case(ePrintSong): {
					begin = 0;
					end = number_of_songs_to_print;
					// Define regex pattern
					std::regex pattern("^([a-zA-Z0-9 _]+)$");
					// Create regex match object
					std::smatch match;

					// Execute regex search
					if (std::regex_search(parameters, match, pattern)) {
						std::string print_by_name_or_id = match[1]; // Extract command
						try {
							int id = std::stoi(print_by_name_or_id);
							m_lib.PrintSong(id);
						}
						catch (std::invalid_argument& e) {
							// Handle the exception if the input string is not a valid integer-> call the overload function
							m_lib.PrintSong(print_by_name_or_id);
						}
					}
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					continue;
				}
				case(eAdd2PL): {
					begin = 0;
					end = number_of_songs_to_print;
					std::string song_id, playlist_name;
					// Define the regular expression to match the song ID and name
					std::regex pattern(R"((\d+)\s+(.*))");
					// Match the input string against the regular expression
					std::smatch match;
					if (std::regex_match(parameters, match, pattern)) {
						// Extract the song ID and name from the match results
						song_id = match[1].str();
						playlist_name = match[2].str();
					}
					// Attempt to convert the string to an integer using stoi
					try {
						int id = std::stoi(song_id);
						m_lib.Add2PL(id, playlist_name);
					}
					catch (std::invalid_argument& e) {
						// Handle the exception if the input string is not a valid integer
						std::cout << "Invalid argument: " << e.what() << std::endl;
					}
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					continue;
				}
				case(eRemoveFromPL): {
					begin = 0;
					end = number_of_songs_to_print;
					// Initialize variables to store the song and playlist names
					std::string song_name, playlist_name;
					// Define the regular expression pattern to extract the song and playlist names
					std::regex pattern("(song)=\\s*(.*?)\\s*(playlist)=\\s*(.*?)\\s*$");
					// Search for the song name using the regular expression pattern
					std::smatch match;
					std::regex_search(parameters, match, pattern);
					// Extract the song name from the match object
					if (std::regex_match(parameters, match, pattern)) {
						song_name = match[2];
						playlist_name = match[4];
						m_lib.RemoveFromPL(song_name, playlist_name);
					}
					continue;
				}
				case(ePrintPL): {
					begin = 0;
					end = number_of_songs_to_print;
					m_lib.PrintPL();
					continue;
				}
				case(ePlay): {
					begin = 0;
					end = number_of_songs_to_print;
					// Define regex pattern
					std::regex pattern("^([a-zA-Z0-9 _]+)$");
					// Create regex match object
					std::smatch match;

					// Execute regex search
					if (std::regex_search(parameters, match, pattern)) {
						std::string play_by_name_or_id = match[1]; // Extract command
						try {
							int id = std::stoi(play_by_name_or_id);
							m_lib.Play(id);
						}
						catch (std::invalid_argument& e) {
							// Handle the exception if the input string is not a valid integer-> call the overload function
							m_lib.Play(play_by_name_or_id);
						}
					}
					continue;
				}
				case(ePlayAll): {
					begin = 0;
					end = number_of_songs_to_print;
					m_lib.PlayAll(false);
					continue;
				}
				case(ePlayRandom): {
					begin = 0;
					end = number_of_songs_to_print;
					m_lib.PlayAll(true);
					continue;
				}
				case(eHelp): {
					begin = 0;
					end = number_of_songs_to_print;
					Print_Library_Menu();
					continue;
				}
				case(eBack): {
					repeat = false;
					continue;
				}
			}
		}
		Print_Invalid_Command_Error(answer);
	}
}


//todo: add AddEpisode and UpdateEpisode
void Mp3Interface::Podcasts_Menu()
{
	// This declares a lambda, which can be called just like a function
	auto print_message = [this]() //means that 'this' is in the lambda's scope
	{
		std::cout << std::endl;
		std::unordered_map<std::string, Podcast*>::iterator itr; 	//printing all podcasts in alphabet order
		for (const auto& podcast_pair : *m_server->get_podcasts()) {
			std::cout << *podcast_pair.second;
		}
		std::cout << "\nDelete <podcast name>" << "\n";
		std::cout << "AddEpisode filename_fullpath episode_name podcast name=<podcast_name> duration=<mm:ss> release date=<dd/mm/yyyy>" << "\n";
		std::cout << "Path and names are required, duration and release date are optional" << "\n";
		std::cout << "UpdateEpisode episode_id episode name=<episode name> duration=<mm:ss> release date=<dd/mm/yyyy>" << "\n";
		std::cout << "DeleteEpisode episode_id" << "\n";
		std::cout << "DeleteEpisode episode_name" << "\n";
		std::cout << "Play <podcast name>" << "\n";
		std::cout << "Back\n" << std::endl;
	};
	bool repeat = true;
	while (repeat)
	{
		print_message();
		std::string input;
		// Clear the input buffer before reading the command input
		std::getline(std::cin, input, '\n'); //We need to use getline and '\n' in the end!
		std::regex pattern(R"(^(Back$|Help$|DeleteEpisode|Delete|Play|AddEpisode|UpdateEpisode)\s*(.*)$)");
		//std::regex pattern("^(Delete|Play|Back)\\s*(.*)"); 
		/*matches a string that starts with "Delete", "Play", or "Back", followed by
			zero or more whitespace characters, and then any characters(including whitespace characters) until the end of the string.*/
		std::smatch match;
		if (regex_search(input, match, pattern)) {
			std::string command = match[1];
			std::string parameters = match[2];
			switch (Utilities::hashit(command))
			{
			case(ePlay): {
				m_lib.Play_Podcast(parameters);
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				continue;
			}
			case(eDelete): { //podcast
				m_lib.Delete_Podcast(parameters);
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				continue;
			}
			case(eDeleteEpisode): {
				try {
					int id = std::stoi(parameters);
					m_lib.Delete_Episode(id);
				}
				catch (std::invalid_argument& e) {
					// Handle the exception if the input string is not a valid integer-> call the overload function
					m_lib.Delete_Episode(parameters);
				}
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				continue;
			}
			case(eUpdateEpisode): {
				std::string id_string, new_name, duration, release_date;
				// Define regex pattern
				std::regex pattern("^(.*?)\\s*(episode name=\\s*(.*?))?\\s*(duration=\\s*(.*?))?\\s*(release date=\\s*(.*?))?$");
				// Create regex match object
				std::smatch matches;
				// Execute regex search
				if (regex_match(parameters, matches, pattern)) {
					id_string = matches[1].str();
					new_name = matches[3].str();
					duration = matches[5].str();
					release_date = matches[7].str();
					try {
						int id = std::stoi(id_string);
						m_lib.UpdateEpisode(id, new_name, duration, release_date);
					}
					catch (std::invalid_argument& e) {
						std::cout << "id isn't a number" << std::endl;
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					}
					continue;
				}
				break;
			}
			case(eAddEpisode): {
				std::string rest_of_string, episode_name, file_path, podcast_name, duration, release_date;
				// Define the regular expression pattern
				std::regex pattern("^(.+\\.mp3)\\s(.+)$");// works for seperatring file path from rest of the string
				// Extract the different parts of the input string using regex
				smatch matches;
				if (regex_match(parameters, matches, pattern)) {
					file_path = matches[1].str();
					rest_of_string = matches[2].str();
					regex pattern1("^(.*?)\\s*(podcast name=\\s*(.+?))\\s*(duration=\\s*(.*?))?\\s*(release date=\\s*(.*?))?$");
					//matches;
					if (regex_match(rest_of_string, matches, pattern1)) {
						episode_name = matches[1].str();
						podcast_name = matches[3].str();
						duration = matches[5].str();
						release_date = matches[7].str();
						m_lib.Add_Podcast_Episode(episode_name, podcast_name, file_path, duration, release_date);
						continue;
					}
				}
				break;
			}
			case(eHelp): {
				continue;
			}
			case(eBack): {
				repeat = false;
				continue;
			}
			}
		}
		Print_Invalid_Command_Error(input); //if no match found or command doesn't fit switch case
	}
#pragma region Without regex
	//size_t space_pos = input.find(' ');
	//if (space_pos != std::string::npos) { //if found 
	//	command = input.substr(0, space_pos);
	//	podcast_name = input.substr(space_pos + 1);
	//}
	//else {
	//	return;
	//}
	//// call the appropriate function based on the command
	//if (command == "delete") {
	//	deletePodcast(podcast_name);
	//}
	//else if (command == "play") {
	//	playPodcast(podcast_name);
	//}
	//else {
	//	std::cout << "Invalid command" << std::endl;
	//}  
#pragma endregion
}

void Mp3Interface::DailyMix_Menu()
{
	// This declares a lambda, which can be called just like a function
	auto print_message = [this]()
	{
		std::cout << *m_daily_mix;
		std::cout << "\nPlay" << "\n";
		std::cout << "Random" << "\n";
		std::cout << "Back\n" << std::endl;
	};
	bool repeat = true;
	//ignore has to be OUTSIDE the loop!
	// Clear the input buffer before reading the command input
	//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //Add #define NOMINMAX first thing in header
	while (repeat)
	{
		print_message();
		string command;
		std::getline(std::cin, command, '\n'); //We need to use getline and '\n' in the end!
		switch (Utilities::hashit(command))
		{
		case(ePlay): {
			m_daily_mix->Play(false);
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		case(eRandom): {
			m_daily_mix->Play(true);
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		case(eBack): {
			repeat = false;
			continue;
		}
		}
		Print_Invalid_Command_Error(command);
	}
}

void Mp3Interface::Search_Menu() {
	// This declares a lambda, which can be called just like a function
	auto print_message = []()
	{
		std::cout << "\nSearch by name <search_text>" << "\n";
		std::cout << "Search by singer <search_text>" << "\n";
		std::cout << "Search by album <search_text>" << "\n";
		std::cout << "Search by genre <search_text>" << "\n";
		std::cout << "Back\n" << std::endl;
	};
	bool repeat = true;
	//ignore has to be OUTSIDE the loop!
	// Clear the input buffer before reading the command input
	//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //Add #define NOMINMAX first thing in header
	while (repeat)
	{
		print_message();
		string input;
		std::getline(std::cin, input, '\n'); //We need to use getline and '\n' in the end!
		std::string command, value_to_search;
		std::regex pattern(R"(^(Search by \w+\s|(Back$)|(Help$)?)(.*)$)");
		std::smatch match;
		if (regex_search(input, match, pattern)) {
			std::string command = match[1];
			std::string value_to_search = match[4];
			std::string message = "Playing all found songs";
			switch (Utilities::hashit(command))
			{
			case(eSearchByAlbum): { //todo: make one method
				auto collection = m_server->find_by_album(value_to_search);
				m_lib.PlayAll(Utilities::Values(collection), message, false, true);
				continue;
			}
			case(eSearchByGenre): {
				auto collection = m_server->find_by_genre(value_to_search);
				m_lib.PlayAll(Utilities::Values(collection), message, false, true);
				continue;
			}
			case(eSearchByName): {
				auto collection = m_server->find_by_name(value_to_search);
				m_lib.PlayAll(Utilities::Values(collection), message, false, true);
				continue;
			}
			case(eSearchBySinger): {
				auto collection = m_server->find_by_artist(value_to_search);
				m_lib.PlayAll(Utilities::Values(collection), message, false, true);
				continue;
			}
			case(eBack): {
				repeat = false;
				continue;
			}
			}
		}
		Print_Invalid_Command_Error(input);
	}
}

void Mp3Interface::Print_Invalid_Command_Error(const std::string& input) {
	std::cout << input << " isn't a valid command. Try again." << std::endl;
}

