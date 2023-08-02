#include "Objects.h"
#include <cmath>
#include <iostream>

using namespace std;
void Player::movePos(int pos)
{
	float move = 1;

	if (GetKeyState(VK_SHIFT) < 0)
		move = move * 2;
	switch (pos)
	{
	case 1:
		(*this).x -= move * 0.6 * (*this).speed;
		(*this).y -= move * 0.6 * (*this).speed;
		break;
	case 2:
		(*this).y -= move * (*this).speed;
		break;
	case 3:
		(*this).x += move * 0.6 * (*this).speed;
		(*this).y -= move * 0.6 * (*this).speed;
		break;
	case 4:
		(*this).x -= move * (*this).speed;
		break;
	case 5:
		break;
	case 6:
		(*this).x += move * (*this).speed;
		break;
	case 7:
		(*this).x -= move * 0.6 * (*this).speed;
		(*this).y += move * 0.6 * (*this).speed;
		break;
	case 8:
		(*this).y += move * (*this).speed;
		break;
	case 9:
		(*this).x += move * 0.6 * (*this).speed;
		(*this).y += move * 0.6 * (*this).speed;
		break;
	default:
		break;
	}
}
