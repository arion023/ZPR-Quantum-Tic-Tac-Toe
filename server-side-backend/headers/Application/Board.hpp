#pragma once

#include "Entanglement.hpp"
#include "Tile.hpp"

#include <sstream>

class Board
{
public:
	Board(int n);

	bool make_entanglement(Sign sign, int title1_idx, int title2_idx);
	void collapse_cycle(std::shared_ptr<Tile> excluded_tile, std::shared_ptr<Tile> parent) const;
	bool check_for_winner() const;

	int get_size() const;
	std::vector<std::shared_ptr<Tile>> get_roots() const;

	json to_json();

	std::shared_ptr<Tile> get_tile(int i) const;
	friend std::ostream& operator<<(std::ostream& out, const Board& board);

private:
	int n;
	bool cycle_occured;
	std::weak_ptr<Entanglement> cycle_entanglement;
	std::unique_ptr<std::shared_ptr<Tile>[]> tiles_table;
	std::vector<std::shared_ptr<Tile>> complete_graphs_roots;

	void rebase_cycle(std::shared_ptr<Tile> root, std::shared_ptr<Tile> new_root);
	void remove_graph_root(std::shared_ptr<Tile> root);
	void change_childrens_root(std::shared_ptr<Tile> excluded_tile,
							   std::shared_ptr<Tile> parent,
							   std::shared_ptr<Tile> root);
};