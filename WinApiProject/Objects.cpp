#define _USE_MATH_DEFINES

#include "Objects.h"
#include <iostream>
#include <gdiplus.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <random>

using namespace std;

Player::Player()
{
	spriteX = 1;
	spriteY = 3;
	frame = 3;
	mdirection = 2;

	maxlife = 100;
	life = maxlife;
	speed = 10;
	width = 20;
	height = 27;

	attackCoolDown = 17;
	leftAttackCoolDown = 0;

	weapon = 1;

	hitframe = 30;
}

Player::~Player()
{
}

void Player::movePos(int pos)
{
	float move = 1;
	if (pcount <= 0)
	{
		if (GetKeyState(VK_SHIFT) < 0)
			pcount = 10;
		else
			mpos = pos;
	}
	else
	{
		pcount--;
		move *= 3;
	}
	switch (mpos)
	{
	case 1:
		x -= move * 0.6 * speed;
		y -= move * 0.6 * speed;
		break;
	case 2:
		y -= move * speed;
		break;
	case 3:
		x += move * 0.6 * speed;
		y -= move * 0.6 * speed;
		break;
	case 4:
		x -= move * speed;
		break;
	case 5:
		break;
	case 6:
		x += move * speed;
		break;
	case 7:
		x -= move * 0.6 * speed;
		y += move * 0.6 * speed;
		break;
	case 8:
		y += move * (*this).speed;
		break;
	case 9:
		x += move * 0.6 * speed;
		y += move * 0.6 * speed;
		break;
	default:
		break;
	}

	if (mpos == 5)
		walking = 0;
	else
	{
		if (walking != 0)
			walking = 3 - walking;
		else
			walking = 1;
		if(attackframe <= 0)
			mdirection = mpos;
	}
}

void Player::spriteNFrame()
{
	if (pcount > 0)
	{
		Point tempP;
		tempP.X = x;
		tempP.Y = y;
		illusion.push_back(tempP);
	}
	else
		illusion.clear();

	if (frame <= 1)
	{
		if (mpos != 5)
		{
			if (mpos <= 3)
				spriteY = 3;
			else if (mpos == 4)
				spriteY = 1;
			else if (mpos == 6)
				spriteY = 2;
			else
				spriteY = 0;
		}

		if (walking == 0)
			spriteX = 1;
		else if (walking == 1)
			spriteX = 0;
		else
			spriteX = 2;
		frame = 3;
	}
	else
		frame--;
}

void Player::action(Rect& rect, Graphics& g, Image*& playerAction, ImageAttributes*& imageAtt)
{

	if (lefthitframe > 0)
		lefthitframe--;

	rect.Width = width * 2;
	rect.Height = height * 2;

	for (int i = 0; i < illusion.size(); i++)
	{

		rect.X = illusion[i].X - width;
		rect.Y = illusion[i].Y - height;
		g.DrawImage(playerAction, rect, 80 * spriteX, 110 * spriteY, 80, 110, UnitPixel, imageAtt);
	}

	rect.X = x - width + 3;
	rect.Y = y - height;
	if(lefthitframe % 4 <= 1)
		g.DrawImage(playerAction, rect, 80 * spriteX, 110 * spriteY, 80, 110, UnitPixel);
}

void Player::correctPosition(RECT& rectView, vector<Block>& blocks, int GridXSize, int GridYSize)
{
	if (x + width > rectView.right)
		x = rectView.right - width;
	if (x - width < rectView.left)
		x = rectView.left + width;
	if (y - height < rectView.top)
		y = rectView.top + height;
	if (y + height > rectView.bottom)
		y = rectView.bottom - height;

	for (int i = 0; i < blocks.size(); i++)
	{
		int xlen = abs((blocks[i].getX() * GridXSize) - x);
		int ylen = abs((blocks[i].getY() * GridYSize) - y);
		if (xlen < GridXSize / 2 + width
			&& ylen < GridYSize / 2 + height)
		{
			POINT center = { blocks[i].getX() * GridXSize, blocks[i].getY() * GridYSize };
			POINT lefttop = { blocks[i].getX() * GridXSize - GridXSize / 2, blocks[i].getY() * GridYSize - GridYSize / 2 };
			POINT righttop = { blocks[i].getX() * GridXSize + GridXSize / 2, blocks[i].getY() * GridYSize - GridYSize / 2 };

			int crossleft = cross(center, lefttop, { (int)x, (int)y });
			int crossright = cross(center, righttop, { (int)x, (int)y });
			//양수 왼쪽 음수 오른쪽
			if (crossleft <= 0 && crossright <= 0)
				x = blocks[i].getX() * GridXSize - (GridXSize / 2 + width);
			else if(crossleft <= 0 && crossright >= 0)
				y = blocks[i].getY() * GridYSize + (GridYSize / 2 + height);
			else if(crossleft >= 0 && crossright >= 0)
				x = blocks[i].getX() * GridXSize + (GridXSize / 2 + width);
			else
				y = blocks[i].getY() * GridYSize - (GridYSize / 2 + height);
		}
	}

}

