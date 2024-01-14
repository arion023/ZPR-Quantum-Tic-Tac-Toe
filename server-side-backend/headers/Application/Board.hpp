#pragma once

#include "Entanglement.hpp"
#include "Status.hpp"
#include "Tile.hpp"

#include <sstream>

class Board
{
public:
	Board(int n);

	Status make_entanglement(Sign sign, int tile1_idx, int tile2_idx);
	bool tile_to_collapse(int tile_idx);
	Sign check_for_winner() const;

	int get_size() const;
	std::vector<std::shared_ptr<Tile>> get_roots() const;

	json to_json();

	std::shared_ptr<Tile> get_tile(int i) const;
	friend std::ostream& operator<<(std::ostream& out, const Board& board);

private:
	int n;
	bool cycle_occured;
	std::shared_ptr<Entanglement> cycle_entanglement;
	std::unique_ptr<std::shared_ptr<Tile>[]> tiles_table;

	//TODO roots can be removed
	std::vector<std::shared_ptr<Tile>> complete_graphs_roots;

	void collapse_cycle(std::shared_ptr<Entanglement>, std::shared_ptr<Tile> parent) const;
	bool rebase_cycle(std::shared_ptr<Tile> root, std::shared_ptr<Tile> new_root);
	//TODO roots can be removed
	bool remove_graph_root(std::shared_ptr<Tile> root);
	void change_childrens_root(std::shared_ptr<Tile> excluded_tile,
							   std::shared_ptr<Tile> parent,
							   std::shared_ptr<Tile> root);
};