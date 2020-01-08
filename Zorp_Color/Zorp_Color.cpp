// Zorp_Color.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include "pch.h"
#include <iostream>
#include <random>
#include <Windows.h>
#include <time.h>
#include "Point2D.h"
#include "GameDefines.h"

using namespace std;



bool enableVirtualTerminal()
{
	// Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode))
	{
		return false;
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode))
	{
		return false;
	}
	return true;
}

void initialize(int map[MAZE_HEIGHT][MAZE_WIDTH])
{
	srand(time(nullptr));

	// fill the arrays with random room types
	for (int y = 0; y < MAZE_HEIGHT; y++)
	{
		for (int x = 0; x < MAZE_WIDTH; x++) {
			int type = rand() % (MAX_RANDOM_TYPE * 2);
			if (type < MAX_RANDOM_TYPE)
				map[y][x] = type;
			else
				map[y][x] = EMPTY;
		}
	}

	// set the entrance and exit of the maze
	map[0][0] = ENTRANCE;
	map[MAZE_HEIGHT - 1][MAZE_WIDTH - 1] = EXIT;
}

void drawWelcomeMessage()
{
	cout << TITLE << MAGENTA << "Welcome to ZORP!" << RESET_COLOR << endl;
	cout << INDENT << "ZORP is a game of adventure, danger, and low cunning." << endl;
	cout << INDENT << "It is definitely not related to any other text-based adventure game." << endl << endl;
}

void drawRoom(int map[MAZE_HEIGHT][MAZE_WIDTH], Point2D position)
{
	// find the console output position
	int outX = INDENT_X + (6 * position.x) + 1;
	int outY = MAP_Y + position.y;

	// jump to the correct location
	cout << CSI << outY << ";" << outX << "H";
	// draw the room
	switch (map[position.y][position.x])
	{
	case EMPTY:
		cout << "[ " << GREEN << "\xb0" << RESET_COLOR << " ] ";
		break;
	case ENEMY:
		cout << "[ " << RED << "\x94" << RESET_COLOR << " ] ";
		break;
	case TREASURE:
		cout << "[ " << YELLOW << "$" << RESET_COLOR << " ] ";
		break;
	case FOOD:
		cout << "[ " << WHITE << "\xcf" << RESET_COLOR << " ] ";
		break;
	case ENTRANCE:
		cout << "[ " << WHITE << "\x9d" << RESET_COLOR << " ] ";
		break;
	case EXIT:
		cout << "[ " << WHITE << "\xFE" << RESET_COLOR << " ] ";
		break;
	}
}

void drawMap(int map[MAZE_HEIGHT][MAZE_WIDTH])
{
	Point2D position = { 0, 0 };
	// reset draw colors
	cout << RESET_COLOR;
	for (position.y = 0; position.y < MAZE_HEIGHT; position.y++)
	{
		cout << INDENT;
		for (position.x = 0; position.x < MAZE_WIDTH; position.x++) {
			drawRoom(map,position);
		}
		cout << endl;
	}
}

void drawRoomDescription(int roomType)
{
	// reset draw colors
	cout << RESET_COLOR;
	// jump to the correct location
	cout << CSI << ROOM_DESC_Y << ";" << 0 << "H";
	// Delete 4 lines and insert 4 empty lines
	cout << CSI << "4M" << CSI << "4L" << endl;

	// write description of current room
	switch (roomType) {
	case EMPTY:
		cout << INDENT << "You are in an empty meadow. There is nothing of note here." << endl;
		break;
	case ENEMY:
		cout << INDENT << RED << "BEWARE." << RESET_COLOR << " An enemy is approaching." << endl;
		break;
	case TREASURE:
		cout << INDENT << "Your journey has been rewarded. You have found some treasure." << endl;
		break;
	case FOOD:
		cout << INDENT << "At last! You collect some food to sustain you on your journey." << endl;
		break;
	case ENTRANCE:
		cout << INDENT << "The entrance you used to enter this maze is blocked. There is no going back." << endl;
		break;
	case EXIT:
		cout << INDENT << "Despite all odds, you made it to the exit. Congratulations." << endl;
		break;
	}
}

void drawPlayer(Point2D position) {
	Point2D outPos = { 
		INDENT_X + (6 * position.x) + 3,
		MAP_Y + position.y };
	// draw the player's position on the map 
	// move cursor to map pos and delete character at current position 
	cout << CSI << outPos.y << ";" << outPos.x << "H"; 
	cout << MAGENTA << "\x81" << RESET_COLOR; 
}