void Player::attackCollide(vector<Arrow>& arrows)
{
	if (GetKeyState(VK_LCONTROL) < 0)
	{
		if (attackframe <= 0 && leftAttackCoolDown <= 0)
		{
			attackframe = 4;
			leftAttackCoolDown = attackCoolDown;

			if (weapon == 2)
			{
				Arrow arrow;
				if (mdirection == 1)
					arrow.setAngle(45);
				else if (mdirection == 2)
					arrow.setAngle(0);
				else if (mdirection == 3)
					arrow.setAngle(315);
				else if (mdirection == 4)
					arrow.setAngle(90);
				else if (mdirection == 6)
					arrow.setAngle(270);
				else if (mdirection == 7)
					arrow.setAngle(135);
				else if (mdirection == 8)
					arrow.setAngle(180);
				else if (mdirection == 9)
					arrow.setAngle(225);
				arrow.setX(x);
				arrow.setY(y);
				arrow.setSpeed(20);
				arrow.setWidth(5);
				arrow.setHeight(30);
				arrow.setNewCollider();
				arrows.push_back(arrow);
			}
		}
	}
	if (attackframe > 0)
	{
		attackframe--;
		double line;
		double endangle;
		int angle = 0;
		if (mdirection == 1)
			angle = 45;
		else if (mdirection == 2)
			angle = 0;
		else if (mdirection == 3)
			angle = 315;
		else if (mdirection == 4)
			angle = 90;
		else if (mdirection == 6)
			angle = 270;
		else if (mdirection == 7)
			angle = 135;
		else if (mdirection == 8)
			angle = 180;
		else if (mdirection == 9)
			angle = 225;
		if (weapon == 1)
		{
			int xline = 40;
			int yline = 100;
			line = sqrt(xline * xline + yline * yline);
			endangle = acos(xline / line);

			attackPoints[0].x = x;
			attackPoints[0].y = y;
			attackPoints[1].x = x + xline * cos(angle * M_PI / 180);
			attackPoints[1].y = y - xline * sin(angle * M_PI / 180);
			attackPoints[2].x = x + line * cos(angle * M_PI / 180 + endangle);
			attackPoints[2].y = y - line * sin(angle * M_PI / 180 + endangle);
			attackPoints[3].x = x + yline * cos((angle + 90) * M_PI / 180);
			attackPoints[3].y = y - yline * sin((angle + 90) * M_PI / 180);
		}
		else if(weapon == 3)
		{
			int xline = 10;
			int yline = 140;
			line = sqrt(xline * xline + yline * yline);
			endangle = acos(xline / line);
			attackPoints[0].x = x - xline * cos(angle * M_PI / 180);
			attackPoints[0].y = y + xline * sin(angle * M_PI / 180);
			attackPoints[1].x = x + xline * cos(angle * M_PI / 180);
			attackPoints[1].y = y - xline * sin(angle * M_PI / 180);
			attackPoints[2].x = x + line * cos(angle * M_PI / 180 + endangle);
			attackPoints[2].y = y - line * sin(angle * M_PI / 180 + endangle);
			attackPoints[3].x = x + line * cos(angle * M_PI / 180 - endangle + M_PI);
			attackPoints[3].y = y - line * sin(angle * M_PI / 180 - endangle + M_PI);
		}
	}

	if (attackframe <= 0)
	{
		attackPoints[0].x = -100;
		attackPoints[0].y = -100;
		attackPoints[1].x = -100;
		attackPoints[1].y = -100;
		attackPoints[2].x = -100;
		attackPoints[2].y = -100;
		attackPoints[3].x = -100;
		attackPoints[3].y = -100;
	}


	if (leftAttackCoolDown > 0)
		leftAttackCoolDown--;
}

