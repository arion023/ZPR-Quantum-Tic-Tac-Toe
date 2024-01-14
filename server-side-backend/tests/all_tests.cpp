#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include "Game.hpp"

std::unique_ptr<Board> set_up_board()
{
	auto board = std::make_unique<Board>(5);
	board->make_entanglement(Sign::X, 0, 1);
	board->make_entanglement(Sign::O, 0, 2);
	board->make_entanglement(Sign::X, 0, 3);

	board->make_entanglement(Sign::O, 5, 6);
	board->make_entanglement(Sign::X, 5, 7);
	board->make_entanglement(Sign::O, 5, 8);

	return board;
}

TEST_CASE("making entanglement", "[board]")
{
	auto b = std::make_unique<Board>(5);

	REQUIRE(b->make_entanglement(Sign::X, 0, 0) == Status::False);
	REQUIRE(b->make_entanglement(Sign::O, 25, 2) == Status::False);
	REQUIRE(b->make_entanglement(Sign::O, 2, -2) == Status::False);

	//make entanglement returns if cycle occured
	REQUIRE(b->make_entanglement(Sign::X, 0, 1) == Status::True);
	REQUIRE(b->make_entanglement(Sign::O, 0, 2) == Status::True);
	REQUIRE(b->make_entanglement(Sign::X, 0, 3) == Status::True);

	REQUIRE(b->make_entanglement(Sign::O, 5, 6) == Status::True);
	REQUIRE(b->make_entanglement(Sign::X, 5, 7) == Status::True);
	REQUIRE(b->make_entanglement(Sign::O, 5, 8) == Status::True);

	auto tile0 = b->get_tile(0);
	auto entanglements0 = tile0->get_entaglements();
	REQUIRE(b->get_tile(0)->get_entaglements_size() == 3);
	REQUIRE(b->get_tile(1)->get_entaglements_size() == 1);
	REQUIRE(b->get_tile(4)->get_entaglements_size() == 0);

	REQUIRE(entanglements0.at(0)->get_next_tile(tile0) == b->get_tile(1));
	REQUIRE(entanglements0.at(1)->get_next_tile(tile0) == b->get_tile(2));
	REQUIRE(entanglements0.at(2)->get_next_tile(tile0) == b->get_tile(3));
}

TEST_CASE("structure of graph", "[board]")
{
	auto b = set_up_board();

	SECTION("graph roots creating")
	{
		REQUIRE(b->get_roots().size() == 2);
		REQUIRE(b->make_entanglement(Sign::O, 10, 11) == Status::True);
		REQUIRE(b->make_entanglement(Sign::O, 10, 12) == Status::True);
		REQUIRE(b->make_entanglement(Sign::O, 12, 13) == Status::True);
		REQUIRE(b->get_roots().size() == 3);
	}

	SECTION("graph rebase root")
	{
		REQUIRE(b->make_entanglement(Sign::O, 0, 5) == Status::True);
		REQUIRE(b->get_roots().size() == 1);
		REQUIRE(b->get_tile(5)->get_root().lock() == b->get_tile(0));
		REQUIRE(b->get_tile(6)->get_root().lock() == b->get_tile(0));
		REQUIRE(b->get_tile(7)->get_root().lock() == b->get_tile(0));
		REQUIRE(b->get_tile(8)->get_root().lock() == b->get_tile(0));
	}

	SECTION("make cycle")
	{
		REQUIRE(b->make_entanglement(Sign::O, 2, 3) == Status::Cycle);
		REQUIRE(b->tile_to_collapse(0) == false);
		REQUIRE(b->tile_to_collapse(2) == true);
		REQUIRE(b->get_tile(2)->get_root().expired() == true);
		REQUIRE(b->get_tile(1)->get_root().expired() == true);
		REQUIRE(b->get_tile(2)->get_root().expired() == true);
		REQUIRE(b->get_tile(3)->get_root().expired() == true);

		REQUIRE(b->get_tile(0)->get_entaglements_size() == 0);
		REQUIRE(b->get_tile(1)->get_entaglements_size() == 0);
		REQUIRE(b->get_tile(2)->get_entaglements_size() == 0);
		REQUIRE(b->get_tile(3)->get_entaglements_size() == 0);

		REQUIRE(b->get_tile(2)->get_const_sign() == Sign::O);
		REQUIRE(b->get_tile(0)->get_const_sign() == Sign::O);
		REQUIRE(b->get_tile(1)->get_const_sign() == Sign::X);
		REQUIRE(b->get_tile(3)->get_const_sign() == Sign::X);
	}
}

