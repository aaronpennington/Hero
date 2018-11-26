#include "pch.h"
#include "main.hpp"

static const int rsMIN = 6, rsMAX = 12;
static const int rMonstersMAX = 3;

class bspLst : public ITCODBspCallback {
	private:
		Map &dungeon;
		int rNum; 
		int lastx, lasty;
	public:
		bspLst(Map &dungeon) : dungeon(dungeon), rNum(0) {}
		bool visitNode(TCODBsp *node, void *userData) {
			if (node->isLeaf()) {
				int x,y,w,h;
				TCODRandom *rng = TCODRandom::getInstance();
				w = rng->getInt(rsMIN, node->w-2);
				h = rng->getInt(rsMIN, node->h-2);
				x = rng->getInt(node->x+1, node->x+node->w-w-1);
				y = rng->getInt(node->y+1, node->y+node->h-h-1);
				dungeon.cRoom(rNum == 0, x,y,x+w-1,y+h-1);
				if (rNum != 0) {
					dungeon.dig(lastx, lasty, x+w/2, lasty);
					dungeon.dig(x+w/2, lasty, x+w/2, y+h/2);
				}
				lastx = x+w/2; lasty = y+h/2; rNum++;
			}
			return true;
		}
};

Map::Map(int w, int h) : w(w), h(h) {
	tiles = new Tile[w*h];
	map = std::make_shared<TCODMap>(w,h);
	TCODBsp bsp(0,0,w,h);
	bsp.splitRecursive(NULL, 8, rsMAX, rsMAX, 1.5f, 1.5f);
	bspLst listener(*this);
	bsp.traverseInvertedLevelOrder(&listener, NULL);
}

Map::~Map() { delete[] tiles; }

void Map::dig(int x1, int y1, int x2, int y2) {
	if (x2 < x1) {
		int tmp = x2;
		x2 = x1;
		x1 = tmp;
	}
	if (y2 < y1) {
		int tmp = y2;
		y2 = y1;
		y1 = tmp;
	}
	for (int tx = x1; tx <= x2; tx++) {
		for (int ty = y1; ty <= y2; ty++) {
			map->setProperties(tx, ty, true, true);
		}
	}
}



void Map::cRoom(bool first, int x1, int y1, int x2, int y2) {
	dig(x1, y1, x2, y2);
	int cx((x1 + x2) / 2), cy((y1 + y2) / 2);
	if (first) {
		engine.player->x = cx; 
		engine.player->y = cy;
	}
	else {
		TCODRandom *rng = TCODRandom::getInstance();
		int nbMonsters(rng->getInt(0, rMonstersMAX));
		while (nbMonsters > 0) {
			int x(rng->getInt(x1, x2)), y(rng->getInt(y1, y2));
			if (canWalk(x, y)) {
				addMonster(x, y);
			}
			nbMonsters--;
		}
	}
}

bool Map::isWall(int x, int y) const { 
	return !map->isWalkable(x,y); 
}

bool Map::isExplored(int x, int y) const {
	return tiles[x+y*w].explored;
}

bool Map::isInFov(int x, int y) const {
	if (x < 0 || x >= w || y < 0 || y >= h) {
		return false;
	}
	if (map->isInFov(x, y)) {
		tiles[x+y*w].explored = true;
		return true;
	}
}

bool Map::canWalk(int x, int y) const {
	if (isWall(x, y)) {
		return false;
	}
	for (auto &ent : engine.entL) {
		if (ent->x == x && ent->y == y) {
			return false;
		}
	}
	return true;
}

void Map::computeFov() {
	map->computeFov(engine.player->x, engine.player->y, engine.fovRad);
}

void Map::addMonster(int x, int y) {
	std::random_device seed;
	std::default_random_engine dRoll(seed());
	std::uniform_int_distribution<int> d100(1, 100);
	auto mDice = std::bind(d100, dRoll);
	if (mDice() < 80) { 
		std::shared_ptr<Ent> toady = std::make_shared<Ent>(x, y, 't', "Toady", TCODColor::desaturatedGreen);
		toady->mortal = std::make_shared<Mortal>(10, 0, "dead toady");
		toady->combat = std::make_shared<Combat>(3);
		toady->ai = std::make_shared<MobAi>();
		engine.entL.push_back(toady);
	}
	else { 
		std::shared_ptr<Ent> henchman = std::make_shared<Ent>(x, y, 'h', "Henchman", TCODColor::darkOrange);
		henchman->mortal = std::make_shared<Mortal>(16, 1, "dead henchman");
		henchman->combat = std::make_shared<Combat>(4);
		henchman->ai = std::make_shared<MobAi>();
		engine.entL.push_back(henchman);
	}
}

void Map::render() const {
	static const TCODColor dWall(0, 0, 30), dFloor(30, 30, 50),
		lWall(80, 80, 110), lFloor(100, 100, 100);

	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {
			if (isInFov(x, y)) {
				TCODConsole::root->setCharBackground(x, y, isWall(x,y) ? lWall : lFloor);
			}
			else if (isExplored(x, y)) {
				TCODConsole::root->setCharBackground(x, y, isWall(x, y) ? dWall : dFloor);
			}
		}
	}
}