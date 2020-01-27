#include "pch.h"
#include "Powerup.h"
#include "GameDefines.h"
#include <string>
#include <iostream>

Powerup::Powerup() : m_healthMultiplier{ 1 }, m_attackMultiplier{ 1 }, m_defenseMultiplier{ 1 }
{
	m_priority = PRIORITY_POWERUP;
	strcpy_s(m_name, "nondescript item");
}

Powerup::Powerup(const char name[30], float health, float attack, float defense)
	: m_healthMultiplier { health }, m_attackMultiplier{ attack }, m_defenseMultiplier{ defense }
{
	m_priority = PRIORITY_POWERUP;
	strcpy_s(m_name, name);
}


Powerup::~Powerup()
{
}

char * Powerup::getName()
{
	return m_name;
}

bool Powerup::compare(const Powerup* p1, const Powerup* p2)
{
	if (strcmp(p1->m_name, p2->m_name) < 0)
	{
		return true;
	}
	else
	{
		return false;
	}

	return (strcmp(p1->m_name, p2->m_name) < 0) ? true : false;
}

void Powerup::setName(const char* pStr) {
	strncpy_s(m_name, pStr, 30);
}

void Powerup::draw()
{
	std::cout << "[ " << YELLOW << "$" << RESET_COLOR << " ] ";
}

void Powerup::drawDescription()
{
	std::cout << INDENT << "There appears to be some treasure here. Perhaps you should investigate futher." << std::endl;
}

void Powerup::lookAt()
{
	std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "There is some treasure here. It looks small enough to pick up." << std::endl;
}

void Powerup::save(std::ofstream & out)
{
	if (!out.is_open())
		return;

	/*out << m_priority << ",";
	out << m_mapPosition.x << ",";
	out << m_mapPosition.y << ",";
	out << m_name << ",";
	out << m_healthMultiplier << ",";
	out << m_attackMultiplier << ",";
	out << m_defenseMultiplier << ",";*/

	out.write((char*)&m_priority, sizeof(int));
	out.write((char*)&m_mapPosition, sizeof(Point2D));
	out.write(m_name, 30);
	out.write((char*)&m_healthMultiplier, sizeof(int));
	out.write((char*)&m_attackMultiplier, sizeof(int));
	out.write((char*)&m_defenseMultiplier, sizeof(int));
}

bool Powerup::load(std::ifstream & in, const Game* game)
{
	if (!in.is_open())
		return false;


	//load prioity
	in.read((char*)&m_priority, sizeof(int));
	if (in.rdstate()) return false;

	in.read((char*)&m_mapPosition, sizeof(Point2D));
	if (in.rdstate()) return false;

	in.read(m_name, 30);
	if (in.rdstate()) return false;

	in.read((char*)&m_healthMultiplier, sizeof(int));
	if (in.rdstate()) return false;

	in.read((char*)&m_attackMultiplier, sizeof(int));
	if (in.rdstate()) return false;

	in.read((char*)&m_defenseMultiplier, sizeof(int));
	if (in.rdstate()) return false;

	return true;
}
