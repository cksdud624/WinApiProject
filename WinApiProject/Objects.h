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
class Projectile;

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
	int walking;//걷는 애니메이션

	//플레이어 애니메이션
	int spriteX;
	int spriteY;
	int frame;

	int weapon;//무기 -> 1: 검 2: 창 3: 활

	//공격 애니메이션
	int attackframe;
	int attackSpriteX;

	//패링 시 잔상
	vector<Point> illusion;
	//공격 콜라이더
	POINT* attackPoints;

	//피격 효과
	int lefthitframe;
	int hitframe;

	//쿨타임
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

	void HitCheck(Monster& monster);//몬스터와의 충돌 체크
	void ProjHitCheck(Monster& monster);//몬스터가 날리는 투사체와의 충돌체크

	int changeWeapon(UIIcon& WeaponIcon);//무기 변경
};

class Monster
{
private:
	double x;
	double y;
	int sizex;
	int sizey;
	int life;

	//투사체 
	vector<Projectile> projectiles;

	int walking;//걷는 애니메이션

	int actionframe;
	int leftactionframe;

	//몬스터 애니메이션
	int pattern;
	int spriteX;
	int spriteY;
	int mpos;
	

public:
	Monster();
	double getX() { return x; }
	double getY() { return y; }
	int getLife() { return life; }
	int getSizeX() { return sizex; }
	int getSizeY() { return sizey; }
	vector<Projectile> getProjectiles() { return projectiles; }

	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }
	void setLife(int life) { this->life = life; }
	void setSizeX(int sizex) { this->sizex = sizex; }
	void setSizeY(int sizey) { this->sizey = sizey; }
	void setProjectile(vector<Projectile> projectiles) { this->projectiles = projectiles; }
	void action(Rect& rect, Graphics& g, Image*& bossAction);

	//통상 상태
	void normalMode(vector<POINT>& route, int GridXSize, int GridYSize, RECT &rectView, Player &player);
	//패턴 상태
	int Randomize(int min, int max);
	//투사체가 맵 밖으로 나가면 삭제
	void CheckProjectilesOutofArea(RECT& rectView);

	void HitCheck(Player& player, vector<Arrow>& arrows);
};

class Projectile
{
private:
	double x;
	double y;
	int radius;

	int speed;
	double angle;
public:
	double getX() { return x; }
	double getY() { return y; }
	int getRadius() { return radius; }
	int getSpeed() { return speed; }
	double getAngle() { return angle; }

	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }
	void setRadius(int radius) { this->radius = radius; }
	void setSpeed(int speed) { this->speed = speed; }
	void setAngle(double angle) { this->angle = angle; }

	void movePos();
};