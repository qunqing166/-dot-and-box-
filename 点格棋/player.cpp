#include"player.h"

//std::pair<int, int> yuchunderen::act(std::vector<std::vector<int>>v)
//{
//	std::pair<int, int>p;
//	//p.first = n.x;
//	//p.second = n.y;
//	std::cin >> p.first >> p.second;
//	return p;
//}

void yuchunderen::get(std::vector<std::vector<int>>v, ui*& p1, int color)
{
	//map = v;
	p = p1->GetCoor();
}

coor yuchunderen::act()
{
	return p;
}

player::player()
{
	score = 0;
}

AI::AI():board()
{
	step = 0;
}

void AI::get(std::vector<std::vector<int>>B, ui*& p, int co)
{
	color = co;
	BoardChange(B);
}

void AI::BoardChange(std::vector<std::vector<int>>b)
{
	int len = b.size();
	auto& v = board.board;
	v.resize(len);
	for (int i = 0; i < len; i++)
	{
		v[i].resize(len);
		for (int j = 0; j < len; j++)
		{
			if (i % 2 || j % 2)
			{
				if ((i + j) % 2)	//Edge
				{
					if (b[i][j] == 0)v[i][j] = 10;
					else v[i][j] = 1;
				}
				else                //Box
				{
					if (b[i][j] == 0)v[i][j] = 20;
					else v[i][j] = b[i][j] * 2 * color;
				}
			}
		}
	}
	//board.board = v;
	board.boardSize = board.board.size();
	board.ShowBoard();
}

void AI::BoardChange_1(std::vector<std::vector<int>> b)
{
	int len = b.size();
	auto& v = b;
	v.resize(len);
	for (int i = 0; i < len; i++)
	{
		v[i].resize(len);
		for (int j = 0; j < len; j++)
		{
			if (i % 2 || j % 2)
			{
				if ((i + j) % 2 == 0)//Box
				{
					if (b[i][j] == 20)v[i][j] = 0;
					else if (b[i][j] == 2)v[i][j] = 1;
					else if (b[i][j] == -2)v[i][j] = -1;
				}
				else
				{
					if (b[i][j] == 10)v[i][j] = 0;
					else
					{
						v[i][j] = 1;
					}
				}
			}
		}
	}
	board.board = v;
	board.boardSize = board.board.size();
}

//int AI::pd_1(int x, int y)	//返回自由度
//{
//	int n = 0;
//	if (v[x + 1][y] > 0)n++;
//	if (v[x - 1][y] > 0)n++;
//	if (v[x][y + 1] > 0)n++;
//	if (v[x][y - 1] > 0)n++;
//	return n;
//}

//int AI::pd_2(int x, int y)
//{
//	int k1 = 0, k2 = 0;
//	if (x % 2)
//	{
//		if (y + 1 < v.size())k1 = pd_1(x, y + 1);
//		if (y - 1 >= 0)k2 = pd_1(x, y - 1);
//	}
//	else
//	{
//		if (x + 1 < v.size())k1 = pd_1(x + 1, y);
//		if (x - 1 >= 0)k2 = pd_1(x - 1, y);
//	}
//	if (k1 == 3 || k2 == 3)return 3;
//	else if (k1 == 2 || k2 == 2)return 0;
//	else return 2;
//
//}
//
//coor AI::pd_3()
//{
//	std::vector<coor>xy;
//	for (int i = 0; i < v.size(); i++)
//	{
//		int j = (i + 1) % 2;
//		for (; j < v.size(); j += 2)
//		{
//			if (v[i][j])continue;
//			coor p = { i,j };
//			int k = pd_2(i, j);
//			if (k == 3) {
//				return p;
//			}
//			else if (k != 0)
//			{
//				xy.push_back(p);
//			}
//		}
//	}
//	if (xy.size() > 0)
//	{
//		srand((unsigned)time(NULL));
//		int n = rand() % (xy.size());
//		return xy[n];
//	}
//	else
//	{
//		return bl();
//	}
//}

//coor AI::bl()
//{
//	//for (int i = 0; i < v.size(); i++)
//	//{
//	//	int j = (i + 1) % 2;
//	//	for (; j < v.size(); j += 2)
//	//	{
//
//	//	}
//	//}
//	while (1)
//	{
//		srand((unsigned)time(NULL));
//		coor p = { -1,-1 };
//		int n = v.size();
//		if (n != 0)
//		{
//			p.x = rand() % n;
//			p.y = rand() % n;
//		}
//		if ((p.x + p.y) % 2) {
//			if (v[p.x][p.y] == 0) {
//				//printf("%d %d\n ", p.first, p.second);
//				return p;
//			}
//		}
//	}
//}

coor AI::act()
{
	if (board.qMove.empty())
	{
		GameTurnMove(board, 1);
	}
	coor pl = board.qMove.front();
	board.qMove.pop();
	Sleep(500);
	return pl;
}

//int AI::pd_step()
//{
//	for (int i = 0; i < v.size(); i ++)
//	{
//		int j = (i + 1) % 2;
//		for (; j < v.size(); j += 2)
//		{
//			if (pd_2(i, j))return 0;
//		}
//	}
//	return 1;
//}
//
//void AI::GetMoves()
//{
//	//qMove.push(pd_3());
//}
