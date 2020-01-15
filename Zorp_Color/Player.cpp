#include "pch.h"
#include "Player.h"
#include "GameDefines.h"
#include "Powerup.h"
#include <iostream>
#include <algorithm>
#include "Room.h"
#include "Enemy.h"
#include "Food.h"

Player::Player() : m_mapPosition{ 0, 0 },m_healthPoints{ 100 },m_attackPoints{ 20 },m_defensePoints{ 20 }
{
	
}

Player::Player(int x, int y) : m_mapPosition{ x, y }, m_healthPoints{ 100 }, m_attackPoints{ 20 },m_defensePoints{ 20 }
{
	
}

Player::~Player()
{
	for (std::vector<Powerup*>::iterator it = m_powerups.begin(); it < m_powerups.end(); it++) {
		delete *it;
	}
	m_powerups.clear();
}

void Player::addPowerup(Powerup* pPowerup)
{
	m_powerups.push_back(pPowerup);

	std::sort(m_powerups.begin(), m_powerups.end(), Powerup::compare);
}

void Player::setPosition(const Point2D& position) {
	m_mapPosition = position;
}

 Point2D Player::getPosition() {
	return m_mapPosition;
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
		if (pRoom->getEnemy() != nullptr) {
			std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "LOOK OUT! An enemy is approaching." << std::endl;
		}
		else if (pRoom->getPowerup() != nullptr) {
			std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "There is some treasure here. It looks small enough to pick up." << std::endl;
		}
		else if (pRoom->getFood() != nullptr) {
			std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "There is some food here. It seems like it should be edible." << std::endl;
		}
		else {
			std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You look around and see nothing worth mentioning." << std::endl;
		}
		break;
	case FIGHT:
		attack(pRoom->getEnemy());
		break;
	case PICKUP:
		pickup(pRoom);
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

void Player::pickup(Room* pRoom)
{
	if (pRoom->getPowerup() != nullptr) {
		std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You pick up the " <<
			pRoom->getPowerup()->getName() << std::endl;

		addPowerup(pRoom->getPowerup());

		pRoom->setPowerup(nullptr);
	}
	else if (pRoom->getFood() != nullptr) {

		m_healthPoints += pRoom->getFood()->getHP();
		std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You feel refreshed. Your health is now " << m_healthPoints << std::endl;
		pRoom->setFood(nullptr);
	}
	else {
		std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "There is nothing to pick up" << std::endl;
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
			if (damage < 0) damage = 0;
			m_healthPoints -= damage;

			std::cout << EXTRA_OUTPUT_POS << RESET_COLOR <<
				"You fight a grue and take " << damage <<
				" points of damage. Your health is now " << m_healthPoints << std::endl;
			std::cout << INDENT << "The grue has " << pEnemy->getHP() << " health remaining." << std::endl;
		}	
	}
}