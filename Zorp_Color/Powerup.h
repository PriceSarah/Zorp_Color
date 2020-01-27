#pragma once
#include "GameObject.h"


class Powerup : public GameObject
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

	void draw();
	void drawDescription();
	void lookAt();
	void save(std::ofstream& out);
	bool load(std::ifstream& in, const Game* game);

private:
	char m_name[30];

	float m_healthMultiplier;
	float m_attackMultiplier;
	float m_defenseMultiplier;
};

