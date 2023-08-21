#include "UIItems.h"
#include <Windows.h>
#include <gdiplus.h>
#include <tchar.h>

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

void Button::drawRectangleButton(Graphics& g, Image*& ButtonImage, const wchar_t* text)
{
	Rect rect;
	rect.Width = width;
	rect.Height = height;
	rect.X = x - width / 2;
	rect.Y = y - height / 2;
	g.DrawImage(ButtonImage, rect);
	StringFormat stringFormat;
	stringFormat.SetAlignment(StringAlignmentCenter);
	FontFamily fontFamily(L"µ¸¿ò");
	Font font(&fontFamily, 30, FontStyleBold, UnitPixel);
	PointF pointF(x, y - height / 4);
	SolidBrush solidBrush(Color(255, 0, 0, 0));
	g.DrawString(text, -1, &font, pointF, &stringFormat, &solidBrush);
}

void Button::drawCircleButton(Graphics& g, Image*& ButtonImage)
{
	SolidBrush blackBrush(Color(255, 0, 0, 0));
	Rect rect;
	rect.X = x - radius;
	rect.Y = y - radius;
	rect.Width = radius * 2;
	rect.Height = radius * 2;
	g.DrawImage(ButtonImage, rect);
}

int Button::OnClickRectangle(int mouseposX, int mouseposY)
{
	if (mouseposX <= x + width / 2 && mouseposX >= x - width / 2
		&& mouseposY <= y + height / 2 && mouseposY >= y - height / 2)
		return 1;
	return 0;
}

int Button::OnClickCircle(int mouseposX, int mouseposY)
{
	if ((mouseposX - x) * (mouseposX - x) + (mouseposY - y) * (mouseposY - y) <= radius * radius)
		return 1;
	return 0;
}
