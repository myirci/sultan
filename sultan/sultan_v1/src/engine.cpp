#include "engine.hpp"
#include "utility.hpp"
#include "factory.hpp"
#include "fen.hpp"

#include <sstream>
#include <algorithm>

Engine::Engine() : 
	last_position_cmd{ "" },
	board{ nullptr },
	game{ nullptr }, 
	debug_mode{ false } 
{
	print_header();
}

bool Engine::check_board() const 
{
	if (board) return true;
	std::cout << "Board is not initialized, use position command to set up the board" << std::endl;
	return false;
}

bool Engine::check_size(std::vector<std::string> const& tokens, size_t expected_size) const 
{
	if (tokens.size() == expected_size) return true;
	std::cout << "Wrong number of parameters" << std::endl;
	return false;
}

bool Engine::check_min_size(std::vector<std::string> const& tokens, size_t expected_size) const
{
	if (tokens.size() >= expected_size) return true;
	std::cout << "Wrong number of parameters" << std::endl;
	return false;
}

void Engine::run() 
{
	std::string engine_command{ "" };
	do 
	{
		std::getline(std::cin, engine_command, '\n');
		communication_history.push_back(std::make_pair(message_direction::gui_to_engine, engine_command));	
	} while (execute_command(engine_command));
}

bool Engine::execute_command(std::string const& engine_command)
{
	auto tokens = Utility::tokenize(engine_command);
	
	if (tokens[0] == "quit") 
		return false;
	
	if (tokens[0] == "position" && check_min_size(tokens, 2)) 
	{ 
		if (last_position_cmd.empty() || engine_command.find(last_position_cmd) == std::string::npos)
		{
			game.reset(nullptr);
			board.reset(nullptr);
			auto it = tokens.begin();
			std::advance(it, 2);
			std::string fen_str{ "" };
			if (tokens[1] == "startpos") 
			{
				fen_str = Fen::startpos;
				while (it != tokens.end() && *it != "moves") it++;
			}
			else if (tokens[1] == "fen") 
			{
				while (it != tokens.end() && *it != "moves") 
				{
					fen_str = fen_str + *it + " ";
					it++;
				}
			}
			board = Factory::CreateBoard(fen_str);
			game = Factory::CreateGame(board.get());
			if (it != tokens.end() && *it == "moves")
			{
				it++;
				for(; it != tokens.end(); it++)
					game->make_move(*it);
			}
		}
		else 
		{
			game->make_move(tokens.back());
		}
		last_position_cmd = engine_command;
	}
	else if (tokens[0] == "go" && check_board()) 
	{ 
		execute_go_command(tokens);
	}
	else if (tokens[0] == "perft" && check_board() && check_min_size(tokens, 2))
	{
		execute_perft_command(tokens);
	}
	else if (tokens[0] == "ucinewgame") 
	{ 
		last_position_cmd == "";
	}
	else if (tokens[0] == "stop") 
	{ 
		/* do nothing for the time being */ 
	}
	else if (tokens[0] == "ponderhit") 
	{ 
		/* do nothing for the time being */ 
	}
	else if (tokens[0] == "register") 
	{ 
		/* do nothing for the time being */ 
	}
	else if (tokens[0] == "setoption") 
	{ 
		/* do nothing for the time being */ 
	}
	else if (tokens[0] == "show" && check_board() && check_size(tokens, 2))
	{
		execute_show_command(tokens);
	}
	else if (tokens[0] == "debug")
	{
		if (tokens[1] == "on")  debug_mode = true;
		if (tokens[1] == "off") debug_mode = false;
	}
	else if (tokens[0] == "isready")
	{
		std::string gcmd{ "readyok" };
		communication_history.push_back(std::make_pair(message_direction::engine_to_gui, gcmd));
		std::cout << gcmd << std::endl;
	}
	else if (tokens[0] == "uci")
	{
		std::stringstream id_command1{ "id name " }, id_command2{ "id author " };
		id_command1 << ENGINE_NAME << " " << VERSION << "." << SUBVERSION;
		id_command2 << AUTHOR_NAME;
		std::string gcmd{ "uciok" };
		communication_history.push_back(std::make_pair(message_direction::engine_to_gui, id_command1.str()));
		communication_history.push_back(std::make_pair(message_direction::engine_to_gui, id_command2.str()));
		communication_history.push_back(std::make_pair(message_direction::engine_to_gui, gcmd));
		std::cout << id_command1.str() << std::endl;
		std::cout << id_command2.str() << std::endl;
		std::cout << gcmd << std::endl;
	}
	else
	{
		std::cout << "unknown command: " << engine_command << std::endl;
	}
	return true;
}

void Engine::print_header() const
{
	std::cout << ENGINE_NAME << " " << VERSION << "." << SUBVERSION << ", developed by " << AUTHOR_NAME << " - myirci@gmail.com" << std::endl;
}

void Engine::execute_perft_command(std::vector<std::string> const& tokens)
{
	auto perft = Factory::CreatePerft(board.get());
	auto depth = std::stoi(tokens[1]);
	if (tokens.size() == 3)
	{
		if (tokens[2] == "divide")
		{
			perft->perft_divide(depth);
		}
		else if (tokens[3] == "stats")
		{
			perft->perft_with_statistics(depth);
		}
	}
	else
	{
		std::cout << perft->perft(depth) << std::endl;
	}
}

void Engine::execute_show_command(std::vector<std::string> const& tokens) 
{
	if (tokens[1] == "board")
	{
		Utility::print_board(*board);
	}
	else if (tokens[1] == "moves")
	{
		Utility::generate_and_print_moves(*board);
	}
	else if (tokens[1] == "plocs")
	{
		Utility::print_piece_locations(*board);
	}
}

void Engine::execute_go_command(std::vector<std::string> const& tokens) 
{
	auto param = std::make_unique<uci::SearchParameters>();
	auto it = std::next(tokens.begin());
	while (it != tokens.end())
	{
		if (*it == "wtime" && ++it != tokens.end())
		{
			param->wtime = std::stoull(*it);
		}
		else if (*it == "btime" && ++it != tokens.end())
		{
			param->btime = std::stoull(*it);
		}
		else if (*it == "winc" && ++it != tokens.end())
		{
			param->winc = static_cast<uint32_t>(std::stoul(*it));
		}
		else if (*it == "binc" && ++it != tokens.end())
		{
			param->binc = static_cast<uint32_t>(std::stoul(*it));
		}
		else if (*it == "movestogo" && ++it != tokens.end())
		{
			param->move_to_go = static_cast<int16_t>(std::stoi(*it));
		}
		else if (*it == "depth" && ++it != tokens.end())
		{
			param->depth = static_cast<int16_t>(std::stoi(*it));
		}
		else if (*it == "nodes" && ++it != tokens.end())
		{
			param->nodes = std::stoull(*it);
		}
		else if (*it == "movetime" && ++it != tokens.end())
		{
			param->move_time = std::stoull(*it);
		}
		else if (*it == "mate" && ++it != tokens.end())
		{
			param->mate = static_cast<int8_t>(std::stoi(*it));
		}
		else if (*it == "infinite")
		{
			param->infinite = true;
		}
		it++;
	}

	auto str = "bestmove " + game->make_move(*param);
	communication_history.push_back(std::make_pair(message_direction::engine_to_gui, str));
	std::cout << str << std::endl;	
}