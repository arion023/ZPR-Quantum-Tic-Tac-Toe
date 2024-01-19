#include "Board.hpp"
#include <cmath>
#include <iostream>

Board::Board(int n)
	: n(n)
	, cycle_occured(false)
	, entanglement_counter(0)
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
	if(i >= n * n || i < 0)
	{
		return nullptr;
	}

	return tiles_table[i];
}

bool Board::rebase_cycle(std::shared_ptr<Tile> root, std::shared_ptr<Tile> new_root)
{

	remove_graph_root(root);
	root->set_root(new_root);
	change_childrens_root(root, root, new_root);
	return true;
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

bool Board::remove_graph_root(std::shared_ptr<Tile> root)
{
	int i = 0;
	for(auto tile : complete_graphs_roots)
	{
		if(tile == root)
		{
			complete_graphs_roots.erase(complete_graphs_roots.begin() + i);
			return true;
		}
		i++;
	}
	return false;
}

std::shared_ptr<Entanglement> Board::get_cycle_entanglement() const
{
	return cycle_entanglement;
}

//returns if cycle occured
Status Board::make_entanglement(Sign sign, int tile1_idx, int tile2_idx)
{
	if(tile1_idx == tile2_idx)
	{
		return Status::False;
	}

	if(!get_tile(tile1_idx) || !get_tile(tile2_idx))
	{
		return Status::False;
	}

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
			entanglement_counter++;
			cycle_entanglement = std::make_shared<Entanglement>(
				sign, entanglement_counter, tiles_table[tile1_idx], tiles_table[tile2_idx]);

			cycle_occured = true;
			return Status::Cycle;
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
		std::shared_ptr<Tile> t1 = get_tile(tile1_idx);
		std::shared_ptr<Tile> t2 = get_tile(tile2_idx);
		t1->set_root(t1);
		t2->set_root(t1);
		complete_graphs_roots.push_back(t1);
	}

	entanglement_counter++;
	std::shared_ptr<Entanglement> entanglement =
		std::make_shared<Entanglement>(sign, entanglement_counter, tiles_table[tile1_idx], tiles_table[tile2_idx]);

	entanglement->update_entanglements(entanglement);

	return Status::True;
}

bool Board::tile_to_collapse(int tile_idx)
{
	auto tile = get_tile(tile_idx);

	if(!tile)
		return false;

	if((tile != cycle_entanglement->get_tile1() && tile != cycle_entanglement->get_tile2()) || !cycle_occured)
		return false;

	tile->measurement(cycle_entanglement);
	cycle_entanglement.reset();
	cycle_occured = false;
	return true;
}

Sign Board::check_for_winner() const
{
	//TODO
	return Sign::None;
}

json Board::to_json()
{
	json board_json;
	for(int i = 0; i < pow(n, 2); i++)
	{
		board_json[std::to_string(i)] = get_tile(i)->to_json();
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