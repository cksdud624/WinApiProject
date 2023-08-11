#pragma once

#include <random>

int PlayerMove()
{
    int playermove = 5;
    if (GetKeyState(VK_LEFT) < 0 && GetKeyState(VK_UP) < 0)
        playermove = 1;
    else if (GetKeyState(VK_RIGHT) < 0 && GetKeyState(VK_UP) < 0)
        playermove = 3;
    else if (GetKeyState(VK_LEFT) < 0 && GetKeyState(VK_DOWN) < 0)
        playermove = 7;
    else if (GetKeyState(VK_RIGHT) < 0 && GetKeyState(VK_DOWN) < 0)
        playermove = 9;
    else if (GetKeyState(VK_UP) < 0)
        playermove = 2;
    else if (GetKeyState(VK_LEFT) < 0)
        playermove = 4;
    else if (GetKeyState(VK_RIGHT) < 0)
        playermove = 6;
    else if (GetKeyState(VK_DOWN) < 0)
        playermove = 8;

    return playermove;
}


void CheckArrowOutofArea(vector<Arrow>& arrows, RECT& rectView)
{
    for (int i = 0; i < arrows.size(); i++)
    {
        if (arrows[i].getX() > rectView.right
            || arrows[i].getX() < rectView.left
            || arrows[i].getY() > rectView.bottom
            || arrows[i].getY() < rectView.top)
        {
            arrows.erase(arrows.begin() + i);
            i = -1;
        }
    }
}