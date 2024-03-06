#pragma once
#include<vector>
#include<stack>
//#include"rule.h"
#include<iostream>
#include<fstream>
#include"player.h"
#include"ui.h"
#include"st.h"

class Game
{
	int num;	
	int nmsl;
	int turn;
	int isExitError;
	int P1, P2;
	coor p;	
	ui* UI;
	//rule rule;
	player* player1, * player2, * player;
	std::stack<undonode>step;

public:
	std::vector<std::vector<int>> board;

	Game(int = 1, int = 0, int = 5, bool = false);
	void ShowMessage();			//输出棋盘信息
	void GetMessageA();			//从玩家那里获取信息
	void Move();				//行棋	
	bool IsEdgeLegality(coor);	//判断边是否合法
	//int score(coor);			//行棋后计算得分
	bool GameRun();				//主循环
	int GetBoxOwner(int, int);	//占有Box
	void Undo();				//悔棋功能
	bool Quit();				//退出功能
	int GetWinner();			//先手1，后手2，没有0
	void SaveBoard();
	void Recover();
};