void Player::attack(Rect& rect, Graphics& g, Image*& attackAction, Image*& arrowAction,vector<Arrow>& arrows)
{
	if (attackframe > 0 && weapon != 2)
	{
		Matrix temp;
		if (mdirection == 1)
			temp.RotateAt(135, PointF(x, y));
		else if (mdirection == 2)
			temp.RotateAt(180, PointF(x, y));
		else if (mdirection == 3)
			temp.RotateAt(225, PointF(x, y));
		else if (mdirection == 4)
			temp.RotateAt(90, PointF(x, y));
		else if (mdirection == 6)
			temp.RotateAt(270, PointF(x, y));
		else if (mdirection == 7)
			temp.RotateAt(45, PointF(x, y));
		else if (mdirection == 8)
			temp.RotateAt(0, PointF(x, y));
		else if (mdirection == 9)
			temp.RotateAt(315, PointF(x, y));
		g.SetTransform(&temp);
		if (weapon == 1)
		{
			rect.Width = 150;
			rect.Height = 150;
			rect.X = x - rect.Width / 2 - 10;
			rect.Y = y - rect.Height / 2 + 30;
			g.DrawImage(attackAction, rect, 72 * (4 - attackframe), 0, 72, 72, UnitPixel);
		}
		else if(weapon == 3)
		{
			rect.Width = 100;
			rect.Height = 200;
			rect.X = x - rect.Width / 2;
			rect.Y = y - rect.Height / 2 + 30;
			g.DrawImage(attackAction, rect, 88 * (6 - attackframe), 0, 88, 88, UnitPixel);
		}
		g.ResetTransform();
	}

	for (int i = 0; i < arrows.size(); i++)
	{
		Matrix temp;
		temp.RotateAt(360 - arrows[i].getAngle(), PointF(arrows[i].getX(), arrows[i].getY()));
		g.SetTransform(&temp);


		rect.Width = arrows[i].getWidth() * 4;
		rect.Height = arrows[i].getHeight() * 2;
		rect.X = arrows[i].getX() - rect.Width / 2;
		rect.Y = arrows[i].getY() - rect.Height / 2;
		g.DrawImage(arrowAction, rect, 32, 32, 16, 16, UnitPixel);
		g.ResetTransform();
	}
}

void Player::useItem(Item& item, vector<AnimationEffect>& animationeffects)
{
	if (item.getType() == 0)
	{
		if (GetKeyState('4') < 0)
		{
			int count = item.getCount();
			if (count > 0)
			{
				if (life < maxlife)
				{
					count--;
					item.setCount(count);
					life += item.getNum();
					if (life > maxlife)
						life = maxlife;
					AnimationEffect animationeffect(x - width * 2, y - height * 2, width * 4, height * 4, 0, 6, 2);
					animationeffects.push_back(animationeffect);
				}
			}
		}
	}
}

int Player::HitCheck(Monster& monster, vector<AnimationEffect>& animationeffects)
{
	POINT playerCollider[4] = { {x - width, y - height}, {x - width, y + height},
		{x + width, y + height}, {x + width, y - height} };//플레이어 콜라이더
	POINT MonsterCollider[4] = { {monster.getX() - monster.getSizeX() / 2, monster.getY() - monster.getSizeY() / 2},
		{monster.getX() - monster.getSizeX() / 2, monster.getY() + monster.getSizeY() / 2},
		{monster.getX() + monster.getSizeX() / 2, monster.getY() + monster.getSizeY() / 2},
		{monster.getX() + monster.getSizeX() / 2, monster.getY() - monster.getSizeY() / 2} };//몬스터 콜라이더

	int check = 0;
	for (int i = 0; i < 4; i++)
	{
		int count = 0;
		int nexti;
		if (i == 3)
			nexti = 0;
		else
			nexti = i + 1;
		for (int j = 0; j < 4; j++)
		{
			int nextj;
			if (j == 3)
				nextj = 0;
			else
				nextj = j + 1;

			if (MonsterCollider[j].y != MonsterCollider[nextj].y)
			{
				POINT down, up;
				if (MonsterCollider[j].y > MonsterCollider[nextj].y)
				{
					down = MonsterCollider[j];
					up = MonsterCollider[nextj];
				}
				else
				{
					down = MonsterCollider[nextj];
					up = MonsterCollider[j];
				}

				int cross = down.x * up.y + up.x * playerCollider[i].y + playerCollider[i].x * down.y
					- (down.y * up.x + up.y * playerCollider[i].x + playerCollider[i].y * down.x);

				if (cross > 0 && playerCollider[i].y <= down.y && playerCollider[i].y >= up.y)
				{
					count++;
				}

			}
		}

		if (count % 2 == 1)
		{
			check = 1;
			break;
		}
	}

	if (check == 1 && lefthitframe <= 0)
	{
		lefthitframe = hitframe;
		AnimationEffect animationeffect(x - 30, y - 30, 60, 60, 0, 12, 1);
		animationeffects.push_back(animationeffect);
		life -= 5;
		return 5;
	}
	return 0;

}



