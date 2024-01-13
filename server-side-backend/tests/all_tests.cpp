#pragma once
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
	//make entanglement returns if cycle occured
	REQUIRE(b->make_entanglement(Sign::X, 0, 1) == false);
	REQUIRE(b->make_entanglement(Sign::O, 0, 2) == false);
	REQUIRE(b->make_entanglement(Sign::X, 0, 3) == false);

	REQUIRE(b->make_entanglement(Sign::O, 5, 6) == false);
	REQUIRE(b->make_entanglement(Sign::X, 5, 7) == false);
	REQUIRE(b->make_entanglement(Sign::O, 5, 8) == false);
	
	
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
		b->make_entanglement(Sign::O, 10, 11);
		b->make_entanglement(Sign::O, 10, 12);
		b->make_entanglement(Sign::O, 12, 13);
		REQUIRE(b->get_roots().size() == 3);
	}

	SECTION("graph rebase root")
	{
		b->make_entanglement(Sign::O, 0, 5);
		REQUIRE(b->get_roots().size() == 2);
		REQUIRE(b->get_tile(5)->get_root().lock() == b->get_tile(0));
		REQUIRE(b->get_tile(6)->get_root().lock() == b->get_tile(0));
		REQUIRE(b->get_tile(7)->get_root().lock() == b->get_tile(0));
		REQUIRE(b->get_tile(8)->get_root().lock() == b->get_tile(0));
	}

	SECTION("make cycle")
	{
		bool ifCycle = b->make_entanglement(Sign::O, 2, 3);
		REQUIRE(ifCycle == true);
		b->tile_to_collapse(2);
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


TEST_CASE("making game", "[game]")
{

	SECTION("game creating")
	{
		
	}

}