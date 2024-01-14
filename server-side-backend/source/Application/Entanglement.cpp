#include "Entanglement.hpp"
#include "Tile.hpp"

Entanglement::Entanglement(Sign sign, int number, std::shared_ptr<Tile> tile1, std::shared_ptr<Tile> tile2)
	: sign(sign)
	, number(number)
	, tile1(tile1)
	, tile2(tile2)
{ }

void Entanglement::update_entanglements(std::shared_ptr<Entanglement> entanglement) const
{
	tile1.lock()->add_entanglement(entanglement);
	tile2.lock()->add_entanglement(entanglement);
}

Sign Entanglement::get_sign() const
{
	return sign;
}

int Entanglement::get_number() const
{
	return number;
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

std::shared_ptr<Tile> Entanglement::get_next_tile(const Tile& call_tile)
{
	if(*tile1.lock() == call_tile)
	{
		return tile2.lock();
	}
	else
	{
		return tile1.lock();
	}
}

std::shared_ptr<Tile> Entanglement::get_tile1() const
{
	return tile1.lock();
}

std::shared_ptr<Tile> Entanglement::get_tile2() const
{
	return tile2.lock();
}

bool operator==(const Entanglement& e1, const Entanglement& e2)
{
	bool ifTile1 = (e1.tile1.lock() == e2.tile1.lock());
	bool ifTile2 = (e1.tile2.lock() == e2.tile2.lock());
	bool ifSign = (e1.sign == e2.sign);
	return (ifTile1 && ifTile2 && ifSign);
}