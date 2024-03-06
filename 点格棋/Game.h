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
	void ShowMessage();			//���������Ϣ
	void GetMessageA();			//����������ȡ��Ϣ
	void Move();				//����	
	bool IsEdgeLegality(coor);	//�жϱ��Ƿ�Ϸ�
	//int score(coor);			//��������÷�
	bool GameRun();				//��ѭ��
	int GetBoxOwner(int, int);	//ռ��Box
	void Undo();				//���幦��
	bool Quit();				//�˳�����
	int GetWinner();			//����1������2��û��0
	void SaveBoard();
	void Recover();
};
