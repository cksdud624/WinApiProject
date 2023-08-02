#pragma once

#include <Windows.h>

class Player
{
private:
	double x;
	double y;
	int speed;
	int life;
public:
	double getX() { return x; }
	double getY() { return y; }
	int getSpeed() { return speed; }
	int getLife() { return life; }

	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }
	void setSpeed(int speed) { this->speed = speed; }
	void setLife(int life) { this->life = life; }

	void movePos(int pos);
};