#include "Game.hpp"

Game::Game(int game_id, int n, int players_number)
	: id(game_id)
	, players(players_number)
	, status(Status::Off)
	, game_winner(Sign::None)
	, turn(Sign::None)
	, turn_counter(0)
{
	board = std::make_shared<Board>(n);
}

bool Game::start(Sign starting_sign)
{
	//players initialization (optional)
	status = Status::Ongoing;
	turn = starting_sign;
	turn_counter++;
	return true;
}

bool Game::change_turn()
{
	turn_counter++;
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
	if(status == Status::Cycle && tile2_idx == -1)
	{
		//move to make collapse
		if(board->tile_to_collapse(tile1_idx))
		{
			status = Status::Ongoing;
			Sign winner = board->check_for_winner();
			if(winner != Sign::None)
			{
				game_winner = winner;
				status = Status::Finished;
				return true;
			}
		}
		else
			return false;
	}
	else if(status == Status::Ongoing)
	{
		Status result = board->make_entanglement(sign, tile1_idx, tile2_idx);
		if(result == Status::False)
			return false;
		else
			status = result;
		
		change_turn();

	}
	else
	{
		return false;
	}

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

json Game::to_json()
{
	json game_json;
	game_json["gameId"] = id;
	game_json["status"] = game_status_to_string(status);
	game_json["currentPlayer"] = sign_to_string(turn);
	game_json["board"] = board->to_json();
	if(status == Status::Cycle)
	{
		game_json["cycle"] = { board->get_cycle_entanglement()->get_tile1()->get_idx(),
							   board->get_cycle_entanglement()->get_tile2()->get_idx() };
	}
	return game_json;
}
