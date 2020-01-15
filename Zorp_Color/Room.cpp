#include "pch.h"
#include "Room.h"
#include <iostream>
#include "GameDefines.h"
#include "Powerup.h"
#include "Player.h"
#include "Food.h"




Room::Room() : m_type{ EMPTY }, m_mapPosition{ 0, 0 }, m_powerup{ nullptr }, m_enemy{ nullptr }, m_food{ nullptr }
{
	
}


Room::~Room()
{
	if (m_powerup != nullptr) 
		delete m_powerup;


}

void Room::setPosition(Point2D position) 
{
	m_mapPosition = position;
}

void Room::setType(int type) 
{
	m_type = type;

	
}

int Room::getType() 
{
	return m_type;
}

void Room::draw() {
// find the console output position 
	int outX = INDENT_X + (6 * m_mapPosition.x) + 1;
	int outY = MAP_Y + m_mapPosition.y;
// jump to the correct location
std::cout << CSI << outY << ";" << outX << "H";
// draw the room 
	switch (m_type) {
	case EMPTY:
		if (m_enemy != nullptr) {
			std::cout << "[ " << RED << "\x94" << RESET_COLOR << " ]";
			break;
		}
		if (m_powerup != nullptr) {
			std::cout << "[ " << YELLOW << "$" << RESET_COLOR << " ]";
			break;
		}
		if (m_food != nullptr) {
			std::cout << "[ " << WHITE << "\xcf" << RESET_COLOR << " ]";
			break;
		}
		std::cout << "[ " << WHITE << "\xb0" << RESET_COLOR << " ]";
		break;
	case ENTRANCE:
		std::cout << "[ " << WHITE << "\x9d" << RESET_COLOR << " ] ";
		break;
	case EXIT:
		std::cout << "[ " << WHITE << "\xFE" << RESET_COLOR << " ] ";
		break;
}
}
	
void Room::drawDescription()
{
	// reset draw colors
	std::cout << RESET_COLOR;
	// jump to the correct location
	std::cout << CSI << ROOM_DESC_Y << ";" << 0 << "H";
	// Delete 4 lines and insert 4 empty lines
	std::cout << CSI << "4M" << CSI << "4L" << std::endl;

	// write description of current room
	switch (m_type) {
	case EMPTY:
		if (m_enemy != nullptr) {
			std::cout << INDENT << RED << "BEWARE." << RESET_COLOR << " An enemy is approching." << std::endl;
			break;
		}
		if (m_powerup != nullptr) {
			std::cout << INDENT  << "There appears to be some treasure here. Perhaps you should investigate further." << std::endl;
			break;
		}
		if (m_food != nullptr) {
			std::cout << INDENT << "You smell a recently extinguished campfire. Perhaps left by a previous traveller." << std::endl;
			break;
		}
		std::cout << INDENT << "You are in an empty meadow. There is nothing of note here." << std::endl;
		break;
	case ENTRANCE:
		std::cout << INDENT << "The entrance you used to enter this maze is blocked. There is no going back." << std::endl;
		break;
	case EXIT:
		std::cout << INDENT << "Despite all odds, you made it to the exit. Congratulations." << std::endl;
		break;
	}
}

//bool Room::executeCommand(int command, Player* player) {
//	std::cout << EXTRA_OUTPUT_POS;
//	switch (command) {
//
//	case LOOK:
//		if (m_type == TREASURE_HP || m_type == TREASURE_AT || m_type == TREASURE_DF)
//		{
//			std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "There is some treasure here. It looks small enough to pick up." << std::endl;
//		}
//		else {
//			std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You look around but see nothing worth mentioning." << std::endl;
//		}
//		std::cout << INDENT << "Press 'Enter' to continue.";
//		std::cin.clear();
//		std::cin.ignore(std::cin.rdbuf()->in_avail());
//		std::cin.get();
//		return true;
//		break;
//	case FIGHT:
//		draw();
//		std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You could try to fight, but you don't have a weapon" << std::endl;
//		std::cout << INDENT << "Press 'Enter' to continue.";
//		std::cin.clear();
//		std::cin.ignore(std::cin.rdbuf()->in_avail());
//		std::cin.get();
//		return true;
//		break;
//	case PICKUP:
//		return pickup(player);
//	default: // the direction was not valid, 
//			 // do nothing, go back to the top of the loop and ask again 
//		draw();
//		std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You try, but you just can't do it." << std::endl;
//		std::cout << INDENT << "Press 'Enter' to continue.";
//		std::cin.clear();
//		std::cin.ignore(std::cin.rdbuf()->in_avail());
//		std::cin.get();
//		return false;
//		break;
//	}
//}

bool Room::pickup(Player* player)
{
	//check that there is a power up
	if (m_powerup == nullptr) {
		std::cout << EXTRA_OUTPUT_POS << RESET_COLOR
			<< "There is nothing to pick up " << m_powerup->getName() << "." << std::endl;
		}
	
	//add powerup to inventory
	std::cout << EXTRA_OUTPUT_POS << RESET_COLOR 
		<< "You pick up the " << m_powerup->getName() << "." << std::endl;
	player->addPowerup(m_powerup);
	
	//remove powerup
	m_powerup = nullptr;
	m_type = EMPTY;

	std::cout << INDENT << "Press 'Enter' to continue.";
	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	std::cin.get();
	return true;
}
