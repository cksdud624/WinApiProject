#include "UIItems.h"
#include <Windows.h>
#include <gdiplus.h>

using namespace std;
using namespace Gdiplus;

void UIIcon::DrawUIIcon(Graphics& g, Image* UIImage)
{
	Rect rect;

	rect.X = x;
	rect.Y = y;
	rect.Width = width;
	rect.Height = height;
	g.DrawImage(UIImage, rect, posx, posy, sizex, sizey, UnitPixel);
}

void UIIcon::DrawUIBackground(Graphics& g, Image* UIImage)
{
	Rect rect;
	
	rect.X = x;
	rect.Y = y;
	rect.Width = width;
	rect.Height = height;

	g.DrawImage(UIImage, rect);
}