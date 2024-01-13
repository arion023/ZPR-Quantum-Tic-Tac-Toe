#include "Game.hpp"

Game::Game(int game_id, int n, int players_number)
	: id(game_id)
	, players(players_number)
	, status(GameStatus::off)
	, game_winner(Sign::None)
	, turn(Sign::X)
{
	board = std::make_shared<Board>(n);
}

bool Game::start()
{
	//players initialization (optional)
	status = GameStatus::ongoing;
	return true;
}

bool Game::change_turn()
{
	turn = (turn == Sign::X) ? Sign::O : Sign::X;
	return true;
}

bool Game::make_move(Sign sign, int tile1_idx, int tile2_idx)
{
	//check if correct player is making move
	if(sign != turn)
	{
		return false;
	}

	//-1 means operation of choosing tile to collapse
	if(tile2_idx == -1)
	{
		//move to make collapse
		//TODO handling if inccorect tile is passed
		board->tile_to_collapse(tile1_idx);
		status = GameStatus::ongoing;
		Sign winner = board->check_for_winner();
		if(winner != Sign::None)
		{
			status = GameStatus::finished;
			game_winner = winner;
		}
	}
	else
	{
		bool ifCycle = board->make_entanglement(sign, tile1_idx, tile2_idx);

		if(ifCycle)
		{
			status = GameStatus::cycle;
		}
	}
	change_turn();
	return true;
}

int Game::get_id()
{
	return id;
}

int Game::get_status()
{
	return status;
}

Sign Game::get_turn()
{
	return turn;
}

std::shared_ptr<Board> Game::get_board()
{
	return board;
}