#pragma once

#include "../json.hpp"
#include "Sign.hpp"
#include <memory>
#include <string>
#include <vector>

class Entanglement;

using nlohmann::json;

class Tile
{
public:
	Tile(int idx);
	bool add_entanglement(std::shared_ptr<Entanglement>);
	bool measurement(std::shared_ptr<Entanglement> collapsing_entanglement);
	std::weak_ptr<Tile> get_root() const;
	std::vector<Sign> get_signs();
	Sign get_const_sign() const;
	int get_idx() const;
	std::vector<std::shared_ptr<Entanglement>> get_entaglements();
	int get_entaglements_size();

	json to_json();

	void set_root(std::shared_ptr<Tile> new_root);


	friend bool operator==(const Tile &t1, const Tile &t2);
	friend std::ostream& operator<<(std::ostream& stream, Tile& tile);

private:
	int index;
	Sign const_sign;
	std::weak_ptr<Tile> root;
	std::vector<std::shared_ptr<Entanglement>> entanglements;
};