int Player::ProjHitCheck(Monster &monster, vector<AnimationEffect>& animationeffects)
{
	vector<Projectile> projectiles = monster.getProjectiles();
	int check = 0;
	for (int i = 0; i < projectiles.size(); i++)
	{
		if (projectiles[i].getY() <= y + height && projectiles[i].getY() >= y - height)
		{
			if (abs(x - projectiles[i].getX()) <= projectiles[i].getRadius() + width)
			{
				cout << "투사체 삭제1" << endl;
				projectiles.erase(projectiles.begin() + i);
				check = 1;
				break;
			}
		}
		else if (projectiles[i].getX() <= x + width && projectiles[i].getX() >= x - width)
		{
			if (abs(y - projectiles[i].getY()) <= projectiles[i].getRadius() + height)
			{
				cout << "투사체 삭제2" << endl;
				projectiles.erase(projectiles.begin() + i);
				check = 1;
				break;
			}
		}
	}
	monster.setProjectile(projectiles);
	if (check == 1 && lefthitframe <= 0)
	{
		lefthitframe = hitframe;
		life -= 5;
		AnimationEffect animationeffect(x - 30, y - 30, 60, 60, 0, 12, 1);
		animationeffects.push_back(animationeffect);
		return 5;
	}
	return 0;

}

int Player::changeWeapon(UIIcon& WeaponIcon)
{
	if (GetKeyState('1') < 0)
	{
		weapon = 1;
		WeaponIcon.setX(10);
		return 1;
	}
	else if (GetKeyState('2') < 0)
	{
		weapon = 2;
		WeaponIcon.setX(10 + WeaponIcon.getWidth());
		return 1;
	}
	else if (GetKeyState('3') < 0)
	{
		weapon = 3;
		WeaponIcon.setX(10 + 2 * WeaponIcon.getWidth());
		return 1;
	}
	return 0;
}

int Player::cross(POINT a, POINT b, POINT c)
{
	return (a.x * b.y + b.x * c.y + c.x * a.y) - 
		(a.y * b.x + b.y * c.x + c.y * a.x);
}
void Arrow::setNewCollider()
{
	setCollider();
}

void Arrow::setCollider()
{
	double line = sqrt(width * width + height * height);
	double endangle = acos(width / line);
	Collider[0].x = x - width * cos(angle * M_PI / 180);
	Collider[0].y = y + width * sin(angle * M_PI / 180);
	Collider[1].x = x + width * cos(angle * M_PI / 180);
	Collider[1].y = y - width * sin(angle * M_PI / 180);
	Collider[2].x = x + line * cos(angle * M_PI / 180 + endangle);
	Collider[2].y = y - line * sin(angle * M_PI / 180 + endangle);
	Collider[3].x = x + line * cos(angle * M_PI / 180 - endangle + M_PI);
	Collider[3].y = y - line * sin(angle * M_PI / 180 - endangle + M_PI);
}

void Arrow::movePos()
{
	x += speed * sin((double)(360 - angle) / 180 * M_PI);
	y -= speed * cos((double)(360 - angle) / 180 * M_PI);
	setCollider();
}

Monster::Monster()
{
	x = 500;
	y = 100;
	sizex = 120;
	sizey = 120;
	maxlife = 1000;
	life = maxlife;

	actionframe = 30;
	leftactionframe = 0;
	pattern = 1;

	patterntime = time(NULL);

	patternprogress = 500;
	leftpatternprogress = 0;

	patternstart = 0;

	spriteX = 0;
	spriteY = 0;

	frame = 12;
	leftframe = 0;

	walkingframe = 1;
}

void Monster::action(Rect& rect, Graphics& g, Image*& bossAction)
{
	rect.X = x - sizex / 2;
	rect.Y = y - sizey / 2;
	rect.Width = sizex;
	rect.Height = sizey;
	walkingframe = 1 - walkingframe;

	if (groggy == 1)
	{
		frame--;
		if(spriteX != 3 && frame % 4 == 0)
			spriteX++;
	}
	else if (walking == 1)
	{
		if (walkingframe == 0)
		{
			if (spriteX == 0 || spriteX >= 3)
				spriteX = 1;
			else
				spriteX++;
		}
	}

	g.DrawImage(bossAction, rect, 144 * spriteX, 144 * spriteY, 144, 144, UnitPixel);
}

