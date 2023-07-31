#pragma once

#include <Windows.h>

class Player
{
private:
	int x;
	int y;
	int speed;
	int life;
public:
	int getX() { return x; }
	int getY() { return y; }
	int getSpeed() { return speed; }
	int getLife() { return life; }

	void setX(int x) { this->x = x; }
	void setY(int y) { this->y = y; }
	void setSpeed(int speed) { this->speed = speed; }
	void setLife(int life) { this->life = life; }
};