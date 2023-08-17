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
class DangerZone;
class AnimationEffect;

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
	int walking;//�ȴ� �ִϸ��̼�

	//�÷��̾� �ִϸ��̼�
	int spriteX;
	int spriteY;
	int frame;

	int weapon;//���� -> 1: �� 2: â 3: Ȱ

	//���� �ִϸ��̼�
	int attackframe;
	int attackSpriteX;

	//�и� �� �ܻ�
	vector<Point> illusion;
	//���� �ݶ��̴�
	POINT* attackPoints;

	//�ǰ� ȿ��
	int lefthitframe;
	int hitframe;

	//��Ÿ��
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
	void correctPosition(RECT& rectView, vector<POINT>& blocks, int GridXSize, int GridYSize);//�÷��̾� ��ġ ����
	void attackCollide(vector<Arrow>& arrows);//���� �浹 ����
	void attack(Rect& rect, Graphics& g, Image*& attackAction, Image*& arrowAction  ,vector<Arrow>& arrows);//���� �ִϸ��̼�
	void HitCheck(Monster& monster);//���Ϳ��� �浹 üũ
	void ProjHitCheck(Monster& monster);//���Ͱ� ������ ����ü���� �浹üũ
	int changeWeapon(UIIcon& WeaponIcon);//���� ����
	int cross(POINT a, POINT b, POINT c);//����
};

class Monster
{
private:
	double x;
	double y;
	int sizex;
	int sizey;
	int life;

	//����ü 
	vector<Projectile> projectiles;
	vector<DangerZone> dangerzones;

	int walking;//�ȴ� �ִϸ��̼�

	int actionframe;
	int leftactionframe;

	time_t patterntime;

	//���� �ִϸ��̼�
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
	time_t getPatterntime() { return patterntime; }

	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }
	void setLife(int life) { this->life = life; }
	void setSizeX(int sizex) { this->sizex = sizex; }
	void setSizeY(int sizey) { this->sizey = sizey; }
	void setProjectile(vector<Projectile> projectiles) { this->projectiles = projectiles; }
	void action(Rect& rect, Graphics& g, Image*& bossAction);

	//��� ����
	void normalMode(vector<POINT>& route, int GridXSize, int GridYSize, RECT &rectView, Player &player, const POINT grids, vector<POINT>& blocks
	, vector<AnimationEffect>& animationeffects);
	//���� ����
	void patternMode(RECT& rectView);

	int Randomize(int min, int max);
	//����ü�� �� ������ ������ ����
	void CheckProjectilesOutofArea(RECT& rectView);//����ü�� �� ������ �������� Ȯ��

	void HitCheck(Player& player, vector<Arrow>& arrows);//���� �ǰ� ����

	void drawProjectiles(Graphics& g, Image*& effect, HDC& mem1dc);//����ü�� �׸�
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

class AnimationEffect
{
private:
	double x;
	double y;
	int width;
	int height;

	int lefteffectframe;
	int effectframe;

	int spriteX;
	int spriteY;
public:
	AnimationEffect(double x, double y, int width, int height, int spriteX, int spriteY);
	int getSpriteX() { return spriteX; }
	void drawAnimationEffect(Graphics& g, Image*& effect);
};

class DangerZone
{
private:
	double x;
	double y;
	int radius;
public:
	double getX() { return x; }
	double getY() { return y; }
	int getRadius() { return radius; }

	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }
	void setRadius(int radius) { this->radius = radius; }
};