void Monster::normalMode(vector<POINT>& route, int GridXSize, int GridYSize, RECT &rectView, Player &player, const POINT grids, vector<Block>& blocks
, vector<AnimationEffect>& animationeffects)
{
	moveProjectilesPos();
	CheckProjectilesOutofAreaorTime(rectView);


	if (leftactionframe > 0)
		leftactionframe--;
	else
	{
		leftactionframe = actionframe;
		pattern = Randomize(1, 4);
		cout << pattern << endl;
	}

	if (route.size() == 0)
		pattern = 5;


	if (pattern <= 2)
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
	}
	else if (pattern == 3)
	{
		walking = 0;
		if (leftactionframe == actionframe)
		{
			double xlen = player.getX() - x;
			double ylen = player.getY() - y;
			double len = sqrt(xlen * xlen + ylen * ylen);
			double angle = acos(ylen / len) * 180 / M_PI;

			if (xlen < 0)
				angle = -angle;


			if (angle <= -60 && angle >= -120)
				spriteY = 1;
			else if (angle >= 60 && angle <= 120)
				spriteY = 2;
			else if (ylen > 0)
				spriteY = 0;
			else if (ylen < 0)
				spriteY = 3;

			for (int i = 0; i < 3; i++)
			{
				Projectile projectile;
				projectile.setX(x);
				projectile.setY(y);
				projectile.setSpeed(20);
				projectile.setRadius(20);
				projectile.setAngle(45 * i + 45 + angle);
				projectiles.push_back(projectile);
			}
		}
	}
	else if (pattern == 4)
	{
		walking = 0;
		int playerx = player.getX() / GridXSize;
		int playery = player.getY() / GridYSize;
		if (leftactionframe == actionframe)
		{
			int pointx = Randomize(1, grids.x - 2);
			int pointy = Randomize(1, grids.y - 2);
			
			int check = 0;
			for (int i = 0; i < blocks.size(); i++)
			{
				if ((pointx == blocks[i].getX() && pointy == blocks[i].getY()) ||
					(pointx == playerx && pointy == playery)
					|| (abs(pointx - x) <=  2 && abs(pointy - y) <= 2))
				{
					check = 1;
					break;
				}
			}

			if (check == 0)
			{
				Block block;
				block.setX(pointx);
				block.setY(pointy);
				blocks.push_back(block);
			}
		}
	}
	else if (pattern == 5)
	{
		if (leftactionframe == actionframe || leftactionframe == actionframe - 10
			|| leftactionframe == actionframe - 20)
		{
			for (int i = 0; i < blocks.size(); i++)
			{
				AnimationEffect animationeffect(blocks[i].getX() * GridXSize, blocks[i].getY() * GridYSize, GridXSize * 2, GridYSize * 2, 0, 9, 1);
				animationeffects.push_back(animationeffect);
			}


			blocks.clear();
			for (int i = 0; i < 8; i++)
			{
				Projectile projectile;
				projectile.setX(x);
				projectile.setY(y);
				projectile.setSpeed(20);
				projectile.setRadius(20);
				projectile.setAngle(45 * i + leftactionframe * 2);
				projectile.setType(0);
				projectiles.push_back(projectile);
			}
		}
	}
}

