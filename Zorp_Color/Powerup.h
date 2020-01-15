#pragma once
#include <cstring>


class Powerup
{
public:
	Powerup();
	Powerup(const char name[30], float health, float attack, float defense);
	~Powerup();

	void setName(const char* pStr);
	void setHealthMultiplier(float health) { m_healthMultiplier = health; }
	void setAttackMultiplier(float attack) { m_healthMultiplier = attack; }
	void setDefenseMultiplier(float defense) { m_healthMultiplier = defense; }

	char* getName();
	float getHealthMultiplier();
	float getAttackMultiplier();
	float getDefenseMultiplier();

	static bool compare(const Powerup* p1, const Powerup* p2);

private:
	char m_name[30];

	float m_healthMultiplier;
	float m_attackMultiplier;
	float m_defenseMultiplier;
};

