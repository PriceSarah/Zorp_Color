#include "pch.h"
#include "Player.h"
#include "GameDefines.h"
#include "Powerup.h"
#include <iostream>
#include <algorithm>
#include "Room.h"
#include "Enemy.h"
#include "Food.h"

Player::Player() : Character({ 0, 0 }, 100, 20, 20)
{
	m_priority = PRIORITY_PLAYER;
}

Player::Player(int x, int y) : Character({ x, y }, 100, 20, 20)
{
	
}

Player::~Player()
{
	for (std::vector<Powerup*>::iterator it = m_powerups.begin(); it < m_powerups.end(); it++) {
		delete *it;
	}
	m_powerups.clear();
}



void Player::draw() {
	int x = INDENT_X + (6 * m_mapPosition.x) + 3;
	int y = MAP_Y + m_mapPosition.y;

	// draw the player's position on the map 
	// move cursor to map pos and delete character at current position 
	std::cout << CSI << y << ";" << x << "H";
	std::cout << MAGENTA << "\x81" << RESET_COLOR;

	std::cout << INVENTORY_OUTPUT_POS;
	for (std::vector<Powerup*>::iterator it = m_powerups.begin(); it < m_powerups.end(); it++) {
		std::cout << (*it)->getName() << "\t";
	}
}


void Player::drawDescription() {}

void Player::lookAt() 
{ 
	std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "Hmmm, I look good!" << std::endl;
}

void Player::executeCommand(int command, Room* pRoom) {
	switch (command) {
	case EAST:
		if (m_mapPosition.x < MAZE_WIDTH - 1)
			m_mapPosition.x++;
		return;
		break;
	case WEST:
		if (m_mapPosition.x > 0)
			m_mapPosition.x--;
		return;
		break;
	case NORTH:
		if (m_mapPosition.y > 0)
			m_mapPosition.y--;
		return;
		break;
	case SOUTH:
		if (m_mapPosition.y < MAZE_HEIGHT - 1)
			m_mapPosition.y++;
		return;
		break;
	case LOOK:
		pRoom->lookAt();
		break;
	case FIGHT:
		attack(pRoom->getEnemy());
		break;
	case PICKUP:
		pickup(pRoom);
		break;
	case SAVE:
	case LOAD:
		// handled by the system before we get here. 
		// No need to process these commands
		break;
	default:
		std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You try but you just can't" << std::endl;
		break;
	}
	std::cout << INDENT << "Press 'Enter' to Continue.";
	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	std::cin.get();

	return;
}

void Player::pickup(Room* pRoom) {
	if (pRoom->getPowerup() != nullptr) {
		Powerup* powerup = pRoom->getPowerup(); 
		std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You pick up the " 
			<< powerup->getName() << std::endl; 
		// add the powerup to the player's inventory 
		addPowerup(powerup); 
		// remove the powerup from the room 
		// (but don't delete it, the player owns it now) 
		pRoom->removeGameObject(powerup); 
	} 
	else if (pRoom->getFood() != nullptr) {
		Food* food = pRoom->getFood(); 
		// eat the food m_healthPoints += food->getHP();
		std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << 
			"You feel refreshed. Your health is now " << m_hitPoints << 
			std::endl; 
		// remove the food from the room
		pRoom->removeGameObject(food); 
	} 
	else
	{ std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << 
		"There is nothing here to pick up." << std::endl; 
	}
}


void Player::attack(Enemy* pEnemy)
{
	if (pEnemy == nullptr) {
		std::cout << EXTRA_OUTPUT_POS << RESET_COLOR <<
			"There is no one here you can fight." << std::endl;
	}
	else {
		pEnemy->onAttacked(m_attackPoints);

		if (pEnemy->isAlive() == false) {
			std::cout << EXTRA_OUTPUT_POS << RESET_COLOR <<
			"You fight a grue and kill it." << std::endl;
		}
		else {
			int damage = pEnemy->getAT() - m_defensePoints;
			if (damage < 1) damage = 1;
			m_hitPoints -= damage;
				
			std::cout << EXTRA_OUTPUT_POS << RESET_COLOR <<
				"You fight a grue and take " << damage <<
				" points of damage. Your health is now " << m_hitPoints << std::endl;
			std::cout << INDENT << "The grue has " << pEnemy->getHP() << " health remaining." << std::endl;
		}	
	}
}