void Monster::patternMode(RECT& rectView, int GridXSize, int GridYSize, POINT Grids, Player& player, vector<AnimationEffect>& animationeffects)
{


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
	for (int i = 0; i < projectiles.size(); i++)
	{
		projectiles[i].movePos();
	}

	CheckProjectilesOutofAreaorTime(rectView);

	if (patternstart == 0)
	{
		leftpatternprogress = patternprogress;
		walking = 0;

	}


	if (leftpatternprogress >= patternprogress - 100)
	{
		if (leftpatternprogress % 3 == 0)
		{
			double xlen = player.getX() - x;
			double ylen = player.getY() - y;
			double len = sqrt(xlen * xlen + ylen * ylen);
			double angle = acos(ylen / len) * 180 / M_PI;

			if (xlen < 0)
				angle = -angle;
			else if (angle >= 60 && angle <= 120)
				spriteY = 2;
			else if (ylen > 0)
				spriteY = 0;
			else if (ylen < 0)
				spriteY = 3;

			Projectile projectile;
			projectile.setX(x);
			projectile.setY(y);
			projectile.setSpeed(20);
			projectile.setRadius(20);
			projectile.setAngle(90 + angle);
			projectiles.push_back(projectile);
		}
	}
	else if (leftpatternprogress >= patternprogress - 300)
	{
		if (leftpatternprogress % 2 == 0)
		{
			Projectile projectile;
			projectile.setSpeed(20);
			projectile.setRadius(35);
			int direction = Randomize(1, 4) * 2;
			int pointx = Randomize(1, GridXSize - 2);
			int pointy = Randomize(1, GridYSize - 2);
			if (direction == 8)
			{
				projectile.setX(pointx * GridXSize);
				projectile.setY(0);
				projectile.setAngle(90);
			}
			else if (direction == 2)
			{
				projectile.setX(pointx * GridXSize);
				projectile.setY(Grids.y * GridYSize);
				projectile.setAngle(270);
			}
			else if (direction == 4)
			{
				projectile.setX(Grids.x * GridXSize);
				projectile.setY(pointy * GridYSize);
				projectile.setAngle(0);
			}
			else
			{
				projectile.setX(0);
				projectile.setY(pointy * GridYSize);
				projectile.setAngle(180);
			}
			projectiles.push_back(projectile);
		}
	}
	else if(leftpatternprogress >= patternprogress - 400)
	{
		if (leftpatternprogress % 10 == 0)
		{
			DangerZone dangerzone(10, 0, 0);
			dangerzone.setX(player.getX());
			dangerzone.setY(player.getY());
			dangerzone.setRadius(150);
			dangerzone.activatezone();

			dangerzones.push_back(dangerzone);
		}

	}
	leftpatternprogress--;
}

int Monster::Randomize(int min, int max)
{
	random_device rd;
	mt19937_64 mt(rd());
	uniform_int_distribution<int> range(min, max);
	return range(mt);
}

void Monster::CheckProjectilesOutofAreaorTime(RECT& rectView)
{
	for (int i = 0; i < projectiles.size(); i++)
	{
		if (projectiles[i].getLeftProjectileFrame() > 0)
		{
			projectiles[i].setLeftProjectileFrame(projectiles[i].getLeftProjectileFrame() - 1);
		}
		if (projectiles[i].getX() < rectView.left ||
			projectiles[i].getX() > rectView.right ||
			projectiles[i].getY() < rectView.top ||
			projectiles[i].getY() > rectView.bottom
			|| projectiles[i].getLeftProjectileFrame() == 0)
		{
			cout << "투사체 삭제3" << endl;
			projectiles.erase(projectiles.begin() + i);
			i = -1;
		}
	}
}

void Monster::moveProjectilesPos()
{
	for (int i = 0; i < projectiles.size(); i++)
	{
		projectiles[i].movePos();
	}

}

