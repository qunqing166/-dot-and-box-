#include"Game.h"
#include "ui_start.h"
#include "Board.h"
#include"BoxBoard.h"
#include"MCTS.h"
#include<fstream>
#include<iostream>
//#include"rule.h"
//#include"ui.h"

int M[][11] = {
	  0,  1,  0, 10,  0,  1,  0, 10,  0, 10,  0,
	 10, 20,  1, 20, 10, 20, 10, 20,  1, 20,  1,
	  0,  1,  0,  1,  0,  1,  0,  1,  0, 10,  0,
	 10, 20,  1, 20, 10, 20, 10, 20, 10, 20,  1,
	  0, 10,  0, 10,  0,  1,  0,  1,  0,  1,  0,
	 10, 20,  1, 20,  1, 20, 10, 20, 10, 20, 10,
	  0,  1,  0, 10,  0, 10,  0,  1,  0,  1,  0,
	  1, 20, 10, 20,  1, 20,  1, 20, 10, 20,  1,
	  0,  1,  0,  1,  0, 10,  0, 10,  0, 10,  0,
	 10, 20, 10, 20, 10, 20,  1, 20, 10, 20,  1,
	  0,  1,  0, 10,  0, 10,  0,  1,  0,  1,  0
};

//int main()
//{
//	boxboard box;
//	box.BoxReSize(5);
//	for (int i = 0; i < box.n; i++)
//	{
//		for (int j = 0; j < box.n; j++)
//		{
//			box.board[i][j] = M[i][j];
//		}
//	}
//	ui UI(5);
//	UI.ShowBoard(box.board);
//	box.DefineBoxesType();
//	box.ShowLiberties();
//	box.DefineAllChains(1);
//	box.ShowBelongingChain();
//	box.ShowBoxType();
//
//	box.DefineDeadChain();
//	box.ShowBelongingChain();
//	box.ShowBoxType();
//	 
//	//for (int i = 0; i < box.n; i++)
//	//{
//	//	for (int j = 0; j < box.n; j++)
//	//	{
//	//		printf("%3d,", box.board[i][j]);
//	//	}
//	//	printf("\n");
//	//}
//	getchar();
//}

int test[][11] =
{
  0, 10,  0, 10,  0, 10,  0, 10,  0, 10,  0,
 10, 20, 10, 20, 10, 20, 10, 20, 10, 20, 10,
  0, 10,  0,  1,  0,  1,  0,  1,  0,  1,  0,
 10, 20, 10, 20,  1, 20, 10, 20,  1, 20, 10,
  0, 10,  0, 10,  0, 10,  0, 10,  0, 10,  0,
 10, 20, 10, 20,  1, 20,  1, 20,  1, 20, 10,
  0, 10,  0, 10,  0, 10,  0, 10,  0,  1,  0,
  1, 20, 10, 20, 10, 20, 10, 20,  1, 20, 10,
  0, 10,  0, 10,  0, 10,  0, 10,  0, 10,  0,
  1, 20, 10, 20, 10, 20,  1, 20,  1, 20,  1,
  0,  1,  0,  1,  0,  1,  0,  1,  0,  1,  0,
};

//int main()
//{
//	boxboard box;
//	box.BoxReSize(5);
//	for (int i = 0; i < box.n; i++)
//	{
//		for (int j = 0; j < box.n; j++)
//		{
//			box.board[i][j] = test[i][j];
//		}
//	}
//	GameTurnMove(box, 1);
//}

int main()
{
	//Game g(0, 0, 1, true);
	//while (1)
	//{
	//	g.GameRun();
	//}
	//std::fstream file;
	//file.open("boardSave\\boardSave.txt", std::ios::in);
	//char buf[2];
	//while(file >> buf)
	//{
	//	std::cout << buf << std::endl;
	//}
	//while (1);
	//printf("请棋盘输入规格\n");
	//int n=5;
	//std::cin >> n;
	//printf("请输入p1，p2模式（AI为0,非AI为1）");
	//int p1 = 1, p2 = 1;//1是人，0是机器
	int p1 = 1, p2 = 1;
	int n = 5;
	bool getRecord = false;
	//std::cin >> p1 >> p2;
	//Game ga(p1, p2, 6);
	
	//ui u;
	while (1)
	{
		ui_start* st = new ui_start(p1, p1, n);
		while (1)
		{
			int k = st->Run(p1, p2, n, getRecord);
			//printf("%d\n", k);
			if (k == 1)
			{
				delete st;
				break;
			}
			else if (k == 2)return 0;
		}
		Game ga(p1, p2, n, getRecord);
		Sleep(1000);
		while (1)
		{
		//u.get();
		if (ga.GameRun())break;
		}

	}
}