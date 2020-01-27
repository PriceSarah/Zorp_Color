#include "pch.h"
#include "Game.h"
#include "Enemy.h"
#include "Powerup.h"
#include "Food.h"
#include <iostream>
#include <windows.h>
#include <random>
#include <time.h>
#include <fstream>
#include <string>


Game::Game() : m_gameOver{ false }, m_tempPowerups{ nullptr }
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
			m_map[pos.y][pos.x].removeGameObject( &m_enemies[i] );
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
		m_map[y][x].addGameObject(&m_enemies[i]);
	}
}

void Game::initializeFood()
{
	m_foodCount = 3;
	m_food = new Food[m_foodCount];

	//Place each enemy in a random room
	for (int i = 0; i < m_enemyCount; i++) {

		int x = rand() % (MAZE_WIDTH - 1);
		int y = rand() % (MAZE_HEIGHT - 1);

		m_food[i].setPosition(Point2D{ x, y });

		//Set the rooms food
		m_map[y][x].addGameObject(&m_food[i]);
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
		
		//Set the powerups position
		m_powerups[i].setPosition(Point2D{ x, y });

		//Set the rooms powerup
		m_map[y][x].addGameObject(&m_powerups[i]);
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
		if (strcmp(input, "save") == 0) {
			// process the save command here, as the game class is 
				// the only class with direct access to all the game objects
			save();
			return SAVE;

		}
		if (strcmp(input, "load") == 0) {
			if (!load()) {
				// could not open the file, display an error message
				std::cout << EXTRA_OUTPUT_POS << RED << "Failed to load zorpsave.dat." << std::endl;
			}
			return LOAD;
		}
		char next = std::cin.peek();
		if (next == '\n' || next == EOF)
			break;
		std::cin >> input;
	}

	return 0;
}

void Game::save()
{
	std::ofstream out;

	//open the file for output, and truncate (any contents that existed in the file before it is open are discarded)
	out.open("zorp_savegame.dat", std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);

	if (out.is_open()) {
		//save the position of every game object, and the players stats

		if (m_gameOver) {
			std::cout << EXTRA_OUTPUT_POS <<
				"You have perished. Saving will not save you." << std::endl;
			std::cout << INDENT << "Your progress has not been saved."
				<< RESET_COLOR << std::endl;
		}
		else {
			// output the powerups first, as these will need to be loaded before 
			// we load any characters (so we can correctly copy the powerup pointers 
			// to the characters' powerup list)
			out.write((char*)&m_powerupCount, sizeof(int));
			for (int i = 0; i < m_powerupCount; i++) {
				m_powerups[i].save(out);
			}

			out.write((char*)&m_enemyCount, sizeof(int));
			for (int i = 0; i < m_enemyCount; i++) {
				m_enemies[i].save(out);
			}

			out.write((char*)&m_foodCount, sizeof(int));
			for (int i = 0; i < m_foodCount; i++) {
				m_food[i].save(out);
			}
			m_player.save(out);
		}
	}
	else {
		//could not open the file, display an error message
		std::cout << EXTRA_OUTPUT_POS << RED
			<< "A grue has corrupted the Save File" << std::endl;
		std::cout << INDENT << "Your progress has not been saved." 
			<< RESET_COLOR << std::endl;
	}
	out.flush();
	out.close();
}

