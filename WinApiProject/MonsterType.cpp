#include "MonsterType.h"

#include <iostream>

void MonsterType2::normalMode(vector<POINT>& route, int GridXSize, int GridYSize, RECT& rectView, Player& player, const POINT grids, vector<Block>& blocks, vector<AnimationEffect>& animationeffects)
{
	moveProjectilesPos();
	CheckProjectilesOutofAreaorTime(rectView);

	if (leftactionframe > 0)
		leftactionframe--;
	else
	{
		leftactionframe = actionframe;
		pattern = Randomize(1, 4);
	}

	if (route.size() == 0)
		pattern = 5;

	if (pattern <= 3)
	{
		walking = 1;
		if (route.size() > 0)
		{
			int xlen = x - route[0].x * GridXSize;
			int ylen = y - route[0].y * GridYSize;

			int xdirection = 0;
			int ydirection = 0;

			if (xlen > 0)
				xdirection = 1;
			else if (xlen < 0)
				xdirection = -1;

			if (ylen > 0)
				ydirection = 1;
			else if (ylen < 0)
				ydirection = -1;


			if (ydirection == 1)
				spriteY = 3;
			else if (ydirection == -1)
				spriteY = 0;
			else
			{
				if (xdirection == 1)
					spriteY = 1;
				else
					spriteY = 2;
			}


			if (abs(xlen) <= 5 && abs(ylen) <= 5)
			{
				x = route[0].x * GridXSize;
				y = route[0].y * GridYSize;
				if (route.size() > 1)
				{
					cout << "ASTAR 삭제" << endl;
					route.erase(route.begin());
				}
			}
			else
			{
				x -= 5 * xdirection;
				y -= 5 * ydirection;
			}
		}


		if (pattern == 3)
		{

			if (leftactionframe == actionframe)
			{
				for (int i = 0; i < 6; i++)
				{
					int gridx = Randomize(2, grids.x - 2);
					int gridy = Randomize(2, grids.y - 2);
					DangerZone dangerzone(30, 1, 0);
					dangerzone.setX(gridx * GridXSize);
					dangerzone.setY(gridy * GridYSize);
					dangerzone.setRadius(40);
					dangerzone.activatezone();
					dangerzones.push_back(dangerzone);
				}
			}
		}
	}
	else if (pattern == 4)
	{
		walking = 0;

		if (leftactionframe == actionframe)
		{
			DangerZone dangerzone(30, 0, 1);
			dangerzone.setX(player.getX());
			dangerzone.setY(player.getY());
			dangerzone.setRadius(100);
			dangerzone.activatezone();

			dangerzones.push_back(dangerzone);
		}
	}


	for (int i = 0; i < dangerzones.size(); i++)
	{
		dangerzones[i].checkframe(projectiles, animationeffects);
	}

	for (int i = 0; i < dangerzones.size(); i++)
	{
		if (dangerzones[i].getleftframe() <= 0)
		{
			cout << "레드존 삭제" << endl;
			dangerzones.erase(dangerzones.begin() + i);
			i = -1;
		}
	}
}

void MonsterType2::patternMode(RECT& rectView, int GridXSize, int GridYSize, POINT Grids, Player& player, vector<AnimationEffect>& animationeffects)
{
}

void MonsterType2::groggyMode()
{
}
