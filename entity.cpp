#include "pch.h"
#include "main.hpp"

Ent::Ent(int x, int y, int ch, const char *name, const TCODColor &col) : x(x), y(y), name(name), ch(ch), col(col) {}

void Ent::render() const {
	TCODConsole::root->setChar(x, y, ch);
	TCODConsole::root->setCharForeground(x, y, col);
}

/*
bool Ent::moveOrAttack(int x, int y) {
	if (engine.dungeon->isWall(x, y)) {
		return false;
	}
	for (auto &ent : engine.entL) {
		if (ent->x == x && ent->y == y) {
			std::cout << "The " << ent->name << " avoids your attack!" << std::endl;
			return false;
		}
	}
	this ->x = x; this->y = y;
	return true;
}
*/

void Ent::update(std::shared_ptr<Ent> owner) {
	if (ai) {
		ai->update(owner);
	}
}