bool Game::load()
{
	std::ifstream in;
	in.open("zorp_savegame.dat", std::ifstream::in | std::ifstream::binary);

	if (!in.is_open()) 
	{ 
		return false; 
	} 
	
	char buffer[50] = { 0 }; 

	// load all the powerups 
	if (m_tempPowerups != nullptr)
		delete[] m_tempPowerups; 
	
	in.read((char*)&m_tempPowerupCount, sizeof(int));
	if (in.rdstate() || m_tempPowerupCount < 0)
		return false;
	
	m_tempPowerups = new Powerup[m_tempPowerupCount]; 
	for (int i = 0; i < m_tempPowerupCount; i++) 
	{
		if (m_tempPowerups[i].load(in, this) == false)
		{
			delete[] m_tempPowerups; 
			m_tempPowerups = nullptr; 
			return false; 
		} 
	} 
	
	// load all the enemies 
	int enemyCount;
	in.read((char*)&enemyCount, sizeof(int));
	if (in.rdstate() || enemyCount < 0)
		return false; 

	Enemy* enemies = new Enemy[enemyCount];
	for (int i = 0; i < enemyCount; i++)
	{ 
		if (enemies[i].load(in, this) == false) 
		{
			delete[] enemies; 
			delete[] m_tempPowerups; 
			m_tempPowerups = nullptr;
			return false; 
		} 
	} 
	// load all the food 
	int foodCount;
	in.read((char*)&foodCount, sizeof(int));
	if (in.rdstate() || foodCount < 0) 
		return false;

	Food* foods = new Food[foodCount]; 
	for (int i = 0; i < foodCount; i++) 
	{
		if (foods[i].load(in, this) == false)
		{ 
			delete[] foods; 
			delete[] enemies; 
			delete[] m_tempPowerups;
			m_tempPowerups = nullptr;
			return false; 
		} 
	} 
	// load the player 
	Player player;
	if (player.load(in, this) == false)
	{
		delete[] foods;
		delete[] enemies;
		delete[] m_tempPowerups;
		m_tempPowerups = nullptr;
		return false;
	}
	// everything succeeded up to here, so rebuild the level 
	// clean out the rooms 
	for (int y = 0; y < MAZE_HEIGHT; y++) 
	{
		for (int x = 0; x < MAZE_WIDTH; x++)
		{
			m_map[y][x].clearGameObjects(); 
		} 
	} 
	// add the powerups
	delete[] m_powerups;
	m_powerups = m_tempPowerups;
	m_powerupCount = m_tempPowerupCount;
	m_tempPowerups = nullptr; 

	for (int i = 0; i < m_powerupCount; i++) {
		Point2D pos = m_powerups[i].getPosition();
		if(pos.x >= 0 && pos.y >= 0)
			m_map[pos.y][pos.x].addGameObject(&m_powerups[i]); 
	} 
	// add the enemies 
	delete[] m_enemies;
	m_enemies = enemies; 
	m_enemyCount = enemyCount;

	for (int i = 0; i < m_enemyCount; i++) 
	{
		Point2D pos = m_enemies[i].getPosition();
		if (m_enemies[i].isAlive() && pos.x >= 0 && pos.x < MAZE_WIDTH && pos.y >= 0 && pos.y < MAZE_HEIGHT) 
			m_map[pos.y][pos.x].addGameObject(&m_enemies[i]); 
	} 

	// add the food
	delete[] m_food;
	m_food = foods; 
	m_foodCount = foodCount; 
	for (int i = 0; i < m_foodCount; i++) 
	{
		Point2D pos = m_food[i].getPosition();
		if (pos.x >= 0 && pos.x < MAZE_WIDTH && pos.y >= 0 && pos.y < MAZE_HEIGHT)
			m_map[pos.y][pos.x].addGameObject(&m_food[i]); 
	}	
		// add the player 
		m_player = player; 
		return true; 
}

Powerup* Game::findPowerup(const char* name, bool isLoading) const
{
	if (!isLoading) 
	{
		for (int i = 0; i < m_powerupCount; i++)
		{
			if (strcmp(name, m_powerups[i].getName()) == 0) 
				return &m_powerups[i];
		}
	}
	else 
	{ 
		// when loading, search the temp array 
		 for (int i = 0; i<m_tempPowerupCount; i++) 
		 { 
			if (strcmp(name, m_tempPowerups[i].getName()) == 0) 
				return &m_tempPowerups[i]; 
		 } 
	} 
	return nullptr;
}
