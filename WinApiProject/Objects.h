#pragma once

#include <Windows.h>
#include <gdiplus.h>
#include <vector>
#include "UIItems.h"
#include "Items.h"

using namespace Gdiplus;
using namespace std;

class Monster;
class Arrow;
class Player;
class Projectile;
class DangerZone;
class AnimationEffect;
class Block;

class Block
{
private:
	int x;
	int y;
	int frame;
	int leftframe;
public:
	Block();
	int getX() { return x; }
	int getY() { return y; }
	int getLeftFrame() { return leftframe; }

	void setX(int x) { this->x = x; }
	void setY(int y) { this->y = y; }
	void setleftframe(int leftframe) { this->leftframe = leftframe; }

	void drawBlock(Graphics& g, Image* BlockImage, int GridXSize, int GridYSize);
	void update();
};

class Arrow
{
private:
	double x;
	double y;

	int speed;
	int width;
	int height;
	int angle;
	POINT Collider[4];
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
	int maxlife;
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
	POINT attackPoints[4];

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
	int getMaxLife() { return maxlife; }
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
	void correctPosition(RECT& rectView, vector<Block>& blocks, int GridXSize, int GridYSize);//플레이어 위치 보정
	void attackCollide(vector<Arrow>& arrows);//공격 충돌 판정
	void attack(Rect& rect, Graphics& g, Image*& attackAction, Image*& arrowAction  ,vector<Arrow>& arrows);//공격 애니메이션
	void useItem(Item& item, vector<AnimationEffect>& animationeffects);//아이템 사용
	int HitCheck(Monster& monster, vector<AnimationEffect>& animationeffects);//몬스터와의 충돌 체크
	int ProjHitCheck(Monster& monster, vector<AnimationEffect>& animationeffects);//몬스터가 날리는 투사체와의 충돌체크
	int changeWeapon(UIIcon& WeaponIcon);//무기 변경
	int cross(POINT a, POINT b, POINT c);//외적
};

class Monster
{
protected:
	double x;
	double y;
	int sizex;
	int sizey;
	int life;
	int maxlife;

	//투사체 
	vector<Projectile> projectiles;
	//위험 구역 생성
	vector<DangerZone> dangerzones;

	//걷는 애니메이션
	int walking;

	//몬스터 그로기 애니메이션
	int groggy;

	//몬스터 애니메이션
	int actionframe;
	int leftactionframe;

	//패턴 시작까지 남은 시간
	time_t patterntime;

	//패턴 진행
	int patternprogress;
	int leftpatternprogress;
	//패턴 시작
	int patternstart;

	//몬스터 애니메이션
	int pattern;
	int spriteX;
	int spriteY;
	int mpos;
	int frame;
	int leftframe;

	int walkingframe;
	
	int spritechanger;

public:
	Monster();
	double getX() { return x; }
	double getY() { return y; }
	int getLife() { return life; }
	int getSizeX() { return sizex; }
	int getSizeY() { return sizey; }
	vector<Projectile> getProjectiles() { return projectiles; }
	time_t getPatterntime() { return patterntime; }
	int getPatternStart() { return patternstart; }
	int getLeftPatternProgress() { return leftpatternprogress; }
	int getMaxLife() { return maxlife; }

	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }
	void setLife(int life) { this->life = life; }
	void setSizeX(int sizex) { this->sizex = sizex; }
	void setSizeY(int sizey) { this->sizey = sizey; }
	void setSpriteX(int spriteX) { this->spriteX = spriteX; }
	void setSpriteY(int spriteY) { this->spriteY = spriteY; }
	void setProjectile(vector<Projectile> projectiles) { this->projectiles = projectiles; }
	void setPatternStart(int patternstart) { this->patternstart = patternstart; }
	void setWalking(int walking) { this->walking = walking; }
	void setPatternTime(time_t patterntime) { this->patterntime = patterntime; }
	void setGroggy(int groggy) { this->groggy = groggy; }
	void setMaxLife(int maxlife) { this->maxlife = maxlife; }

	void ChasePlayer(vector<POINT>& route, int GridXSize, int GridYSize);


	//투사체 이동
	void moveProjectilesPos();
	//몬스터 애니메이션
	void action(Rect& rect, Graphics& g, Image*& bossAction);

	//통상 상태
	virtual void normalMode(vector<POINT>& route, int GridXSize, int GridYSize, RECT &rectView, Player &player, const POINT grids, vector<Block>& blocks
	, vector<AnimationEffect>& animationeffects);
	//패턴 상태
	virtual int patternMode(RECT& rectView, int GridXSize, int GridYSize, POINT Grids, Player& player, vector<AnimationEffect>& animationeffects, int monsters);
	//그로기 상태
	void groggyMode();
	int Randomize(int min, int max);

	//투사체가 맵 밖으로 나가면 삭제
	void CheckProjectilesOutofAreaorTime(RECT& rectView);
	//몬스터 피격 판정
	int HitCheck(Player& player, vector<Arrow>& arrows, vector<AnimationEffect>& animationeffects);
	//투사체를 그림
	void drawProjectiles(Graphics& g, Image** effect, Image*& projectile, Image*& projectile2);
	//위험 구역을 그림
	void drawDangerZones(Graphics& g, HDC& mem1dc);
	//투사체, 레드존 초기화
	void clearDangerZone();
};

class Projectile
{
private:
	double x;
	double y;
	int radius;

	int speed;
	double angle;

	int type;

	int projectileframe;
	int leftprojectileframe;

	int spriteX;
	int spriteY;
public:
	Projectile();
	double getX() { return x; }
	double getY() { return y; }
	int getRadius() { return radius; }
	int getSpeed() { return speed; }
	double getAngle() { return angle; }
	int getLeftProjectileFrame() { return leftprojectileframe; }
	int getType() { return type; }
	int getSpriteX() { return spriteX; }
	int getSpriteY() { return spriteY; }

	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }
	void setRadius(int radius) { this->radius = radius; }
	void setSpeed(int speed) { this->speed = speed; }
	void setAngle(double angle) { this->angle = angle; }
	void setLeftProjectileFrame(int leftprojectileframe) { this->leftprojectileframe = leftprojectileframe; }
	void setType(int type) { this->type = type; }
	void setSpriteX(int spriteX) { this->spriteX = spriteX; }
	void setSpriteY(int spriteY) { this->spriteY = spriteY; }

	void movePos();
};

class AnimationEffect
{
private:
	double x;
	double y;
	int width;
	int height;

	int type;

	int lefteffectframe;
	int effectframe;

	int spriteX;
	int spriteY;
public:
	AnimationEffect(double x, double y, int width, int height, int spriteX, int spriteY, int type);
	void setType(int type) { this->type = type; }
	int getSpriteX() { return spriteX; }
	void drawAnimationEffect(Graphics& g, Image** effect);
};

class DangerZone
{
private:
	double x;
	double y;
	int radius;
	int frame;
	int leftframe;
	int zonetype;
	int variation;
public:
	DangerZone(int frame, int zonetype,int variation);
	double getX() { return x; }
	double getY() { return y; }
	int getRadius() { return radius; }
	int getleftframe() { return leftframe; }

	void setX(double x) { this->x = x; }
	void setY(double y) { this->y = y; }
	void setRadius(int radius) { this->radius = radius; }

	void activatezone();
	int checkframe(vector<Projectile>& projectiles, vector<AnimationEffect>& animationeffects);
};