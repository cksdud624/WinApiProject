#pragma once
#include "Objects.h"
class MonsterType2 : public Monster
{
	//��� ����
	void normalMode(vector<POINT>& route, int GridXSize, int GridYSize, RECT& rectView, Player& player, const POINT grids, vector<Block>& blocks
		, vector<AnimationEffect>& animationeffects);
	//���� ����
	void patternMode(RECT& rectView, int GridXSize, int GridYSize, POINT Grids, Player& player, vector<AnimationEffect>& animationeffects);
	//�׷α� ����
	void groggyMode();
};