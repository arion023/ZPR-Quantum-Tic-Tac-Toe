#pragma once

#include "Sign.hpp"
#include <memory>

class Tile;

class Entanglement
{
public:
	Entanglement(Sign sign, std::shared_ptr<Tile> tile1, std::shared_ptr<Tile> tile2);
	void update_entanglements(std::shared_ptr<Entanglement> entanglement) const;

	std::shared_ptr<Tile> get_next_tile(std::shared_ptr<Tile> call_tile);
	std::shared_ptr<Tile> get_next_tile(const Tile &call_tile);

	Sign get_sign();

    friend bool operator==(const Entanglement &e1, const Entanglement &e2);

private:
	const Sign sign;
	const std::weak_ptr<Tile> tile1;
	const std::weak_ptr<Tile> tile2;
};