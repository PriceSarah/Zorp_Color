// Zorp_Color.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include "pch.h"
#include <iostream>
#include <random>
#include <Windows.h>
#include <time.h>
#include "Point2D.h"
#include "GameDefines.h"
#include "Room.h"
#include "Player.h"
#include "Game.h"

using namespace std;

void main() {
	Game game;
	game.startup();
	

	// game loop
	while (!game.isGameOver())
	{
		game.draw();
		game.update();
	} // end game loop

	cout << CSI << PLAYER_INPUT_Y << ";" << 0 << "H";	// jump to the correct location
	cout << endl << INDENT << "Press 'Enter' to exit the program.";
	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());
	cin.get();
}
