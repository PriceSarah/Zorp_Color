#include "pch.h"
#include "Game.h"
#include <iostream>
#include <Windows.h>
#include <random>
#include <time.h>


Game::Game()
{
	m_gameOver = false;
}


Game::~Game()
{
}

bool Game::startup() {
	// create a 2D array
	Room rooms[MAZE_HEIGHT][MAZE_WIDTH];

	Player player;
	player.setPosition(Point2D{ 0, 0 });

	if (enableVirtualTerminal() == false) {
		std::cout << "The virtual terminal processing mode could not be activated." << std::endl;
		std::cout << "Press 'Enter' to exit." << std::endl;
		std::cin.get();
		return true;
	}

	initializeMap();

	drawWelcomeMessage();


	// output the map
	//drawMap(rooms);
	return true;
}

void Game::draw() {
	drawValidDirections();
	Point2D playerPos = m_player.getPosition();
	m_map[playerPos.y][playerPos.x].drawDescription();
	drawMap();
	m_player.draw();
}

void Game::update()
{
	Point2D playerPos = m_player.getPosition();
	if (m_map[playerPos.y][playerPos.x].getType() == EXIT) {
		m_gameOver = true;
		return;
	}
	int command = getCommand();
	if (m_player.executeCommand(command)) {

	}
	else {
		m_map[playerPos.y][playerPos.x].executeCommand(command);
	}
}

bool Game::isGameOver() {
	return m_gameOver;
}

bool Game::enableVirtualTerminal()
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

void Game::initializeMap()
{
	srand(time(nullptr));

	// fill the arrays with random room types
	for (int y = 0; y < MAZE_HEIGHT; y++)
	{
		for (int x = 0; x < MAZE_WIDTH; x++) {
			int type = rand() % (MAX_RANDOM_TYPE * 2);
			if (type < MAX_RANDOM_TYPE)
				m_map[y][x].setType(type);
			else
				m_map[y][x].setType(EMPTY);
			m_map[y][x].setPosition(Point2D{ x, y });
		}
	}

	// set the entrance and exit of the maze
	m_map[0][0].setType(ENTRANCE);
	m_map[MAZE_HEIGHT - 1][MAZE_WIDTH - 1].setType(EXIT);
}

void Game::drawWelcomeMessage()
{
	std::cout << TITLE << MAGENTA << "Welcome to ZORP!" << RESET_COLOR << std::endl;
	std::cout << INDENT << "ZORP is a game of adventure, danger, and low cunning." << std::endl;
	std::cout << INDENT << "It is definitely not related to any other text-based adventure game." << std::endl << std::endl;
}

void Game::drawMap()
{

	// reset draw colors
	std::cout << RESET_COLOR;
	for (int y = 0; y < MAZE_HEIGHT; y++)
	{
		std::cout << INDENT;
		for (int x = 0; x < MAZE_WIDTH; x++) {
			m_map[y][x].draw();
			//drawRoom(map,position);
		}
		std::cout << std::endl;
	}
}

void Game::drawValidDirections()
{
	// reset draw colors
	std::cout << RESET_COLOR;
	// jump to the correct location
	std::cout << CSI << MOVEMENT_DESC_Y + 1 << ";" << 0 << "H";
	std::cout << INDENT << "You can see paths leading to the " <<
		((m_player.getPosition().x > 0) ? "west, " : "") <<
		((m_player.getPosition().x < MAZE_WIDTH - 1) ? "east, " : "") <<
		((m_player.getPosition().y > 0) ? "north, " : "") <<
		((m_player.getPosition().y < MAZE_HEIGHT - 1) ? "south, " : "") << std::endl;
}


int Game::getCommand()
{ // for now, we can't read commands longer than 50 characters 
	char input[50] = "\0";
	// jump to the correct location 
	std::cout << CSI << PLAYER_INPUT_Y << ";" << 0 << "H";
	// clear any existing text 
	std::cout << CSI << "4M"; std::cout << INDENT << "Enter a command.";
	// move cursor to position for player to enter input
	std::cout << CSI << PLAYER_INPUT_Y << ";" << PLAYER_INPUT_X << "H" << YELLOW;
	// clear the input buffer, ready for player input
	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	std::cin >> input; std::cout << RESET_COLOR;
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

		char next = std::cin.peek();
		if (next == '\n' || next == EOF)
			break;
		std::cin >> input;
	}

	return 0;
}