int Monster::HitCheck(Player& player, vector<Arrow>& arrows, vector<AnimationEffect>& animationeffects)
{
	POINT* attackpoints = player.getAttackPoints();
	POINT monsterCollider[4] = { {x - sizex / 2, y - sizey / 2},
		{x - sizex / 2, y + sizey / 2},
		{x + sizex / 2, y + sizey / 2},
		{x + sizex / 2, y - sizey / 2} };//몬스터 콜라이더

	int count = 0;
	int hitcheck = 0;
	for (int i = 0; i < 4; i++)//플레이어 공격 또는 화살이 몬스터 콜라이더와 부딪치는지 확인
	{
		int nexti;
		count = 0;
		if (i == 3)
			nexti = 0;
		else
			nexti = i + 1;

		
		for (int j = 0; j < 4; j++)
		{
			int nextj;
			if (j == 3)
				nextj = 0;
			else
				nextj = j + 1;

			if (monsterCollider[j].y != monsterCollider[nextj].y)
			{
				POINT down;
				POINT up;

				if (monsterCollider[j].y > monsterCollider[nextj].y)
				{
					down = monsterCollider[j];
					up = monsterCollider[nextj];
				}
				else
				{
					down = monsterCollider[nextj];
					up = monsterCollider[j];
				}
				int cross = down.x * up.y + up.x * attackpoints[i].y + attackpoints[i].x * down.y
					- (down.y * up.x + up.y * attackpoints[i].x + attackpoints[i].y * down.x);
				if (attackpoints[i].y <= down.y && attackpoints[i].y >= up.y
					&& cross > 0)
					count++;
			}
		}

		if (count % 2 == 1)
		{
			hitcheck = 1;
			AnimationEffect animationeffect(x - 30, y - 30, 60, 60, 0, 12, 1);
			animationeffects.push_back(animationeffect);
			break;
		}
	}
	int arrowcheck = 0;
	for (int v = 0; v < arrows.size(); v++)
	{
		POINT* arrowCollider = arrows[v].getCollider();
		arrowcheck = 0;
		for (int i = 0; i < 4; i++)
		{
			int nexti;
			count = 0;
			if (i == 3)
				nexti = 0;
			else
				nexti = i + 1;


			for (int j = 0; j < 4; j++)
			{
				int nextj;
				if (j == 3)
					nextj = 0;
				else
					nextj = j + 1;

				if (monsterCollider[j].y != monsterCollider[nextj].y)
				{
					POINT down;
					POINT up;

					if (monsterCollider[j].y > monsterCollider[nextj].y)
					{
						down = monsterCollider[j];
						up = monsterCollider[nextj];
					}
					else
					{
						down = monsterCollider[nextj];
						up = monsterCollider[j];
					}
					int cross = down.x * up.y + up.x * arrowCollider[i].y + arrowCollider[i].x * down.y
						- (down.y * up.x + up.y * arrowCollider[i].x + arrowCollider[i].y * down.x);
					if (arrowCollider[i].y <= down.y && arrowCollider[i].y >= up.y
						&& cross > 0)
						count++;
				}
			}


			if (count % 2 == 1)
			{
				arrowcheck = 1;
				break;
			}
		}


		if (arrowcheck == 1)
		{
			arrows.erase(arrows.begin() + v);
			AnimationEffect animationeffect(x - 30, y - 30, 60, 60, 0, 12, 1);
			animationeffects.push_back(animationeffect);
			break;
		}
	}

	if ((hitcheck == 1 && player.getAttackFrame() >= 3) ||
		arrowcheck == 1)
	{
		int damage = 0;
		if (player.getWeaponType() == 1)
		{
			damage = 15;
		}
		else if (player.getWeaponType() == 2)
		{
			damage = 5;
		}
		else if (player.getWeaponType() == 3)
		{
			damage = 10;
		}

		if (groggy == 1)
			damage *= 2;
		life -= damage;
		return damage;
	}
	return 0;
}

void Monster::clearDangerZone()
{
	dangerzones.clear();
}

void Monster::groggyMode()
{
	spriteX = 0;
	spriteY = 4;
	groggy = 1;
	leftframe = frame;
}

void Monster::drawProjectiles(Graphics& g, Image** effect,Image*& projectile, Image*& projectile2)
{
	ColorMatrix colorMatrix = {
   2.0f, 0.0f, 0.0f, 0.0f, 0.0f,
   0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
   0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
   0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
   0.2f, 0.2f, 0.2f, 0.0f, 1.0f };

	ImageAttributes imageAttributes;
		imageAttributes.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

	for (int i = 0; i < projectiles.size(); i++)
	{
		Rect rect;
		rect.X = projectiles[i].getX() - projectiles[i].getRadius();
		rect.Y = projectiles[i].getY() - projectiles[i].getRadius();
		rect.Width = projectiles[i].getRadius() * 2;
		rect.Height = projectiles[i].getRadius() * 2;
		if (projectiles[i].getType() == 0)
		{
			g.DrawImage(projectile, rect, 0, 0, projectile->GetWidth(), projectile->GetHeight(), UnitPixel, &imageAttributes);
		}
		else if (projectiles[i].getType() == 1)
		{
		}
		else if (projectiles[i].getType() == 2)
		{
			if (projectiles[i].getSpriteX() == 3 || projectiles[i].getSpriteX() == 4)
				projectiles[i].setSpriteX(7 - projectiles[i].getSpriteX());
			else
				projectiles[i].setSpriteX(projectiles[i].getSpriteX() + 1);
			g.DrawImage(projectile2, rect, projectiles[i].getSpriteX() * 24, 0, 24, 32, UnitPixel);
		}
		else if (projectiles[i].getType() == 3)
		{
			g.DrawImage(effect[2], rect, 32 * (2 + spritechanger), 32, 32, 32, UnitPixel);
		}
	}
	spritechanger = 1 - spritechanger;
}

