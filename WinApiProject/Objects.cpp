#define _USE_MATH_DEFINES

#include "Objects.h"
#include <iostream>
#include <gdiplus.h>
#include <vector>
#include <cmath>

using namespace std;

Player::Player()
{
	spriteX = 1;
	spriteY = 3;
	frame = 3;
	mdirection = 2;

	speed = 10;
	width = 20;
	height = 27;

	attackPoints = new POINT[4];
	attackCoolDown = 17;
	leftAttackCoolDown = 0;

	weapon = 1;
}

Player::~Player()
{
	delete[] attackPoints;
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
	rect.Width = width * 2;
	rect.Height = height * 2;

	for (int i = 0; i < illusion.size(); i++)
	{

		rect.X = illusion[i].X - width;
		rect.Y = illusion[i].Y - height;
		g.DrawImage(playerAction, rect, 80 * spriteX, 110 * spriteY, 80, 110, UnitPixel, imageAtt);
	}

	rect.X = x - width;
	rect.Y = y - height;
	g.DrawImage(playerAction, rect, 80 * spriteX, 110 * spriteY, 80, 110, UnitPixel);
}

void Player::correctPosition(RECT& rectView)
{
	if (x + width > rectView.right)
		x = rectView.right - width;
	if (x - width < rectView.left)
		x = rectView.left + width;
	if (y - height < rectView.top)
		y = rectView.top + height;
	if (y + height > rectView.bottom)
		y = rectView.bottom - height;
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
		g.DrawImage(arrowAction, rect,32, 32, 16, 16, UnitPixel);
		g.ResetTransform();
	}
}

int Player::changeWeapon(UIIcon& WeaponIcon)
{
	if (GetKeyState('1') < 0)
	{
		weapon = 1;
		WeaponIcon.setPosX(0);
		return 1;
	}
	else if (GetKeyState('2') < 0)
	{
		weapon = 2;
		WeaponIcon.setPosX(128);
		return 1;
	}
	else if (GetKeyState('3') < 0)
	{
		weapon = 3;
		WeaponIcon.setPosX(160);
		return 1;
	}
	return 0;
}

void Arrow::setNewCollider()
{
	Collider = new POINT[4];
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
