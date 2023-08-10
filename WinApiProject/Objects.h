#pragma once

#include <Windows.h>
#include <gdiplus.h>
#include <vector>
#include "UIItems.h"

using namespace Gdiplus;
using namespace std;

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

	int mpos;//이동
	int mdirection;//바라보고 있는 방향
	int walking;

	int spriteX;
	int spriteY;
	int frame;

	int weapon;//무기 -> 1: 검 2: 창 3: 활

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



	void movePos(int pos);//이동
	void spriteNFrame();//이동 애니메이션 프레임 설정
	void action(Rect& rect, Graphics& g, Image*& playerAction,  ImageAttributes* &imageAtt);//이동 애니메이션

	void correctPosition(RECT& rectView);//맵 밖으로 나가게하지 못함

	void attackCollide(vector<Arrow>& arrows);//공격 충돌 판정
	void attack(Rect& rect, Graphics& g, Image*& attackAction, Image*& arrowAction  ,vector<Arrow>& arrows);//공격 애니메이션


	int changeWeapon(UIIcon& WeaponIcon);//무기 변경
};