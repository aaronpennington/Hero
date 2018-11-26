#include "pch.h"
#include "main.hpp"

//Panel Height, Bar Width, Message X Position, Message Log Height
static const int pHeight(7), bWidth(20), msgX(bWidth+2), msgHeight(pHeight-1);

Gui::Gui() {
	con = new TCODConsole(engine.sW, pHeight);
}

Gui::~Gui() {
	delete con;
	log.clear();
}

void Gui::render() {
	con->setDefaultBackground(TCODColor::yellow);
	con->clear();



	renderBar(1, 1, bWidth, "HP", engine.player->mortal->hp, engine.player->mortal->maxHp,
			  TCODColor::lightRed, TCODColor::darkerRed);

	con->setDefaultBackground(TCODColor::white);
	con->hline(0, 0, engine.sW, TCOD_BKGND_SET);

	int y(1); 
	float cCoef(0.4f); //"color coefficient" makes text look darker.

	for (auto &msg : log) { //<-- this is a "ranged for" loop, by the way.
		con->setDefaultForeground(msg->col * cCoef); 
		con->print(msgX, y, msg->text);
		y++;

		if (cCoef < 1.0f) {
			cCoef += 0.3f;
		}
	}

	renderMouseLook();

	TCODConsole::blit(con, 0, 0, engine.sW, pHeight, TCODConsole::root, 0, engine.sH - pHeight);
}

void Gui::renderBar(int x, int y, int width, const char *name, float value,
	float maxValue, const TCODColor &barColor,
	const TCODColor &backColor) {
	con->setDefaultBackground(backColor);
	con->rect(x, y, width, 1, false, TCOD_BKGND_SET);

	int barWidth = (int)(value / maxValue * width);
	if (barWidth > 0) {
		con->setDefaultBackground(barColor);
		con->rect(x, y, barWidth, 1, false, TCOD_BKGND_SET);
	}
	con->setDefaultForeground(TCODColor::white);
	con->printEx(x + width / 2, y, TCOD_BKGND_NONE, TCOD_CENTER, 
		"%s : %g/%g", name, value, maxValue);
}

void renderLine(int x, int y, int w, const TCODColor &lineColor) {
	//con->hline()
}

void Gui::renderMouseLook() {
	if (!engine.dungeon->isInFov(engine.mouse.cx, engine.mouse.cy)) { return; }
	char buf[128] = "";
	bool first = true;
	for (auto &ent : engine.entL) {
		if (ent->x == engine.mouse.cx && ent->y == engine.mouse.cy) {
			if (!first) {
				strcat_s(buf, ", ");
			}
			else {
				first = false;
			}
			strcat_s(buf, ent->name);
		}
	}
	con->setDefaultForeground(TCODColor::lightGrey);
	con->print(1, 0, buf);
}

Gui::Message::Message(const char *text, const TCODColor &col) : text(_strdup(text)), col(col) {}
Gui::Message::~Message() { delete [] text; }

void Gui::message(const TCODColor &col, const char *text, ...) {
	va_list ap; 
	char buf[128];
	va_start(ap, text);
	vsprintf_s(buf, text, ap); 
	va_end(ap);

	char *lineBegin = buf; 
	char *lineEnd;
	do {
		if (size(log) == msgHeight) {
			log.erase(begin(log));
		}
		lineEnd = strchr(lineBegin, '\n');
		if (lineEnd) {
			*lineEnd = '\0';
		}
		std::shared_ptr<Message> msg = std::make_shared<Message>(lineBegin, col);
		log.push_back(msg); 
		lineBegin = lineEnd + 1;
	} while (lineEnd);
}