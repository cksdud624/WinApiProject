#include "MonsterType.h"
#include "Algorithms.h"
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
					route.erase(route.begin());
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
			dangerzones.erase(dangerzones.begin() + i);
			i = -1;
		}
	}
}

int MonsterType2::patternMode(RECT& rectView, int GridXSize, int GridYSize, POINT Grids, Player& player, vector<AnimationEffect>& animationeffects, int monsters)
{
	int check = 0;
	int maintain = 0;
	for (int i = 0; i < dangerzones.size(); i++)
	{
		dangerzones[i].checkframe(projectiles, animationeffects);
	}


	for (int i = 0; i < dangerzones.size(); i++)
	{
		if (dangerzones[i].getleftframe() <= 0)
		{
			dangerzones.erase(dangerzones.begin() + i);
			i = -1;
		}
	}


	moveProjectilesPos();
	CheckProjectilesOutofAreaorTime(rectView);

	if (patternstart == 0)
	{
		leftpatternprogress = patternprogress;
		walking = 0;
	}
	if (leftpatternprogress >= patternprogress - 100)
	{
		if (leftpatternprogress % 25 == 0)
		{
			int xrand = Randomize(1, 3);
			int yrand = Randomize(1, 3);
			while (xrand == 1 && yrand == 1)
			{
				xrand = Randomize(1, 3);
				yrand = Randomize(1, 3);
			}
			for (int i = 0; i < 3; i++)
			{
				DangerZone dangerzone(30, 0, 1);
				if (xrand == 1)
					dangerzone.setX(Grids.x / 2 * GridXSize);
				else if (xrand == 2)
					dangerzone.setX(GridXSize * ((2 - i) * 8) + GridXSize * 3);
				else
					dangerzone.setX(GridXSize * (i * 8) + GridXSize * 3);
				if (yrand == 1)
					dangerzone.setY(Grids.y / 2 * GridYSize);
				else if (yrand == 2)
					dangerzone.setY(GridYSize * ((2 - i) * 8) + GridYSize * 3);
				else
					dangerzone.setY(GridYSize * (i * 8) + GridYSize * 3);
				dangerzone.setRadius(100);
				dangerzone.activatezone();

				dangerzones.push_back(dangerzone);
			}
		}
	}
	else if (leftpatternprogress >= patternprogress - 200)
	{
		if (leftpatternprogress % 30 == 0)
		{
			check = 1;
			tempprogress = 20;
		}
	}
	else
	{
		if (monsters > 0)
			maintain = 1;

		if (tempprogress <= 0)
		{
			DangerZone dangerzone(30, 0, 1);
			dangerzone.setX(player.getX());
			dangerzone.setY(player.getY());
			dangerzone.setRadius(100);
			dangerzone.activatezone();

			dangerzones.push_back(dangerzone);
			tempprogress = 30;
		}
		else
			tempprogress--;

	}

	if(maintain == 0)
		leftpatternprogress--;
	if (check == 1)
		return 1;
	return 0;
}