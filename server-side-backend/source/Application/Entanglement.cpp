#include "Entanglement.hpp"
#include "Tile.hpp"

Entanglement::Entanglement(Sign sign, std::shared_ptr<Tile> tile1, std::shared_ptr<Tile> tile2)
	: sign(sign)
	, tile1(tile1)
	, tile2(tile2)
{ }

void Entanglement::update_entanglements(std::shared_ptr<Entanglement> entanglement) const
{
	tile1.lock()->add_entanglement(entanglement);
	tile2.lock()->add_entanglement(entanglement);
}

Sign Entanglement::get_sign()
{
	return sign;
}

std::shared_ptr<Tile> Entanglement::get_next_tile(std::shared_ptr<Tile> call_tile)
{
	if(tile1.lock() == call_tile)
	{
		return tile2.lock();
	}
	else
	{
		return tile1.lock();
	}
}