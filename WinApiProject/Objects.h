#pragma once

#include <Windows.h>
#include <gdiplus.h>
#include <vector>
#include "UIItems.h"

using namespace Gdiplus;
using namespace std;

class Monster;
class Arrow;
class Player;

class Arrow
{
private:
	double x;
	double y;

	int speed;
	int width;
	int height;
	int angle;
	POINT* Collider;
public:
	double getX() { return x; }
	double getY() { return y; }
	int getSpeed() { return speed; }
	int getWidth() { return width; }
	int getHeight() { return height; }
	int getAngle() { return angle; }

	POINT* getCollider() { return Collider; }

	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }
	void setSpeed(int speed) { this->speed = speed; }
	void setWidth(int width) { this->width = width; }
	void setHeight(int height) { this->height = height; }
	void setAngle(int angle) { this->angle = angle; }

	void setNewCollider();
	void setCollider();
	void movePos();
};


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

	int weapon;//���� -> 1: �� 2: â 3: Ȱ

	int attackframe;
	int attackSpriteX;

	vector<Point> illusion;
	POINT* attackPoints;


	int attackCoolDown;
	int leftAttackCoolDown;

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
	int getAttackFrame() { return attackframe; }
	int getWeaponType() { return weapon; }

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

	void attackCollide(vector<Arrow>& arrows);//���� �浹 ����
	void attack(Rect& rect, Graphics& g, Image*& attackAction, Image*& arrowAction  ,vector<Arrow>& arrows);//���� �ִϸ��̼�

	void HitCheck(Monster& monster);
	int CCW(POINT a, POINT b, POINT c);

	int changeWeapon(UIIcon& WeaponIcon);//���� ����
};

class Monster
{
private:
	double x;
	double y;
	int sizex;
	int sizey;
	int life;

	time_t patterntimer;
public:
	Monster();
	double getX() { return x; }
	double getY() { return y; }
	int getLife() { return life; }
	int getSizeX() { return sizex; }
	int getSizeY() { return sizey; }
	time_t getPatternTimer() { return patterntimer; }

	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }
	void setLife(int life) { this->life = life; }
	void setSizeX(int sizex) { this->sizex = sizex; }
	void setSizeY(int sizey) { this->sizey = sizey; }
	void setPatternTimer(time_t patterntimer) { this->patterntimer = patterntimer; }

	//��� ����
	void normalMode(vector<POINT>& route, int GridXSize, int GridYSize);
	//���� ����
	int Randomize(int min, int max);
};