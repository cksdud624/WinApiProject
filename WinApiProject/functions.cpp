#include "functions.h"

DamageText::DamageText()
{
	frame = 20;
	leftframe = frame;
	overlen = 0;
}

void DamageText::update(Player& player, Monster& monster)
{
	if (type == 0)
	{
		x = player.getX();
		y = player.getY();
	}
	else
	{
		x = monster.getX();
		y = monster.getY();
	}
	leftframe--;
	overlen += 2;
}
