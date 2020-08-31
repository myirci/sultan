#include "engine.hpp"
#include "utility.hpp"

#include <sstream>
#include <algorithm>

Engine::Engine() : game{ std::make_unique<Game>() } { }

void Engine::run() 
{
	print_header();
	bool continue_running { true };
	std::string ecmd{ "" };
	bool newgame{ false };

	do 
	{
		std::getline(std::cin, ecmd, '\n');
		communication_history.push_back(std::make_pair(message_direction::gui_to_engine, ecmd));
		auto tokens = Utility::tokenize(ecmd);
		for (auto it = tokens.begin(); it != tokens.end(); it++) 
		{
			if (*it == "quit")
			{
				continue_running = false;
				break;
			}
			else if (*it == "ucinewgame")
			{
				newgame = true;
			}
			else if (*it == "stop")
			{
				
			}
			else if (*it == "ponderhint")
			{

			}
			else if (*it == "position")
			{
				if (newgame)
				{
					it++;
					if (*it == "startpos")
					{
						game.reset(nullptr);
						game = std::make_unique<Game>();
					}
					else if (*it == "fen")
					{
						game.reset(nullptr);
						std::string fen_str{""};
						while (*it != "moves" || it != tokens.end()) 
						{
							fen_str += *it;
							it++;
						}
						game = std::make_unique<Game>(fen_str);
					}

					if (*it == "moves") 
					{
						for (; it != tokens.end(); it++)
							game->make_move(*it);
					}
					newgame = false;
				}
				else 
				{
					while (*it != "moves" || it != tokens.end()) { it++; }
					if (it != tokens.end())
						game->make_move(tokens.back());
				}
			}
			else if (*it == "go")
			{
				// cout << "bestmove b1a3" 
			}
			else if (*it == "register")
			{
				// do nothing for the time being
			}
			else if (*it == "setoption")
			{
				// do nothing for the time being
			}
			else if (*it == "debug")
			{
				it++;
				if (*it == "on") debug_mode = true;
				if (*it == "off") debug_mode = false;
			}
			else if (*it == "isready")
			{
				std::string gcmd{ "readyok\n" };
				communication_history.push_back(std::make_pair(message_direction::engine_to_gui, gcmd));
				std::cout << gcmd;
			}
			else if (*it == "uci")
			{
				std::stringstream id_command1{ "id name " }, id_command2{ "id author "};
				id_command1 << ENGINE_NAME << " " << VERSION << "." << SUBVERSION << "\n";
				id_command2 << AUTHOR_NAME << "\n";
				std::string gcmd{ "uciok\n" };
				communication_history.push_back(std::make_pair(message_direction::engine_to_gui, id_command1.str()));
				communication_history.push_back(std::make_pair(message_direction::engine_to_gui, id_command2.str()));
				communication_history.push_back(std::make_pair(message_direction::engine_to_gui, gcmd));
				std::cout << id_command1.str();
				std::cout << id_command2.str();
				std::cout << gcmd;
			}
			else 
			{
				std::cout << "unknown command: " << *it << std::endl;
			}
		}
	} while (continue_running);
}

void Engine::print_header() const
{
	std::cout << ENGINE_NAME << " " << VERSION << "." << SUBVERSION << std::endl;
	std::cout << "Developed by " << AUTHOR_NAME << " - myirci@gmail.com" << std::endl;
}