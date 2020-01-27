#include "pch.h"
#include "Character.h"
#include <algorithm>
#include "Powerup.h"
#include <string>
#include "Game.h"

Character::Character() : 
	GameObject{ { 0, 0 } },
	m_hitPoints{ 0 },
	m_attackPoints{ 0 }, 
	m_defensePoints{ 0 }
{
}

Character::Character(Point2D position, int health, int attack, int defense) :
	GameObject{ position },
	m_hitPoints{ health },
	m_attackPoints{ attack }, 
	m_defensePoints{ defense } 
{
}

Character::~Character()
{
}

void Character::addPowerup(Powerup* pPowerup) 
{
	m_powerups.push_back(pPowerup);
	std::sort(m_powerups.begin(), m_powerups.end(), Powerup::compare);
}

void Character::save(std::ofstream & out)
{
	if (!out.is_open())
		return;

	out.write((char*)&m_priority, sizeof(int));
	out.write((char*)&m_mapPosition.x, sizeof(Point2D));
	out.write((char*)&m_mapPosition.y, sizeof(int));
	out.write((char*)&m_hitPoints, sizeof(int));
	out.write((char*)&m_attackPoints, sizeof(int));
	out.write((char*)&m_defensePoints, sizeof(int));

	int count = m_powerups.size();
	out.write((char*)&count, sizeof(int));

	out << m_powerups.size() << std::endl;
	for (int i = 0; i < m_powerups.size(); i++) { 
		// save only the name of each powerup 
		// beacause each powerup will be saved by the Game class 
		// (the character only stores a pointer to these powerups) 
		// When loading the game class will need to re-link the pointers
		out.write(m_powerups[i]->getName(), 30);
	}
}

bool Character::load(std::ifstream & in, const Game * game)
{
	if (!in.is_open())
		return false;

	in.read((char*)&m_priority, sizeof(int)); 
	if (in.rdstate()) return false;
	
	in.read((char*)&m_mapPosition, sizeof(Point2D)); 
	if (in.rdstate()) return false; 
	
	in.read((char*)&m_hitPoints, sizeof(int)); 
	if (in.rdstate()) return false;
	
	in.read((char*)&m_attackPoints, sizeof(int)); 
	if (in.rdstate()) return false; 
	
	in.read((char*)&m_defensePoints, sizeof(int));
	if (in.rdstate()) return false; 

	int powerupCount; 
	in.read((char*)&powerupCount, sizeof(int));
	if (in.rdstate()) return false; 
	
	char name[30] = { 0 };
	for (int i = 0; i < powerupCount; i++) {
		name[0] = 0; 
		in.read(name, 30); 
		if (in.rdstate()) return false; 
		
		// match the name with the powerups loaded by the Game class
		Powerup* up = game->findPowerup(name, true); 
		m_powerups.push_back(up);
	}

	return true;
}
