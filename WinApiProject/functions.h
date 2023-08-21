#pragma once

#include "Objects.h"
class DamageText
{
private:
	double x;
	double y;
	int leftframe;
	int frame;
	int overlen;
	int damage;
	int type;
public:
	int getDamage() { return damage; }
	double getX() { return x; }
	double getY() { return y; }
	int getOverLen() { return overlen; }
	int getType() { return type; }
	int getLeftFrame() { return leftframe; }


	DamageText();
	void setType(int type) { this->type = type; }
	void setDamage(int damage) { this->damage = damage; }
	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }
	void setOverLen(int overlen) { this->overlen = overlen; }

	void update(Player& player, Monster& monster);
};