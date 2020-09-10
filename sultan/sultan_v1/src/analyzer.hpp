#pragma once

class Board; 

class Analyzer 
{
	Board* board;

	Analyzer(Board* brd);

	friend class Factory;
};