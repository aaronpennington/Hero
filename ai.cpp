#include "pch.h"
#include "main.hpp"

void PlayerAi::update(std::shared_ptr<Ent> owner) {
	if (owner->mortal) {
		if (!owner->mortal->isDead()) {
			int ix = 0;
			int iy = 0;

			switch (engine.lastKey.vk) {
				case TCODK_UP: iy = -1; break;
				case TCODK_DOWN: iy = +1; break;
				case TCODK_LEFT: ix = -1; break;
				case TCODK_RIGHT: ix = +1; break;
				default:break;
			}

			if (ix != 0 || iy != 0) {
				engine.gameState = Engine::TURN;
				if (moveOrAttack(owner, owner->x + ix, owner->y + iy)) {
					engine.dungeon->computeFov();
				}
			}
		}
	}
}

bool PlayerAi::moveOrAttack(std::shared_ptr<Ent> owner, int tx, int ty) {
	if (engine.dungeon->isWall(tx, ty)) {
		return false;
	}
	for (auto &ent : engine.entL) {
		if (ent->mortal) {
			if (!ent->mortal->isDead() && ent->x == tx && ent->y == ty) {
				owner->combat->attack(owner, ent);
				return false;
			}
		}
		else if (ent->mortal->isDead() && ent->x == tx && ent->y == ty) {
			engine.gui->message(TCODColor::white, "A %s lays here.\n", ent->name);
		}
	}
	owner->x = tx; 
	owner->y = ty;
	return true;
}

static const int TRACK_TURNS = 3;
void MobAi::update(std::shared_ptr<Ent> owner) {
	if (owner->mortal) {
		if (owner->mortal->isDead()) {
			return;
		}
		if (engine.dungeon->isInFov(owner->x, owner->y)) {
			moveCount = TRACK_TURNS;
		}
		else {
			moveCount--;
		}

		if (moveCount > 0) {
			moveOrAttack(owner, engine.player->x, engine.player->y);
		}
	}
}

void MobAi::moveOrAttack(std::shared_ptr<Ent> owner, int tx, int ty) {
	int dx(tx - owner->x), dy(ty - owner->y), sdx(dx > 0 ? 1:-1), sdy(dy > 0 ? 1:-1), distance((int)sqrt(dx*dx + dy*dy));
	if (distance >= 2) {
		dx = (int)(round(dx / distance));
		dy = (int)(round(dy / distance));
		if (engine.dungeon->canWalk(owner->x + dx, owner->y + dy)) {
			owner->x += dx; 
			owner->y += dy;
		}
		else if (engine.dungeon->canWalk(owner->x + sdx, owner->y)) {
			owner->x += sdx;
		}
		else if (engine.dungeon->canWalk(owner->x, owner->y + sdy)) {
			owner->y += sdy;
		}
	}
	else if (owner->combat) {
		owner->combat->attack(owner, engine.player);
	}
}