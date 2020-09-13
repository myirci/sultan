#include "uci_engine.hpp"
#include "utility.hpp"
#include "factory.hpp"
#include "fen.hpp"

#include <sstream>
#include <algorithm>

UciEngine::UciEngine() : board{ nullptr }, game{ nullptr }, last_position_cmd{""}, debug_mode{ false } { print_header(); }

void UciEngine::print_header() const
{
	std::cout << ENGINE_NAME << " " << VERSION << "." << SUBVERSION << ", developed by " << AUTHOR_NAME << " - myirci@gmail.com" << std::endl;
}

void UciEngine::run() 
{
	std::string engine_command{ "" };
	do 
	{
		std::getline(std::cin, engine_command, '\n');
		store_gui_to_engine_command(engine_command);	
	} while (execute_command(engine_command));
}

bool UciEngine::execute_command(std::string const& engine_command)
{
	auto tokens = Utility::tokenize(engine_command);
	
	if (tokens[0] == "quit") 
		return false;
	
	if (tokens[0] == "position") 
	{ 
		if (check_min_tokens_size(tokens, 2)) 
		{
			if (last_position_cmd.empty() || engine_command.find(last_position_cmd) == std::string::npos)
			{
				reset(tokens);
			}
			else
			{
				game->make_move(tokens.back());
			}
			last_position_cmd = engine_command;
		}
	}
	else if (tokens[0] == "go")
	{ 
		if(check_board())
			execute_go_command(tokens);
	}
	else if (tokens[0] == "perft")
	{
		if (check_board() && check_min_tokens_size(tokens, 2))
			execute_perft_command(tokens);
	}
	else if (tokens[0] == "ucinewgame") 
	{ 
		last_position_cmd = "";
	}
	else if (tokens[0] == "stop") 
	{ 
		// stop searching and send the move and the ponder move  
	}
	else if (tokens[0] == "ponderhit") 
	{ 
		// ponder move is played by the opponent, no need to update the board, 
		// engine can finalize the ponder search and make its move
	}
	else if (tokens[0] == "register") 
	{ 
		/* do nothing for the time being */ 
	}
	else if (tokens[0] == "setoption") 
	{ 
		/* do nothing for the time being */
	}
	else if (tokens[0] == "show" && check_board() && check_tokens_size(tokens, 2))
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
		send_engine_to_gui_command("readyok");
	}
	else if (tokens[0] == "uci")
	{
		std::stringstream gcmdss{ "id name " };
		gcmdss << ENGINE_NAME << " " << VERSION << "." << SUBVERSION;
		send_engine_to_gui_command(gcmdss.str());
		gcmdss.clear();
		
		gcmdss << "id author " << AUTHOR_NAME;
		send_engine_to_gui_command(gcmdss.str());

		send_engine_to_gui_command("option name Threads type spin default 1 min 1 max 512");
		send_engine_to_gui_command("option name Hash type spin default 16 min 1 max 131072");
		send_engine_to_gui_command("option name Clear Hash type button");
		send_engine_to_gui_command("option name Skill Level type spin default 10 min 0 max 10");		
		send_engine_to_gui_command("uciok");		
	}
	else
	{
		send_engine_to_gui_command("unknown command: " + engine_command);
	}
	return true;
}

void UciEngine::execute_perft_command(std::vector<std::string> const& tokens)
{
	auto perft = Factory::CreatePerft(board.get());
	auto depth = std::stoi(tokens[1]);
	if (tokens.size() == 3)
	{
		if (tokens[2] == "divide")
		{
			perft->perft_divide(depth);
		}
		else if (tokens[2] == "stats")
		{
			perft->perft_with_statistics(depth);
		}
	}
	else
	{
		std::cout << perft->perft(depth) << std::endl;
	}
}

void UciEngine::execute_show_command(std::vector<std::string> const& tokens) 
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

void UciEngine::reset(std::vector<std::string> const& tokens)
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
		for (; it != tokens.end(); it++)
			game->make_move(*it);
	}
}

void UciEngine::execute_go_command(std::vector<std::string> const& tokens) 
{
	bool ponder_mode{ std::any_of(tokens.begin(), tokens.end(), [](auto& s) { return s == "ponder"; }) };
	
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

	send_engine_to_gui_command(game->make_move(*param));
}

bool UciEngine::check_board() const
{
	if (board) return true;
	std::cout << "Board is not initialized, use position command to set up the board" << std::endl;
	return false;
}

bool UciEngine::check_tokens_size(std::vector<std::string> const& tokens, size_t expected_size) const
{
	if (tokens.size() == expected_size) return true;
	std::cout << "Wrong number of parameters" << std::endl;
	return false;
}

bool UciEngine::check_min_tokens_size(std::vector<std::string> const& tokens, size_t expected_size) const
{
	if (tokens.size() >= expected_size) return true;
	std::cout << "Wrong number of parameters" << std::endl;
	return false;
}

void UciEngine::send_engine_to_gui_command(std::string const& cmd)
{
	communication_history.push_back(std::make_pair(message_direction::engine_to_gui, cmd));
	std::cout << cmd << std::endl;
}

void UciEngine::store_gui_to_engine_command(std::string const& cmd)
{
	communication_history.push_back(std::make_pair(message_direction::gui_to_engine, cmd));
}