void drawValidDirections(int x, int y)
{
	// reset draw colors
	cout << RESET_COLOR;
	// jump to the correct location
	cout << CSI << MOVEMENT_DESC_Y + 1 << ";" << 0 << "H";
	cout << INDENT << "You can see paths leading to the " <<
		((x > 0) ? "west, " : "") <<
		((x < MAZE_WIDTH - 1) ? "east, " : "") <<
		((y > 0) ? "north, " : "") <<
		((y < MAZE_HEIGHT - 1) ? "south, " : "") << endl;
}

int getMovementDirection()
{
	// jump to the correct location
	cout << CSI << PLAYER_INPUT_Y << ";" << 0 << "H";
	cout << INDENT << "Where to now?";

	int direction;
	// move cursor to position for player to enter input
	cout << CSI << PLAYER_INPUT_Y << ";" << PLAYER_INPUT_X << "H" << YELLOW;

	// clear the input buffer, ready for player input
	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());

	cin >> direction;
	cout << RESET_COLOR;

	if (cin.fail())
		return 0;
	return direction;
}

int getCommand() 
{ // for now, we can't read commands longer than 50 characters 
	char input[50] = "\0";
	// jump to the correct location 
	cout << CSI << PLAYER_INPUT_Y << ";" << 0 << "H";
	// clear any existing text 
	cout << CSI << "4M"; cout << INDENT << "Enter a command."; 
	// move cursor to position for player to enter input
	cout << CSI << PLAYER_INPUT_Y << ";" << PLAYER_INPUT_X << "H" << YELLOW; 
	// clear the input buffer, ready for player input
	cin.clear(); 
	cin.ignore(cin.rdbuf()->in_avail()); 
	cin >> input; cout << RESET_COLOR; 
	bool bMove = false; 
	while (input) { 
		if (strcmp(input, "move") == 0) { 
			bMove = true; 
		} 
		else if (bMove == true) { 
			if (strcmp(input, "north") == 0) 
				return NORTH;
			if (strcmp(input, "south") == 0) 
				return SOUTH; 
			if (strcmp(input, "east") == 0) 
				return EAST; 
			if (strcmp(input, "west") == 0) 
				return WEST; 
		} 

			if (strcmp(input, "look") == 0) { 
				return LOOK; 
			} 
			
			if (strcmp(input, "fight") == 0) { 
				return FIGHT; 
			} 
			
			char next = cin.peek();
			if (next == '\n' || next == EOF)
				break; 
			cin >> input; 
	} 

		return 0; 
}

void main() {
	// create a 2D array
	int rooms[MAZE_HEIGHT][MAZE_WIDTH];

	bool gameOver = false;

	Point2D player = { 0, 0 };

	if (enableVirtualTerminal() == false) {
		cout << "The virtual terminal processing mode could not be activated." << endl;
		cout << "Press 'Enter' to exit." << endl;
		cin.get();
		return;
	}

	initialize(rooms);

	drawWelcomeMessage();

	// output the map
	drawMap(rooms);

	// game loop
	while (!gameOver)
	{
		drawRoomDescription(rooms[player.y][player.x]);

		drawPlayer(player);

		if (rooms[player.y][player.x] == EXIT) {
			gameOver = true;
			continue;
		}

		// list the directions the player can take
		drawValidDirections(player.x, player.y);

		int command = getCommand();

		// before updating the player position, redraw the old room character over 
		// the old position
		drawRoom(rooms, player);

		// update the player's position using the input movement data
		switch (command) {
		case EAST:
			if (player.x < MAZE_WIDTH - 1)
				player.x++;
			break;
		case WEST:
			if (player.x > 0)
				player.x--;
			break;
		case NORTH:
			if (player.y > 0)
				player.y--;
			break;
		case SOUTH:
			if (player.y < MAZE_HEIGHT - 1)
				player.y++;
		break;
		case LOOK: 
			drawPlayer(player); 
				cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You look around, but see nothing worth mentioning" << endl;
				cout << INDENT << "Press 'Enter' to continue."; 
				cin.clear(); 
				cin.ignore(cin.rdbuf()->in_avail()); 
				cin.get(); 
		break; 
		case FIGHT: 
			drawPlayer(player);
			cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You could try to fight, but you don't have a weapon" << endl; 
			cout << INDENT << "Press 'Enter' to continue."; 
			cin.clear(); 
			cin.ignore(cin.rdbuf()->in_avail()); 
			cin.get();
		break;
		default: // the direction was not valid, 
				 // do nothing, go back to the top of the loop and ask again 
			drawPlayer(player);
			cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You try, but you just can't do it." << endl;
			cout << INDENT << "Press 'Enter' to continue.";
			cin.clear(); 
			cin.ignore(cin.rdbuf()->in_avail());
			cin.get(); 
		break;
		}
	} // end game loop

	cout << CSI << PLAYER_INPUT_Y << ";" << 0 << "H";	// jump to the correct location
	cout << endl << INDENT << "Press 'Enter' to exit the program.";
	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());
	cin.get();
}
