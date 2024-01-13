#pragma once

#include "Board.hpp"
#include "GameStatus.hpp"

class Game
{
public:
	Game(int game_id, int n, int players_number);
	int get_id();
	int get_status();
	Sign get_turn();
	bool change_turn();
	std::shared_ptr<Board> get_board();

	bool start();
	bool make_move(Sign sign, int tile1_idx, int tile2_idx);

private:
	int id;
	GameStatus status;
	Sign game_winner;
	int players;
	Sign turn;
	std::shared_ptr<Board> board;
};