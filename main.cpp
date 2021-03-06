// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include "main.hpp"

Engine engine(100,80);

int main() {
	while (!TCODConsole::isWindowClosed()) {
		engine.update(); 
		engine.render();
		TCODConsole::flush();
	}
}
