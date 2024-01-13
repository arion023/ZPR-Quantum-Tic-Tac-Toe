#include "Board.hpp"
#include <cmath>
#include <iostream>

Board::Board(int n)
	: n(n)
	, cycle_occured(false)
{
	tiles_table = std::make_unique<std::shared_ptr<Tile>[]>(pow(n, 2));
	for(int i = 0; i < pow(n, 2); i++)
	{
		tiles_table[i] = std::make_shared<Tile>(i);
	}
}

int Board::get_size() const
{
	return n;
}

std::vector<std::shared_ptr<Tile>> Board::get_roots() const
{
	return complete_graphs_roots;
}

std::shared_ptr<Tile> Board::get_tile(int i) const
{
	return tiles_table[i];
}

void Board::rebase_cycle(std::shared_ptr<Tile> root, std::shared_ptr<Tile> new_root)
{
	remove_graph_root(root);
	root->set_root(new_root);
	change_childrens_root(root, root, new_root);
	//todo change value of w ptr in graph
}

void Board::change_childrens_root(std::shared_ptr<Tile> excluded_tile,
								  std::shared_ptr<Tile> parent,
								  std::shared_ptr<Tile> root)
{
	for(auto entaglement : parent->get_entaglements())
	{
		auto children = entaglement->get_next_tile(parent);
		if(children != excluded_tile)
		{
			children->set_root(root);
			change_childrens_root(parent, children, root);
		}
	}
}

void Board::remove_graph_root(std::shared_ptr<Tile> root)
{
	//delete root from set of complete cycles
	for(auto roots_iter = complete_graphs_roots.begin(); roots_iter < complete_graphs_roots.end();
		roots_iter++)
	{
		if(*roots_iter == root)
			complete_graphs_roots.erase(roots_iter);
	}
}

bool Board::make_entanglement(Sign sign, int tile1_idx, int tile2_idx)
{
	//check if this entaglement is connected with any graph
	std::weak_ptr<Tile> root_t1 = get_tile(tile1_idx)->get_root();
	std::weak_ptr<Tile> root_t2 = get_tile(tile2_idx)->get_root();

	if(!root_t1.expired() && !root_t2.expired())
	{
		std::shared_ptr<Tile> shr_root_t1 = root_t1.lock();
		std::shared_ptr<Tile> shr_root_t2 = root_t2.lock();
		if(shr_root_t1 == shr_root_t2)
		{
			//that means cycle occured
			cycle_occured = true;
			return false;
		}
		else
		{
			rebase_cycle(root_t2.lock(), root_t1.lock());
		}
	}
	else if(!root_t1.expired())
	{
		tiles_table[tile2_idx]->set_root(get_tile(tile1_idx));
	}
	else if(!root_t2.expired())
	{
		get_tile(tile1_idx)->set_root(get_tile(tile2_idx));
	}
	else
	{
		get_tile(tile1_idx)->set_root(get_tile(tile1_idx));
		get_tile(tile2_idx)->set_root(get_tile(tile1_idx));

		complete_graphs_roots.push_back(root_t1.lock());
	}

	//TODO checking if correct move

	std::shared_ptr<Entanglement> entanglement =
		std::make_shared<Entanglement>(sign, tiles_table[tile1_idx], tiles_table[tile2_idx]);

	entanglement->update_entanglements(entanglement);

	return true;
}

void Board::collapse_cycle(std::shared_ptr<Tile> excluded_tile, std::shared_ptr<Tile> parent) const
{
	auto entanglements = parent->get_entaglements();
	for(auto entanglement : entanglements)
	{
		std::shared_ptr<Tile> children = entanglement->get_next_tile(parent);
		if(children != excluded_tile)
		{
			collapse_cycle(parent, children);
		}
	}
	parent->measurement();
}

bool Board::check_for_winner() const
{
	//TODO
	return false;
}

json Board::to_json()
{
	json board_json;
	for(int i = 0; i < pow(n, 2); i++)
	{
		board_json["board"][std::to_string(i)] = tiles_table[i]->to_json();
	}
	return board_json;
}

std::ostream& operator<<(std::ostream& stream, const Board& board)
{
	for(int i = 0; i < board.n; i++)
	{
		for(int j = 0; j < board.n; j++)
		{
			stream << *board.tiles_table[i * board.n + j];
		}
		stream << std::endl;
	}
	return stream;
}