#include "Tile.hpp"
#include "Entanglement.hpp"

Tile::Tile(int idx)
	: const_sign(Sign::None)
	, index(idx)
{ }

bool Tile::add_entanglement(std::shared_ptr<Entanglement> entanglement)
{
	entanglements.push_back(entanglement);
	return true;
}

int Tile::get_entaglements_size()
{
	return entanglements.size();
}

std::vector<std::shared_ptr<Entanglement>> Tile::get_entaglements()
{
	return entanglements;
}

bool Tile::measurement(std::shared_ptr<Entanglement> collapsing_entanglement)
{
	for(auto e : entanglements)
	{
		if(e!=collapsing_entanglement)
		{
			std::shared_ptr<Tile> children = e->get_next_tile(*this);
			children->measurement(e);
		}
	}
	const_sign = collapsing_entanglement->get_sign();
	entanglements.clear();
	root.reset();
	return true;
}

std::vector<Sign> Tile::get_signs()
{
	if(const_sign != Sign::None)
	{
		return {const_sign};
	}
	std::vector<Sign> signs = {Sign::None};

	for(std::vector<std::shared_ptr<Entanglement>>::iterator i = entanglements.begin();
		i != entanglements.end();
		i++)
	{
		signs.push_back(i->get()->get_sign());
	}

	return signs;
}

Sign Tile::get_const_sign() const
{
	return const_sign;
}

void Tile::set_root(std::shared_ptr<Tile> new_root)
{
	root = new_root;
}

std::weak_ptr<Tile> Tile::get_root() const
{
	return root;
}

json Tile::to_json()
{
	json tile_json;
	std::vector<Sign> signs = get_signs();
	tile_json["ConstSign"] = signs.at(0);

	int counter = 0;
	for(std::vector<Sign>::iterator i = signs.begin() + 1; i != signs.end(); i++)
	{
		tile_json["Entanglement"][std::to_string(counter)] = *i;
		counter++;
	}

	return tile_json;
}


bool operator==(const Tile &t1, const Tile &t2)
{
	return (t1.index == t2.index);
}

std::ostream& operator<<(std::ostream& stream, Tile& tile)
{
	std::vector<Sign> signs = tile.get_signs();
	stream << "[ ";
	for(std::vector<Sign>::iterator i = signs.begin(); i != signs.end(); i++)
	{
		stream << *i << " ";
	}
	stream << "]";
	return stream;
}