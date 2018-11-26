#include "pch.h"
#include "main.hpp"

Mortal::Mortal(float maxHp, float def, const char *corpseName) : maxHp(maxHp), hp(maxHp), def(def), corpseName(corpseName) {}

float Mortal::takeDamage(std::shared_ptr<Ent>owner, float dmg) {
	dmg -= def;
	if (dmg > 0) {
		hp -= dmg;
		if (hp <= 0) {
			die(owner);
		}
	}
	else {
		dmg = 0;
	}
	return dmg;
}

float Mortal::heal(float amount) {
	hp += amount;
	if (hp > maxHp) {
		amount -= hp-maxHp;
		hp = maxHp;
	}
	return amount;
}

void Mortal::die(std::shared_ptr<Ent> owner) {
	owner->ch = '%';
	owner->col = TCODColor::darkRed;
	owner->name = corpseName; 
	owner->blocks = false;
}

pcMortal::pcMortal(float maxHp, float def, const char *corpseName) : Mortal(maxHp, def, corpseName) {}

npcMortal::npcMortal(float maxHp, float def, const char *corpseName) : Mortal(maxHp, def, corpseName) {}

void pcMortal::die(std::shared_ptr<Ent> owner) {
	engine.gui->message(TCODColor::white, "You have been slain...\n");
	Mortal::die(owner);
	engine.gameState = Engine::LOSE;
}

void npcMortal::die(std::shared_ptr<Ent> owner) {
	engine.gui->message(TCODColor::white, "The %s has perished.\n", owner->name);
	Mortal::die(owner);
}