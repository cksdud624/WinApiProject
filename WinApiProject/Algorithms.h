#pragma once

#include <vector>
#include <Windows.h>
#include "Objects.h"

using namespace std;

class Grid
{
private:
	int G;
	int H;
	int F;
	int beforex;
	int beforey;

	int block;
	int open;
public:
	void setG(int G) { this->G = G; }
	void setH(int H) { this->H = H; }
	void setF(int F) { this->F = F; }
	void setBeforeX(int beforex) { this->beforex = beforex; }
	void setBeforeY(int beforey) { this->beforey = beforey; }
	void setOpen(int open) { this->open = open; }
	void setBlock(int block) { this->block = block; }

	int getG() { return G; }
	int getH() { return H; }
	int getF() { return F; }
	int getBeforeX() { return beforex; }
	int getBeforeY() { return beforey; }
	int getOpen() { return open; }
	int getBlock() { return block; }

	void Clear();
};

class Astar
{
	//25 * 25 사이즈의 격자에서 Astar알고리즘을 통해 플레이어를 쫒아온다.
private:
	Grid* grids;
	int gridx;
	int gridy;
public:
	void Init(int gridx, int gridy);

	vector<POINT> Route(int startx, int starty, int endx, int endy, int GridXSize, int GridYSize, vector<Block>& blocks);
};