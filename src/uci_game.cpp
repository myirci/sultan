#include "uci_game.hpp"
#include "board.hpp"
#include "utility.hpp"
#include "move_selector.hpp"

UciGame::UciGame(Board* brd) : board{ brd } { }
 
void UciGame::make_move(std::string_view mv_str) { make_move(Utility::to_move(mv_str, *board)); }

void UciGame::make_move(Move const& mv)
{
	move_stack.push(mv); 
	state_stack.push(board->make_move(mv));
}

void UciGame::unmake_move()
{
	board->unmake_move(move_stack.top(), state_stack.top());
	move_stack.pop();
	state_stack.pop();
}

std::string UciGame::make_move(uci::SearchParameters const& param)
{
	std::string mvstr{""};
	auto mv = MoveSelector::select_random_move(*board);
	if (mv) 
	{
		make_move(*mv);
		mvstr = "bestmove " + Utility::to_string(*mv);
		auto pmv = MoveSelector::select_random_move(*board);
		if (pmv) 
			mvstr += " ponder " + Utility::to_string(*pmv);
		
	}
	return mvstr;
}
