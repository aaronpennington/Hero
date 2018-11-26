#include "pch.h"
#include "main.hpp"

Combat::Combat(float atk) : atk(atk) {}

void Combat::attack(std::shared_ptr<Ent> attacker, std::shared_ptr<Ent> target) {
	if (target->mortal) {
		if (!target->mortal->isDead()) {
			float dmg = atk - target->mortal->def;
			if (dmg > 0.0) {
				engine.gui->message(TCODColor::white, "The %s hits the %s for %g damage!\n", attacker->name, target->name, dmg);
				target->mortal->takeDamage(target, atk);
			}
			else {
				engine.gui->message(TCODColor::white, "The %s is too weak to harm the %s!\n", attacker->name, target->name);
			}
		}
	}
	else {
		engine.gui->message(TCODColor::white, "The %s is impervious to damage.\n", target->name);
	}
}
