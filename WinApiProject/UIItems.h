#pragma once

#include <Windows.h>
#include <gdiplus.h>

using namespace Gdiplus;

class Button
{
private:
	int x;
	int y;
	int width;
	int height;
public:
};

class UIIcon
{
private:
	int x;
	int y;
	int width;
	int height;

	int sizex;
	int sizey;
	int posx;
	int posy;
public:
	int getX() { return x; }
	int getY() { return y; }
	int getWidth() { return width; }
	int getHeight() { return height; }
	int getSizeX() { return sizex; }
	int getSizeY() { return sizey; }
	int getPosX() { return posx; }
	int getPosY() { return posy; }

	void setX(int x) { this->x = x; }
	void setY(int y) { this->y = y; }
	void setWidth(int width) { this->width = width; }
	void setHeight(int height) { this->height = height; }
	void setSizeX(int sizex) { this->sizex = sizex; }
	void setSizeY(int sizey) { this->sizey = sizey; }
	void setPosX(int posx) { this->posx = posx; }
	void setPosY(int posy) { this->posy = posy; }

	void DrawUIIcon(Graphics& g, Image* UIImage);
	void DrawUIBackground(Graphics& g, Image* UIImage);
};