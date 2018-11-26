#include "pch.h"
#include "main.hpp"

Engine::Engine(int sW, int sH) : fovRad(10), gameState(START), sW(sW), sH(sH) {
	TCODConsole::initRoot(sW, sH, "HEROES", false);

	player = std::make_shared<Ent>(1, 1, '@', "player", TCODColor::white);
	player->mortal = std::make_shared<pcMortal>(100, 2, "your lifeless corpse");
	player->combat = std::make_shared<Combat>(5);
	player->ai = std::make_shared<PlayerAi>();
	entL.push_back(player);

	dungeon = std::make_unique<Map>(100,70);
	gui = new Gui();
	gui->message(TCODColor::red, "WELCOME TO HEROES!");
}

Engine::~Engine() { entL.clear(); delete gui; }

void Engine::update() {
	if (gameState == START) {
		dungeon->computeFov();
	}
	gameState = IDLE;
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE, &lastKey, &mouse);
	player->update(player);
	if (gameState == TURN) {
		for (auto &ent : entL) {
			if (ent != entL[0]) {
				ent->update(ent);
			}
		}
	}
}

void Engine::render() {
	TCODConsole::root->clear(); 
	//render dungeon first
	dungeon->render();
	//render dead entities second
	for (auto &ent : entL) {
		if (ent->mortal) {
			if (ent->mortal->isDead()) {
				if (dungeon->isInFov(ent->x, ent->y)) {
					ent->render();
				}
			}
		}
	}
	//render alive entities third
	for (auto &ent : entL) {
		if (ent->mortal) {
			if (!ent->mortal->isDead()) {
				if (dungeon->isInFov(ent->x, ent->y)) {
					ent->render();
				}
			}
		}
	}
	//render player last
	player->render();

	//just kidding, render gui last
	gui->render();

	//TCODConsole::root->print(1, sH - 2, "HP: %d:%d", (int)player->mortal->hp, (int)player->mortal->maxHp);
}