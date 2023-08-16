#include "Algorithms.h"
#include <cmath>
#include <iostream>
using namespace std;

void Astar::Init(int gridx, int gridy)
{
	grids = new Grid[gridx * gridy];//625개의 격자 생성
	//인덱스값이 그대로 좌표값을 가짐

	this->gridx = gridx;
	this->gridy = gridy;
}

vector<POINT> Astar::Route(int startx, int starty, int endx, int endy, int GridXSize, int GridYSize, vector<POINT>& blocks)
{
	vector<POINT> route;
	vector<POINT> openpoints;
	int indexstartx = startx / GridXSize;//시작점 x좌표
	int indexstarty = starty / GridYSize;//시작점 y좌표
	int indexendx = endx / GridXSize;//도착점 x좌표
	int indexendy = endy / GridYSize;//도작첨 y좌표



	for (int i = 0; i < gridx; i++)
	{
		for (int j = 0; j < gridy; j++)
		{
			grids[i * gridx + j].Clear();//원래 있던 값을 초기화
		}
	}
	for (int i = 0; i < blocks.size(); i++)
	{
		for (int a = -1; a <= 1; a++)
		{
			for (int b = -1; b <= 1; b++)
			{
				grids[(blocks[i].x + a) * gridx + (blocks[i].y + b)].setBlock(1);
			}
		}
	}
	int searchrange = 1;
	while (grids[indexendx * gridx + indexendy].getBlock() == 1)
	{
		int check = 0;
		for (int a = -searchrange; a <= searchrange; a++)
		{
			for (int b = -searchrange; b <= searchrange; b++)
			{
				if (indexendx + a < 0 || indexendy + b < 0 || indexendx + a >= gridx 
					||indexendy + b >= gridy)
					continue;
				if (grids[(indexendx + a) * gridx + (indexendy + b)].getBlock() == 0)
				{
					indexendx += a;
					indexendy += b;
					check = 1;
					break;
				}
			}

			if (check == 1)
				break;
		}

		if (check == 0)
			searchrange++;
	}
	POINT move = { indexstartx, indexstarty };
	int currentG = 0;
	while (!(move.x == indexendx && move.y == indexendy))//경로 탐색
	{
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				int index = (move.x + i) * gridx + (move.y + j);

				if (i == 0 && j == 0)
					continue;
				if (move.x + i < 0 || move.x + i >= gridx ||
					move.y + j < 0 || move.y + j >= gridy)
					continue;
				if (grids[index].getBlock() != 0)
					continue;
				if (grids[index].getOpen() != 2)
				{
					int tempG = currentG;
					int tempH = 0;
					if (i == 0 || j == 0)
						tempG += 10;
					else
						tempG += 14;

					if (grids[index].getOpen() == 0)
					{
						grids[index].setG(tempG);
						grids[index].setBeforeX(move.x);
						grids[index].setBeforeY(move.y);
					}
					else if (grids[index].getG() > tempG)
					{
						grids[index].setG(tempG);
						grids[index].setBeforeX(move.x);
						grids[index].setBeforeY(move.y);
					}

					int x = abs(move.x + i - indexendx);
					int y = abs(move.y + j - indexendy);

					if (x > y)
					{
						tempH += y * 14;
						x -= y;
						tempH += x * 10;
					}
					else
					{
						tempH += x * 14;
						y -= x;
						tempH += y * 10;
					}

					grids[index].setH(tempH);
					grids[index].setF(grids[index].getG() + tempH);

					if (grids[index].getOpen() == 0)
					{
						openpoints.push_back({ move.x + i, move.y + j });
						grids[index].setOpen(1);
					}
				}

			}
		}
		//for문끝
		int minF = grids[openpoints[0].x * gridx + openpoints[0].y].getF();//openpoints가 0인 경유가 있음
		for (int i = 0; i < openpoints.size(); i++)
		{
			if (minF > grids[openpoints[i].x * gridx + openpoints[i].y].getF())
			{
				minF = grids[openpoints[i].x * gridx + openpoints[i].y].getF();
			}
		}

		vector<POINT> minFpoints;

		for (int i = 0; i < openpoints.size(); i++)
		{
			if (minF == grids[openpoints[i].x * gridx + openpoints[i].y].getF())
			{
				minFpoints.push_back({openpoints[i].x, openpoints[i].y});
			}
		}
		int minH = grids[minFpoints[0].x * gridx + minFpoints[0].y].getH();
		for (int i = 1; i < minFpoints.size(); i++)
		{
			if (minH > grids[minFpoints[i].x * gridx + minFpoints[i].y].getH())
			{
				minH = grids[minFpoints[i].x * gridx + minFpoints[i].y].getH();
			}
		}

		POINT nextPoint;

		for (int i = 0; i < minFpoints.size(); i++)
		{
			if (minH == grids[minFpoints[i].x * gridx + minFpoints[i].y].getH())
			{
				nextPoint = { minFpoints[i].x , minFpoints[i].y };
				break;
			}
		}

		grids[move.x * gridx + move.y].setOpen(2);
		move.x = nextPoint.x;
		move.y = nextPoint.y;

		for (int i = 0; i < openpoints.size(); i++)
		{
			if (move.x == openpoints[i].x && move.y == openpoints[i].y)
			{
				openpoints.erase(openpoints.begin() + i);
			}
		}
		currentG = grids[move.x * gridx + move.y].getG();
	}
	POINT returnPoint = move;

	route.push_back({ indexendx, indexendy });

	while (!(returnPoint.x == indexstartx && returnPoint.y == indexstarty))
	{
		POINT temp = returnPoint;
		returnPoint.x = grids[temp.x * gridx + temp.y].getBeforeX();
		returnPoint.y = grids[temp.x * gridx + temp.y].getBeforeY();
		route.push_back(temp);
	}


	reverse(route.begin(), route.end());
	return route;
}


void Grid::Clear()
{
	G = 0; H = 0; F = 0; beforex = 0; beforey = 0; open = 0; block = 0;
}
