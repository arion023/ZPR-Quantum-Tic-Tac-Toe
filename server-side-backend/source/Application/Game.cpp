#include "Game.hpp"

Game::Game(int game_id, int n, int players_number)
	: id(game_id)
	, players(players_number)
	, status(Status::Off)
	, game_winner(Sign::None)
	, turn(Sign::None)
{
	board = std::make_shared<Board>(n);
}

bool Game::start(Sign starting_sign)
{
	//players initialization (optional)
	status = Status::Ongoing;
	turn = starting_sign;
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
	if((status!=Status::Ongoing && status == Status::Cycle) || sign != turn)
	{
		return false;
	}

	//-1 means operation of choosing tile to collapse
	if(tile2_idx == -1)
	{
		if(status == Status::Cycle)
		{

			//move to make collapse
			//TODO handling if inccorect tile is passed
			if(board->tile_to_collapse(tile1_idx))
			{
				status = Status::Ongoing;
				Sign winner = board->check_for_winner();
				if(winner != Sign::None)
				{
					game_winner = winner;
					status = Status::Finished;
				}
			}
			else
				return false;
		}
		else
		{
			return false;
		}
	}
	else
	{
		Status result = board->make_entanglement(sign, tile1_idx, tile2_idx);
		if(result == Status::False)
			return false;
		else
			status = result;
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

Sign Game::get_winner()
{
	return game_winner;
}

std::shared_ptr<Board> Game::get_board()
{
	return board;
}