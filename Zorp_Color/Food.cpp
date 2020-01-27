#include "pch.h"
#include "Food.h"
#include "GameDefines.h" 
#include <iostream> 
#include <string>

Food::Food() : m_hitPoints{ 10 }
{ 
	m_priority = PRIORITY_FOOD;
};

Food::Food(Point2D position) : GameObject{ position }, m_hitPoints{ 10 }
{ 
	m_priority = PRIORITY_FOOD; 
}; 

void Food::draw() 
{ 
	std::cout << "[ " << WHITE << "\xcf" << RESET_COLOR << " ] ";
} 

void Food::drawDescription() 
{
	std::cout << INDENT << "You smell a recently extinguished campfire, perhaps left by a previous traveller." << std::endl;
}

void Food::lookAt() 
{ 
	std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "There is some food here. It should be edible." << std::endl; 
}

void Food::save(std::ofstream& out)
{
	if (!out.is_open())
		return;

	/*out << m_priority << ",";
	out << m_mapPosition.x << ",";
	out << m_mapPosition.y << ",";
	out << m_hitPoints << ",";*/

	out.write((char*)&m_priority, sizeof(int));
	out.write((char*)&m_mapPosition, sizeof(Point2D));
	out.write((char*)&m_hitPoints, sizeof(int));
}

bool Food::load(std::ifstream & in, const Game * game)
{
	if (!in.is_open())
		return false;

	char buffer[50] = { 0 };

	//load prioity
	in.read((char*)&m_priority, sizeof(int));
	if (in.rdstate()) return false;

	in.read((char*)&m_mapPosition, sizeof(Point2D));
	if (in.rdstate()) return false;

	in.read((char*)&m_hitPoints, sizeof(int));
	if (in.rdstate()) return false;
}

