#include "pch.h"
#include "Game.h"
#include <iostream>
#include <Windows.h>
#include <random>
#include <time.h>
#include "Enemy.h"
#include "Food.h"
#include "Powerup.h"


Game::Game() : m_gameOver{ false }
{

}


Game::~Game()
{

	delete[] m_powerups;
	delete[] m_enemies;
	delete[] m_food;
}

bool Game::startup() {
	
	if (enableVirtualTerminal() == false) {
		std::cout << "The virtual terminal processing mode could not be activated." << std::endl;
		std::cout << "Press 'Enter' to exit." << std::endl;
		std::cin.get();
		return true;
	}

	srand(time(nullptr));

	initializeMap();
	initializeEnemies();
	initializeFood();
	initializePowerups();

	m_player.setPosition(Point2D{ 0, 0 });

	drawWelcomeMessage();

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

	if (command == QUIT)
	{
		m_gameOver = true;
		return;
	}

	m_player.executeCommand(command, &m_map[playerPos.y][playerPos.x]);

	for (int i = 0; i < m_enemyCount; i++) {
		if (m_enemies[i].isAlive() == false) {
			Point2D pos = m_enemies[i].getPosition();
			m_map[pos.y][pos.x].setEnemy(nullptr);
		}
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
	

	//Set room positions
	for (int y = 0; y < MAZE_HEIGHT; y++)
	{
		for (int x = 0; x < MAZE_WIDTH; x++) {
			m_map[y][x].setPosition(Point2D{ x, y });
		}
	}

	// set the entrance and exit of the maze
	m_map[0][0].setType(ENTRANCE);
	m_map[MAZE_HEIGHT - 1][MAZE_WIDTH - 1].setType(EXIT);
}

void Game::initializeEnemies()
{
	//Create a dynamic array of enemies
	m_enemyCount = 1 + rand() % 4;
	m_enemies = new Enemy[m_enemyCount];

	//Place each enemy in a random room
	for (int i = 0; i < m_enemyCount; i++) {
		//Make sure there arent any enemies on the entrance or exit
		int x = 2 + (rand() % (MAZE_WIDTH - 3));
		int y = 2 + (rand() % (MAZE_HEIGHT - 3));

		//SEt the enemy's position
		m_enemies[i].setPosition({ x, y });
		//Set the rooms enemy
		m_map[y][x].setEnemy(&m_enemies[i]);
	}
}

void Game::initializeFood()
{
	m_foodCount = 3;
	m_food = new Food[m_foodCount];

	//Place each enemy in a random room
	for (int i = 0; i < m_enemyCount; i++) {

		int x = 2 + (rand() % (MAZE_WIDTH - 1));
		int y = 2 + (rand() % (MAZE_HEIGHT - 1));

		//Set the rooms food
		m_map[y][x].setFood(&m_food[i]);
	}
}

void Game::initializePowerups()
{
	m_powerupCount = 3;
	m_powerups = new Powerup[m_powerupCount];

	//Place each enemy in a random room
	for (int i = 0; i < m_enemyCount; i++) {
		
		char name[30] = "";

		int x = rand() % (MAZE_WIDTH - 1);
		int y = rand() % (MAZE_HEIGHT - 1);

		int item = rand() % 15;
		

		float HP = 1;
		float AT = 1;
		float DF = 1;

		switch (i) {
		case 0:
			strcpy_s(name, "Potion of ");
			m_powerups[i].setHealthMultiplier(1.1f);
			break;
		case 1:
			strcpy_s(name, "Sword of ");
			m_powerups[i].setAttackMultiplier(1.1f);
			break;
		case 2:
			strcpy_s(name, "Shield of ");
			m_powerups[i].setDefenseMultiplier(1.1f);
			break;
		}


		//Add the second part of the item name
		strncat_s(name, descriptors[rand() % 15], 30);

		//Set the powers name
		m_powerups[i].setName(name);
		
		//Set the rooms powerup
		m_map[y][x].setPowerup(&m_powerups[i]);
	}

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
	std::cout << CSI << "5M";

	//insert 5 blank lines to ensure the inventory output remains correct
	std::cout << CSI << "5L";

	std::cout << INDENT << "Enter a command.";

	// move cursor to position for player to enter input
	std::cout << CSI << PLAYER_INPUT_Y << ";" << PLAYER_INPUT_X << "H" << YELLOW;

	// clear the input buffer, ready for player input
	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());

	std::cin >> input; 
	std::cout << RESET_COLOR;


	bool bMove = false;
	bool bPickup = false;
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
		if (strcmp(input, "exit") == 0) {
			return QUIT;
		}
		if (strcmp(input, "pick") == 0) {
			bPickup = true;
		}
		else if (bPickup == true) {
			if (strcmp(input, "up") == 0)
				return PICKUP;
		}
		char next = std::cin.peek();
		if (next == '\n' || next == EOF)
			break;
		std::cin >> input;
	}

	return 0;
}