void Monster::drawDangerZones(Graphics& g, HDC& mem1dc)
{
	SolidBrush DangerZoneBrush(Color(50, 255, 0, 0));
	for (int i = 0; i < dangerzones.size(); i++)
	{
		Rect rect;
		rect.X = dangerzones[i].getX() - dangerzones[i].getRadius() / 2;
		rect.Y = dangerzones[i].getY() - dangerzones[i].getRadius() / 2;
		rect.Width = rect.Height = dangerzones[i].getRadius();

		g.FillEllipse(&DangerZoneBrush, rect);
	}
}

Projectile::Projectile()
{
	leftprojectileframe = -1;
	type = 0;
	spriteX = 0;
	spriteY = 0;
}

void Projectile::movePos()
{
	x = x - speed * cos(angle / 180 * M_PI);
	y = y + speed * sin(angle / 180 * M_PI);
}

AnimationEffect::AnimationEffect(double x, double y, int width, int height, int spriteX, int spriteY, int type)
{
	effectframe = 10;
	lefteffectframe = 0;
	this->x = x;
	this->y = y;
	this->spriteX = spriteX;
	this->spriteY = spriteY;
	this->type = type;
	this->width = width;
	this->height = height;
}

void AnimationEffect::drawAnimationEffect(Graphics& g, Image** effect)
{
	Rect rect;
	rect.X = x;
	rect.Y = y;
	rect.Width = width;
	rect.Height = height;
	if (type == 1)
		g.DrawImage(effect[0], rect, 64 * (spriteX / 2), 64 * spriteY, 64, 64, UnitPixel);
	else if (type == 2)
		g.DrawImage(effect[1], rect, 64 * (spriteX / 2), 64 * spriteY, 64, 64, UnitPixel);
	else if (type == 3)
		g.DrawImage(effect[2], rect, 32 * (6 + (spriteX / 3)), 32, 32, 32, UnitPixel);
	spriteX++;
}

void DangerZone::activatezone()
{
	leftframe = frame;
}

DangerZone::DangerZone(int frame, int zonetype, int variation)
{
	this->frame = frame;
	this->zonetype = zonetype;
	this->variation = variation;
}

int DangerZone::checkframe(vector<Projectile>& projectiles, vector<AnimationEffect>& animationeffects)
{
	leftframe--;
	if (leftframe <= 0)
	{
		if (zonetype == 0)
		{
			if (variation == 0)//stage1
			{
				Projectile projectile;
				projectile.setX(x);
				projectile.setY(y);
				projectile.setRadius(radius / 2);
				projectile.setSpeed(0);
				projectile.setLeftProjectileFrame(10);
				projectile.setType(1);

				projectiles.push_back(projectile);

				AnimationEffect animationeffect(x - radius / 2, y - radius / 2, radius, radius, 0, 9, 1);
				animationeffects.push_back(animationeffect);
			}
			else if (variation == 1)//stage2
			{
				Projectile projectile;
				projectile.setX(x);
				projectile.setY(y);
				projectile.setRadius(radius / 2);
				projectile.setSpeed(0);
				projectile.setLeftProjectileFrame(10);
				projectile.setType(1);
				projectiles.push_back(projectile);

				for (int i = 0; i < 6; i++)
				{
					Projectile projectile;
					projectile.setX(x);
					projectile.setY(y);
					projectile.setRadius(radius / 4);
					projectile.setSpeed(10);
					projectile.setType(3);
					projectile.setAngle(60 * i);
					projectiles.push_back(projectile);
				}

				AnimationEffect animationeffect(x - radius / 2, y - radius / 2, radius, radius, 0, 9, 3);
				animationeffects.push_back(animationeffect);
			}
		}
		else if (zonetype == 1)
		{
			Projectile projectile;
			projectile.setX(x);
			projectile.setY(y);
			projectile.setRadius(radius / 2);
			projectile.setSpeed(0);
			projectile.setLeftProjectileFrame(100);
			projectile.setType(2);

			projectiles.push_back(projectile);
		}
		return 1;
	}
	else
	{
		return 0;
	}
}

Block::Block()
{
	frame = 10;
	leftframe = frame;
}

void Block::drawBlock(Graphics& g, Image* BlockImage, int GridXSize, int GridYSize)
{
	Rect rect;
	double ratio = (1 - (double)leftframe / (double)frame);
	rect.Width = GridXSize;
	rect.Height = GridYSize * ratio;
	rect.X = x * GridXSize - GridXSize / 2;
	rect.Y = y * GridYSize - GridYSize / 2 + GridYSize * (1 - ratio);
	g.DrawImage(BlockImage, rect, 0, 0, 64, 64 * ratio, UnitPixel);
}

void Block::update()
{
	if(leftframe > 0)
		leftframe--;
}
