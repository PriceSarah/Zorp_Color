#include "pch.h"
#include "Player.h"
#include "GameDefines.h"
#include "Powerup.h"
#include <iostream>
#include <algorithm>


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

void Player::setPosition(Point2D position) {
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

bool Player::executeCommand(int command) {
	switch (command) {
	case EAST:
		if (m_mapPosition.x < MAZE_WIDTH - 1)
			m_mapPosition.x++;
		return true;
		break;
	case WEST:
		if (m_mapPosition.x > 0)
			m_mapPosition.x--;
		return true;
		break;
	case NORTH:
		if (m_mapPosition.y > 0)
			m_mapPosition.y--;
		return true;
		break;
	case SOUTH:
		if (m_mapPosition.y < MAZE_HEIGHT - 1)
			m_mapPosition.y++;
		return true;
		break;
	}
	return false;
}

//bool Player::pickup(Room* room)
//{
//	static const char descriptors[15][30] = {
//		"Indifference", "Invisibility", "Invulnerability",
//		"Incontinence", "Improbability", "Impatience",
//		"Indecision", "Inspiration", "Independence",
//		"Incurability", "Integration", "Invocation",
//		"Inferno", "Indigestion", "Inoculation"
//	};
//	int item = rand() % 15;
//
//	char name[30];
//	//strcpy_s(name, 30, itemNames[item]);
//
//	switch (room->getType()) {
//	case TREASURE_HP:
//		strcpy_s(name, "Potion of ");
//		break;
//	case TREASURE_AT:
//		strcpy_s(name, "Sword of ");
//		break;
//	case TREASURE_DF:
//		strcpy_s(name, "Shield of ");
//		break;
//	default:
//		return false;
//	}
//	strncat_s(name, descriptors[item], 30);
//
//
//	m_powerups.push_back(Powerup(name, 1, 1, 1));
//	std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You pick up the " << name << "." << std::endl;
//
//	std::sort(m_powerups.begin(), m_powerups.end(), Powerup::compare);
//	//set room to empty
//	room->setType(EMPTY);
//
//	std::cout << INDENT << "Press 'Enter' to continue.";
//	std::cin.clear();
//	std::cin.ignore(std::cin.rdbuf()->in_avail());
//	std::cin.clear();
//	std::cin.ignore(std::cin.rdbuf()->in_avail());
//	std::cin.get();
//	return true;
//
//}