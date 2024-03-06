#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<random>
#include"st.h"
#include"ui.h"
#include"BoxBoard.h"

class player
{
public:
	int score;
	virtual void get(std::vector<std::vector<int>>, ui*&, int color) = 0;
	virtual coor act() = 0;
	player();
};

class yuchunderen :public player
{
	coor p;
public:
	void get(std::vector<std::vector<int>>, ui*&, int color);
	coor act();
};

class AI :public player
{
	int step;
	//std::vector<std::vector<int>>v;
	Board board;
	int color;

public:
	AI();
	void get(std::vector<std::vector<int>>B, ui*&,int pl);
	void BoardChange(std::vector<std::vector<int>>);
	void BoardChange_1(std::vector<std::vector<int>>);
	//int pd_1(int x, int y);
	//int pd_2(int x, int y);
	//int pd_step();
	//void GetMoves();
	//coor pd_3();
	//coor bl();
	coor act();
};
