#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include "Board.hpp"

std::unique_ptr<Board> set_up_board()
{
	auto board = std::make_unique<Board>(5);
	board->make_entanglement(Sign::O, 0, 1);
	board->make_entanglement(Sign::X, 0, 2);
	board->make_entanglement(Sign::O, 0, 3);

	board->make_entanglement(Sign::O, 5, 6);
	board->make_entanglement(Sign::O, 5, 7);
	board->make_entanglement(Sign::O, 5, 8);

	return board;
}

TEST_CASE("board making entaglement", "[board]")
{
	auto b = set_up_board();
	auto tile0 = b->get_tile(0);
	auto entanglements0 = tile0->get_entaglements();
	REQUIRE(b->get_tile(0)->get_entaglements_size() == 3);
	REQUIRE(b->get_tile(1)->get_entaglements_size() == 1);
	REQUIRE(b->get_tile(4)->get_entaglements_size() == 0);

	REQUIRE(entanglements0.at(0)->get_next_tile(tile0) == b->get_tile(1));
	REQUIRE(entanglements0.at(1)->get_next_tile(tile0) == b->get_tile(2));
	REQUIRE(entanglements0.at(2)->get_next_tile(tile0) == b->get_tile(3));

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
}