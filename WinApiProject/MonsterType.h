#pragma once
#include "Objects.h"
#include "Algorithms.h"
class MonsterType2 : public Monster
{
private:
	int tempprogress;
public:
	//��� ����
	void normalMode(vector<POINT>& route, int GridXSize, int GridYSize, RECT& rectView, Player& player, const POINT grids, vector<Block>& blocks
		, vector<AnimationEffect>& animationeffects);
	//���� ����
	int patternMode(RECT& rectView, int GridXSize, int GridYSize, POINT Grids, Player& player, vector<AnimationEffect>& animationeffects, int monsters);
	//�׷α� ����


};