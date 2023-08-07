#include "Objects.h"
#include <iostream>
#include <gdiplus.h>
#include <vector>

using namespace std;

Player::Player()
{
	spriteX = 1;
	spriteY = 3;
	frame = 3;
	mdirection = 2;

	attackPoints = new POINT[4];
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

void Player::attackCollide()
{
	if (GetKeyState(VK_LCONTROL) < 0 && swordframe <= 0)
		swordframe = 4;
	if (swordframe > 0)
	{
		swordframe--;
		double line = sqrt(1600 + 10000);

		double endangle = acos(40 / line);//°¢µµ
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
		attackPoints[0].x = x;
		attackPoints[0].y = y;
		attackPoints[1].x = x + 40 * cos(angle * 3.14 / 180);
		attackPoints[1].y = y - 40 * sin(angle * 3.14 / 180);
		attackPoints[2].x = x + line * cos(angle * 3.14 / 180 + endangle);
		attackPoints[2].y = y - line * sin(angle * 3.14 / 180 + endangle);
		attackPoints[3].x = x + 100 * cos((angle + 90) * 3.14 / 180);
		attackPoints[3].y = y - 100 * sin((angle + 90) * 3.14 / 180);
	}

}

void Player::attack(Rect& rect, Graphics& g, Image*& swordAction)
{
	if (swordframe > 0)
	{
		Matrix temp;
		rect.Width = 150;
		rect.Height = 150;
		rect.X = x - rect.Width / 2 - 10;
		rect.Y = y - rect.Height / 2 + 30;
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
		g.DrawImage(swordAction, rect, 72 * (4 - swordframe), 0, 72, 72, UnitPixel);
	}
}
