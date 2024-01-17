#pragma once

#include "Board.hpp"

class Game
{
public:
	Game(int game_id, int n, int players_number);
	int get_id();
	int get_status();
	Sign get_turn();
	Sign get_winner();
	std::shared_ptr<Board> get_board();
	json to_json();

	bool start(Sign starting_sign = Sign::X);
	bool make_move(Sign sign, int tile1_idx, int tile2_idx);

private:
	bool change_turn();
	
	int id;
	int players;
	int turn_counter;
	Status status;
	Sign game_winner;
	Sign turn;
	std::shared_ptr<Board> board;
};