Game setupGame()
{
	Game game = Game(1, 5, 2);
	game.start();
	return game;
}


TEST_CASE("making game", "[game]")
{

	SECTION("game creating")
	{
		Game game = Game(1, 5, 2);
		REQUIRE(game.get_status() == Status::Off);
		//make move before start
		REQUIRE(game.make_move(Sign::X, 0, 1) == false);
		game.start();
		REQUIRE(game.get_status() == Status::Ongoing);
		REQUIRE(game.get_turn() == Sign::X);
		REQUIRE(game.get_winner() == Sign::None);
		REQUIRE(game.get_board()->get_size() == 5);
	}

	SECTION("making moves")
	{
		Game game = Game(1, 5, 2);
		game.start();
		//invalid sign
		REQUIRE(game.make_move(Sign::O, 0, 1) == false);
		//self entanglement
		REQUIRE(game.make_move(Sign::X, 0, 0) == false);
		//invalid idx
		REQUIRE(game.make_move(Sign::X, -1, 0) == false);
		//invalid idx
		REQUIRE(game.make_move(Sign::X, 0, 25) == false);
		//trying to collapse cycle without cycle
		REQUIRE(game.make_move(Sign::X, 0, -1) == false);
		//correct move
		REQUIRE(game.make_move(Sign::X, 0, 1) == true);
		REQUIRE(game.get_turn() == Sign::O);
		//invalid sign
		REQUIRE(game.make_move(Sign::X, 1, 0) == false);
		//correct move
		REQUIRE(game.get_board()->get_roots().size() == 1);
		REQUIRE(game.make_move(Sign::O, 1, 2) == true);
		REQUIRE(game.make_move(Sign::X, 2, 3) == true);
		REQUIRE(game.make_move(Sign::O, 5, 6) == true);
		REQUIRE(game.make_move(Sign::X, 6, 7) == true);
		REQUIRE(game.get_board()->get_roots().size() == 2);
		REQUIRE(game.make_move(Sign::O, 7, 0) == true);
		REQUIRE(game.get_board()->get_roots().size() == 1);
	}

	SECTION("creating cycle")
	{
		Game game = setupGame();
		REQUIRE(game.make_move(Sign::X, 0, 1) == true);
		REQUIRE(game.make_move(Sign::O, 0, 2) == true);
		REQUIRE(game.make_move(Sign::X, 4, 5) == true);
		REQUIRE(game.make_move(Sign::O, 0, 4) == true);
		REQUIRE(game.make_move(Sign::X, 5, 2) == true);
		REQUIRE(game.get_turn() == Sign::O);
		REQUIRE(game.get_status() == Status::Cycle);
		//cannot make normal move thus waiting for selectiong collapsing tile 
		REQUIRE(game.make_move(Sign::O, 10, 11) == false);
		//invalid move
		REQUIRE(game.make_move(Sign::O, -1, 10) == false);
		//invalid collapsing tile
		REQUIRE(game.make_move(Sign::O, 1, -1) == false);
		//correct collapsing tile
		REQUIRE(game.make_move(Sign::O, 2, -1) == true);
		REQUIRE(game.get_status() == Status::Ongoing);
	}

	SECTION("game finishing")
	{
		//TODO
	}
}