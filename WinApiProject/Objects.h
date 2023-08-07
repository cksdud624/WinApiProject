#pragma once

#include <Windows.h>
#include <gdiplus.h>
#include <vector>

using namespace Gdiplus;
using namespace std;

class Player
{
private:
	double x;
	double y;
	int speed;
	int life;
	int pcount;

	int width;
	int height;

	int mpos;//�̵�
	int mdirection;//�ٶ󺸰� �ִ� ����
	int walking;

	int spriteX;
	int spriteY;
	int frame;

	int swordframe;
	int swordSpriteX;

	vector<Point> illusion;
	POINT* attackPoints;


public:



	Player();
	~Player();
	double getX() { return x; }
	double getY() { return y; }
	int getSpeed() { return speed; }
	int getLife() { return life; }
	int getpCount() { return pcount; }
	int getWidth() { return width; }
	int getHeight() { return height; }
	int getmPos() { return mpos; }
	int getSpriteX() { return spriteX; }
	int getSpriteY() { return spriteY; }
	int getFrame() { return frame; }
	int getSwordFrame() { return swordframe; }

	POINT* getAttackPoints() { return attackPoints; }

	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }
	void setSpeed(int speed) { this->speed = speed; }
	void setLife(int life) { this->life = life; }
	void setpCount(int pcount) { this->pcount = pcount; }
	void setWidth(int width) { this->width = width; }
	void setHeight(int height) { this->height = height; }
	void setmPos(int mpos) { this->mpos = mpos; }
	void setSpriteX(int spriteX) { this->spriteX = spriteX; }
	void setSpriteY(int spriteY) { this->spriteY = spriteY; }
	void setFrame(int frame) { this->frame = frame; }



	void movePos(int pos);//�̵�
	void spriteNFrame();//�̵� �ִϸ��̼� ������ ����
	void action(Rect& rect, Graphics& g, Image*& playerAction,  ImageAttributes* &imageAtt);//�̵� �ִϸ��̼�

	void correctPosition(RECT& rectView);//�� ������ ���������� ����

	void attackCollide();//���� �浹 ����
	void attack(Rect& rect, Graphics& g, Image*& swordAction);//���� �ִϸ��̼�
};