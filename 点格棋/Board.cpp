#include "Board.h"
#include<iostream>
using namespace std;

//设置偏移量
const int dx[] = { 0, 1, 0, -1 };
const int dy[] = { 1, 0, -1, 0 };

TURN::TURN(int num)
{
	boardSize = num;
	for (int i = 0; i < boardSize; i++)
	{
		for (int j = 0; j < boardSize; i++)
		{
			if (i % 2)
			{
				if (j % 2)
				{
					newBoard[i][j] = BOX;
				}
				else
				{
					newBoard[i][j] = EDGE;
				}
			}
			else
			{
				if (j % 2)
				{
					newBoard[i][j] = EDGE;
				}
				else
				{
					newBoard[i][j] = EMPTY;
				}
			}
		}
	}
}

void TURN::SetBoard(std::vector<std::vector<int>>b)
{
	newBoard = b;
}

//Board::Board()
//{
//	n = 5 * 2 + 1;
//	step = 0;
//	board.resize(n);
//	for (int i = 0; i < n; i++)
//	{
//		board[i].resize(n);
//	}
//	for (int i = 0; i < n; i++)
//	{
//		for (int j = 0; j < n; j++)
//		{
//			if (i % 2)
//			{
//				if (j % 2)
//				{
//					board[i][j] = BOX;
//				}
//				else
//				{
//					board[i][j] = EDGE;
//				}
//			}
//			else
//			{
//				if (j % 2)
//				{
//					board[i][j] = EDGE;
//				}
//				else
//				{
//					board[i][j] = EMPTY;
//				}
//			}
//		}
//	}
//}

//Board::Board(int num)
//{
//	n = num * 2 + 1;
//	step = 0;
//	board.resize(n);
//	for (int i = 0; i < n; i++)
//	{
//		board[i].resize(n);
//	}
//	for (int i = 0; i < n; i++)
//	{
//		for (int j = 0; j < n; j++)
//		{
//			if (i % 2)
//			{
//				if (j % 2)
//				{
//					board[i][j] = BOX;
//				}
//				else
//				{
//					board[i][j] = EDGE;
//				}
//			}
//			else
//			{
//				if (j % 2)
//				{
//					board[i][j] = EDGE;
//				}
//				else
//				{
//					board[i][j] = EMPTY;
//				}
//			}
//		}
//	}
//}

Board::Board()
{
}

Board::Board(std::vector<std::vector<int>>b, int step1)
{
	board = b;
	step = step1;
	boardSize = board.size();
}

//重置棋盘，参数为Box大小
void Board::BoardReSize(int num)
{
	boardSize = num * 2 + 1;
	step = 0;
	board.resize(boardSize);
	for (int i = 0; i < boardSize; i++)
	{
		board[i].resize(boardSize);
	}
	for (int i = 0; i < boardSize; i++)
	{
		for (int j = 0; j < boardSize; j++)
		{
			if (i % 2)
			{
				if (j % 2)
				{
					board[i][j] = BOX;
				}
				else
				{
					board[i][j] = EDGE;
				}
			}
			else
			{
				if (j % 2)
				{
					board[i][j] = EDGE;
				}
				else
				{
					board[i][j] = EMPTY;
				}
			}
		}
	}
}

void Board::ShowLiberties()
{
	cout << "Box自由度:" << '\n';
	for (int i = 1; i < boardSize; i += 2)
	{
		for (int j = 0; j < boardSize; j++)
		{
			if (i % 2 == 1 && j % 2 == 1)
			{
				cout << GetBoxLiberties(i, j) << " ";
			}
		}
		cout << '\n';
	}
	cout << '\n';
}

void Board::ShowBoard()
{
	fstream saveBoard;
	saveBoard.open("boardSave\\AI\\boardSave.txt", ios::app);
	for (int i = 0; i < boardSize; i++)
	{
		for (int j = 0; j < boardSize; j++)
		{
			saveBoard << board[i][j] << " ";
			//printf("%3d,", board[i][j]);
		}
		//cout << '\n';
		saveBoard << endl;
	}
	//cout << "\n";
	saveBoard << endl;
	saveBoard.close();
}

int Board::GetEmptyEdges()
{
	int emptyEdgeNum = 0;
	for (int i = 0; i < boardSize; i++)
	{
		for (int j = 0; j < boardSize; j++)
		{
			if (board[i][j] == EDGE)emptyEdgeNum++;
		}
	}
	return emptyEdgeNum;
}

int Board::GetEmptyBoxes()
{
	int emptyBoxNum = 0;
	for (int i = 0; i < boardSize; i++)
	{
		for (int j = 0; j < boardSize; j++)
		{
			if (board[i][j] == EDGE)emptyBoxNum++;
		}
	}
	return emptyBoxNum;
}

int Board::GetEdgeOwner(int x, int y)
{
	int num = board[x][y];
	if (num != EDGE && num != RED_EDGE && num != BLUE_EDGE)
	{
		cout << "GetEdgeOwnerPlayer函数输入了一个错误的地址";
		return 0;
	}
	if (num > 0)
	{
		return RED;
		//cout<<x<<" "<<y<<" EdgeOwner = RED"<<endl;
	}
	if (num < 0)
	{
		return BLUE;
		//cout<<x<<" "<<y<<" EdgeOwner = BLUE"<<endl;
	}
	return EMPTY;
}

int Board::GetBoxOwner(int x, int y)
{
	int num = board[x][y];
	if (num != BOX && num != RED_BOX && num != BLUE_BOX)
	{
		cout << "GetEdgeOwnerPlayer函数输入了一个错误的地址";
		return 0;
	}
	if (num > 0)
	{
		return RED;
		//cout<<x<<" "<<y<<" BoxOwner = RED"<<endl;
	}
	if (num < 0)
	{
		return BLUE;
		//cout<<x<<" "<<y<<" BoxOwner = BLUE"<<endl;
	}
	return EMPTY;
}

int Board::GetBoxLiberties(int x, int y)
{
	int libertyNum = 0;
	if (board[x + 1][y] == EDGE)libertyNum++;
	if (board[x - 1][y] == EDGE)libertyNum++;
	if (board[x][y + 1] == EDGE)libertyNum++;
	if (board[x][y - 1] == EDGE)libertyNum++;
	return libertyNum;
}

int Board::GetPlayerBoxes(int player)
{
	int boxNum = 0;
	if (player == RED)
	{
		for (int i = 1; i < boardSize; i += 2)
		{
			for (int j = 1; j < boardSize; j += 2)
			{
				if (board[i][j] == RED_BOX)boxNum++;
			}
		}
		return boxNum;
	}
	else if (player == BLUE)
	{
		for (int i = 1; i < boardSize; i += 2)
		{
			for (int j = 1; j < boardSize; j += 2)
			{
				if (board[i][j] == BLUE_BOX)boxNum++;
			}
		}
		return boxNum;
	}
	return 0;
}

int Board::GetPlayerEdges(int player)
{
	int edgeNum = 0;
	if (player == RED)
	{
		for (int i = 0; i < boardSize; i++)
		{
			for (int j = 0; j < boardSize; j++)
			{
				if (board[i][j] == RED_EDGE)edgeNum++;
			}
		}
		return edgeNum;
	}
	else if (player == BLUE)
	{
		for (int i = 0; i < boardSize; i++)
		{
			for (int j = 0; j < boardSize; j++)
			{
				if (board[i][j] == BLUE_EDGE)edgeNum++;
			}
		}
		return edgeNum;
	}
	return 0;
}

//直接从board判断该坐标是否是未被占领的Edge
bool Board::IsEdgeLegality(int x, int y)
{
	if (x < 0 || y < 0 || x >= boardSize || y >= boardSize)
	{
		//cout << "Edge超限" << '\n';
		return false;
	}
	if (board[x][y] == EDGE)return true;
	//else if (board[x][y] == RED_EDGE || board[x][y] == BLUE_EDGE)
	//{
	//	cout << "该Edge已经被占据" << '\n';
	//}
	//else
	//{
	//	cout << "不是Edge" << '\n';
	//}
	return false;
}

int Board::Winner()
{
	int redBoxNum = GetPlayerBoxes(RED);
	int blueBoxNum = GetPlayerBoxes(BLUE);
	if (redBoxNum + blueBoxNum >= (boardSize / 2) * (boardSize / 2))
	{
		if (redBoxNum > blueBoxNum)
		{
			return RED;
		}
		else
		{
			return BLUE;
		}
	}
	if (redBoxNum > (boardSize / 2) * (boardSize / 2) / 2)
	{
		return RED;
	}
	else if (blueBoxNum > (boardSize / 2) * (boardSize / 2) / 2)
	{
		return BLUE;
	}
	return 0;
}

void Board::SetBoard(std::vector<std::vector<int>>b)
{
	board = b;
}

bool Board::IsFreeBox(int x, int y)
{
	if (x % 2 && y % 2)
	{
		if (GetBoxLiberties(x, y) >= 3)
		{
			return true;
		}
		else return false;
	}
	//cout << "不是Box" << '\n';
	return false;
}

//可行边
bool Board::IsFreeEdge(int x, int y)
{
	int boxLiberties1 = 4;
	int boxLiberties2 = 4;
	if (x % 2 == 0)
	{
		if (x - 1 >= 0)boxLiberties1 = GetBoxLiberties(x - 1, y);
		if (x + 1 < boardSize)boxLiberties2 = GetBoxLiberties(x + 1, y);
	}
	else
	{
		if (y - 1 >= 0)boxLiberties1 = GetBoxLiberties(x, y - 1);
		if (y + 1 < boardSize)boxLiberties2 = GetBoxLiberties(x, y + 1);
	}
	if (boxLiberties1 > 2 && boxLiberties2 > 2)return true;
	
	return false;
}

//获得自由格数量
int Board::GetFreeEdgeNum()
{
	//moves.resize(0);
	int moveNum = 0;
	for (int i = 0; i < boardSize; i++)
	{
		int j = (i + 1) % 2;
		for (; j < boardSize; j += 2)
		{
			if (IsEdgeLegality(i, j))
			{
				if (IsFreeEdge(i, j))
				{
					moveNum++;
					//moves.push_back(coor(i, j));
				}
			}
		}
	}
	return moveNum;
	//int edgeNum = 0;
	//for (int y = 1; y < boardSize - 1; y = y + 2)
	//{
	//	//先判定头部第一个格子与外界的边是否自由边
	//	if (IsFreeBox(1, y) && IsEdgeLegality(0, y))//第一个为交格而且与外界交互的边为空边
	//	{
	//		edgeNum++;//总自由边数目自增1
	//	}
	//	//循环判定中间的几个格子
	//	for (int x = 1; x < boardSize - 3; x = x + 2)//x轴
	//	{
	//		if (IsFreeBox(x, y) && IsFreeBox(x + 2, y) && IsEdgeLegality(x + 1, y))
	//		{
	//			edgeNum++;//总自由边数目自增1
	//		}
	//	}
	//	//判断末尾的格子
	//	if (IsFreeBox(boardSize - 2, y) && IsEdgeLegality(boardSize - 1, y))//最后一个为交格且与外界交互的边为空边
	//	{
	//		edgeNum++;//总自由边数目自增1
	//	}
	//	//先判定头部第一个格子与外界的边是否自由边
	//	if (IsFreeBox(y, 1) && IsEdgeLegality(y, 0))//第一个为交格而且与外界交互的边为空边
	//	{
	//		edgeNum++;//总自由边数目自增1
	//	}
	//	//循环判定中间的几个格子
	//	for (int x = 1; x < boardSize - 3; x = x + 2)//x轴
	//	{
	//		if (IsFreeBox(y, x) && IsFreeBox(y, x + 2) && IsEdgeLegality(y, x + 1))
	//		{
	//			edgeNum++;//总自由边数目自增1
	//		}
	//	}
	//	//判断末尾的格子
	//	if (IsFreeBox(y, boardSize - 2) && IsEdgeLegality(y, boardSize - 1))//最后一个为交格且与外界交互的边为空边
	//	{
	//		edgeNum++;//总自由边数目自增1
	//	}
	//}
	//return edgeNum;//返回自由边的总数
}

bool Board::IsCTypeBoxExist(int x, int y)
{
	//遍历Box
	for (int i = 1; i < boardSize - 1; i += 2)
	{
		for (int j = 1; j < boardSize - 1; j += 2)
		{
			if (GetBoxLiberties(i, j)==DEADBOX)
			{
				return true;
			}
		}
	}
	return false;
}

//直接从board获得自由度，判断是否是CBox
bool Board::IsCTypeBox(int x, int y)
{
	if (x > 0 && x < boardSize - 1 && y>0 && y < boardSize - 1 && (x * y) % 2)
	{
		if (GetBoxLiberties(x, y) == DEADBOX)return true;
		else return false;
	}
	//cout << "不是Box" << "\n";
	return false;
}

//占领CBox
bool Board::GetCTypeBox(int player, bool isPush)
{
	int flag = 0;
	for (int i = 1; i < boardSize; i += 2)
	{
		for (int j = 1; j < boardSize; j += 2)
		{
			if (IsCTypeBox(i, j))
			{
				for (int k = 0; k < 4; k++)
				{
					int ex = i + dx[k];
					int ey = j + dy[k];

					if (IsEdgeLegality(ex, ey))
					{
						TryMove(coor(ex, ey), player, isPush);
						//flag = 1;
						return true;
					}
				}
			}
		}
	}
	if (flag)return true;
	return false;
}

//DeadBox+ChainBox
bool Board::IsLongCTypeBoxExist()
{
	for (int i = 1; i < boardSize - 1; i += 2)
	{
		for (int j = 1; j < boardSize - 1; j += 2)
		{
			//偏移量
			//int dx[] = { 0,1,0,-1 };
			//int dy[] = { 1,0,-1,0 };
			if (GetBoxLiberties(i, j) != DEADBOX)continue;
			for (int k = 0; k < 4; k++)
			{
				//邻边
				int ex = i + dx[k];
				int ey = j + dy[k];

				//邻格
				int bx = i + dx[k] * 2;
				int by = j + dy[k] * 2;
				if (IsEdgeLegality(ex, ey) && bx > 0 && bx < boardSize - 1 && by>0 && by < boardSize - 1)
				{
					if (GetBoxLiberties(bx, by) == CHAINBOX)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

void Board::GetAllCTypeBoxes(int player, bool isPush)
{
	while (1)
	{
		if (!GetCTypeBox(player,isPush))break;
	}
}

coor Board::GetDoubleCrossLoc(int Player)
{
	coor p = { -1,-1 };
	for (int i = 1; i < boardSize - 1; i += 2)
	{
		for (int j = 1; j < boardSize - 1; j += 2)
		{
			if (IsCTypeBox(i, j))
			{
				for (int k = 0; k < 4; k++)
				{
					int ex = i + dx[k];
					int ey = j + dy[k];

					int bx = i + dx[k] * 2;
					int by = j + dy[k] * 2;
					if (IsEdgeLegality(ex, ey) && bx > 0 && bx < boardSize - 1 && by>0 && by < boardSize - 1)
					{
						for (int l = 0; l < 4; l++)
						{
							int nex = bx + dx[l];
							int ney = by + dy[l];
							if (IsEdgeLegality(nex, ney) && (nex != ex || ney != ey))
							{
								p = { nex,ney };
								return p;
							}
						}
					}
				}
			}
		}
	}
	return p;
}

bool Board::GetCTypeBoxLimit(int player,bool isPush)
{
	for (int i = 1; i < boardSize; i += 2)
	{
		for (int j = 1; j < boardSize; j += 2)
		{
			if (IsCTypeBox(i, j))
			{
				for(int k=0;k<4;k++)
				{
					int ex = i + dx[k];
					int ey = j + dy[k];

					int nx = i + dx[k] * 2;
					int ny = j + dy[k] * 2;

					if (IsEdgeLegality(ex, ey) && IsCTypeBox(nx, ny))
					{
						TryMove(coor(ex, ey), player, isPush);
						return true;
					}
				}
			}
		}
	}
	return false;
}

//所有空边
int Board::GetAllMoves(std::vector<coor>&moves)
{
	moves.resize(0);
	//coor p;
	int moveNum = 0;
	for (int i = 0; i < boardSize; i++)
	{
		int j = (i + 1) % 2;
		for (; j < boardSize; j += 2)
		{
			if (IsEdgeLegality(i, j))
			{
				moveNum++;
				//p = { i,j };
				moves.push_back(coor(i, j));
			}
		}
	}
	return moveNum;
}

int Board::GetFreeMoves(std::vector<coor>&moves)
{
	//moves.resize(0);
	//int edgeNum = 0;
	//for (int y = 1; y < boardSize - 1; y = y + 2)
	//{
	//	//先判定头部第一个格子与外界的边是否自由边
	//	if (IsFreeBox(1, y) && IsEdgeLegality(0, y))//第一个为交格而且与外界交互的边为空边
	//	{
	//		moves.push_back(coor(0, y));
	//		edgeNum++;//总自由边数目自增1
	//	}
	//	//循环判定中间的几个格子
	//	for (int x = 1; x < boardSize - 3; x = x + 2)//x轴
	//	{
	//		if (IsFreeBox(x, y) && IsFreeBox(x + 2, y) && IsEdgeLegality(x + 1, y))
	//		{
	//			moves.push_back(coor(x + 1, y));
	//			edgeNum++;//总自由边数目自增1
	//		}
	//	}
	//	//判断末尾的格子
	//	if (IsFreeBox(boardSize - 2, y) && IsEdgeLegality(boardSize - 1, y))//最后一个为交格且与外界交互的边为空边
	//	{
	//		moves.push_back(coor(boardSize - 1, y));
	//		edgeNum++;//总自由边数目自增1
	//	}
	//	//先判定头部第一个格子与外界的边是否自由边
	//	if (IsFreeBox(y, 1) && IsEdgeLegality(y, 0))//第一个为交格而且与外界交互的边为空边
	//	{
	//		moves.push_back(coor(y, 0));
	//		edgeNum++;//总自由边数目自增1
	//	}
	//	//循环判定中间的几个格子
	//	for (int x = 1; x < boardSize - 3; x = x + 2)//x轴
	//	{
	//		if (IsFreeBox(y, x) && IsFreeBox(y, x + 2) && IsEdgeLegality(y, x + 1))
	//		{
	//			moves.push_back(coor(y, x + 1));
	//			edgeNum++;//总自由边数目自增1
	//		}
	//	}
	//	//判断末尾的格子
	//	if (IsFreeBox(y, boardSize - 2) && IsEdgeLegality(y, boardSize - 1))//最后一个为交格且与外界交互的边为空边
	//	{
	//		moves.push_back(coor(y, boardSize - 1));
	//		edgeNum++;//总自由边数目自增1
	//	}
	//}
	//return edgeNum;//返回自由边的总数
	moves.resize(0);
	int moveNum = 0;
	for (int i = 0; i < boardSize; i++)
	{
		int j = (i + 1) % 2;
		for (; j < boardSize; j += 2)
		{
			if (IsEdgeLegality(i, j))
			{
				if (IsFreeEdge(i, j))
				{
					moveNum++;
					moves.push_back(coor(i, j));
				}
			}
		}
	}
	return moveNum;
}

int Board::GetFilterMoves(std::vector<coor>& moves, bool isGet)
{
	int moveNum = 0;
	moves.resize(0);
	//moveNum = GetFreeMoves(moves);
	//if (isGet == false)if (moveNum)return moveNum;
	for (int x = 0; x < boardSize; x++)
	{
		for (int y = 0; y < boardSize; y++)
		{
			if (board[x][y] == EDGE)//若为空白边
			{
				auto boardSave = board;
				int CurrentStep = step;
				TryMove(coor(x, y), RED);				//玩家模拟走一步试试
				if (x % 2 == 1 && y % 2 == 0)//X奇数Y偶数，横行
				{
					if (y == 0)
					{
						if (!IsLongCTypeBox(x, y + 1))//如果下面的那个格子没问题的话，这个招法也没问题
						{
							moves.push_back(coor(x, y));
							moveNum++;//总数目自增
						}
					}
					else if (y == boardSize - 1)
					{
						if (!IsLongCTypeBox(x, y - 1))//如果上面的那个格子没问题的话，这个招法也没问题
						{

							moves.push_back(coor(x, y));
							moveNum++;//总数目自增
						}
					}
					else
					{
						if (!IsLongCTypeBox(x, y + 1) && !IsLongCTypeBox(x, y - 1))//如果上下的格子都没问题的话，这个招法也没问题
						{
							moves.push_back(coor(x, y));
							moveNum++;//总数目自增
						}
					}
				}
				else//竖行
				{
					if (x == 0)
					{
						if (!IsLongCTypeBox(x + 1, y))//如果右边的那个格子没问题的话，这个招法也没问题
						{
							moves.push_back(coor(x, y));
							moveNum++;//总数目自增
						}
					}
					else if (x == boardSize - 1)
					{
						if (!IsLongCTypeBox(x - 1, y))//如果左边的那个格子没问题的话，这个招法也没问题
						{
							moves.push_back(coor(x, y));
							moveNum++;//总数目自增
						}
					}
					else
					{
						if (!IsLongCTypeBox(x + 1, y) && !IsLongCTypeBox(x - 1, y))//如果左右两边的格子都没问题的话，这个招法也没问题
						{
							moves.push_back(coor(x, y));
							moveNum++;//总数目自增
						}
					}
				}
				board = boardSave;
				step = CurrentStep;				//还原
			}
		}
	}

	return moveNum;
}

int Board::GetFilterMoveNum(bool isGet)
{
	int moveNum = 0;
	////moveNum = GetFreeMoves(moves);
	//moveNum = GetFreeEdgeNum();
	//if (isGet == false)if (moveNum)return moveNum;
	for (int x = 0; x < boardSize; x++)
	{
		for (int y = 0; y < boardSize; y++)
		{
			if (board[x][y] == EDGE)//若为空白边
			{
				auto boardSave = board;
				int CurrentStep = step;
				TryMove(coor(x, y), RED);				//玩家模拟走一步试试
				if (x % 2 == 1 && y % 2 == 0)//X奇数Y偶数，横行
				{
					if (y == 0)
					{
						if (!IsLongCTypeBox(x, y + 1))//如果下面的那个格子没问题的话，这个招法也没问题
						{
							moves.push_back(coor(x, y));
							moveNum++;//总数目自增
						}
					}
					else if (y == boardSize - 1)
					{
						if (!IsLongCTypeBox(x, y - 1))//如果上面的那个格子没问题的话，这个招法也没问题
						{
							moveNum++;//总数目自增
						}
					}
					else
					{
						if (!IsLongCTypeBox(x, y + 1) && !IsLongCTypeBox(x, y - 1))//如果上下的格子都没问题的话，这个招法也没问题
						{
							moveNum++;//总数目自增
						}
					}
				}
				else//竖行
				{
					if (x == 0)
					{
						if (!IsLongCTypeBox(x + 1, y))//如果右边的那个格子没问题的话，这个招法也没问题
						{
							moveNum++;//总数目自增
						}
					}
					else if (x == boardSize - 1)
					{
						if (!IsLongCTypeBox(x - 1, y))//如果左边的那个格子没问题的话，这个招法也没问题
						{
							moveNum++;//总数目自增
						}
					}
					else
					{
						if (!IsLongCTypeBox(x + 1, y) && !IsLongCTypeBox(x - 1, y))//如果左右两边的格子都没问题的话，这个招法也没问题
						{
							moveNum++;//总数目自增
						}
					}
				}
				board = boardSave;
				step = CurrentStep;				//还原
			}
		}
	}
	return moveNum;
}

bool Board::IsLongCTypeBox(int x, int y)
{
	if (GetBoxLiberties(x, y) == DEADBOX)//首先这个格子必须本身是一个C型格
	{
		//return true;
		//int dx[] = { 0,1,0,-1 };
		//int dy[] = { 1,0,-1,0 };
		for (int i = 0; i < 4; i++)
		{
			int ex = x + dx[i];
			int ey = y + dy[i];
			int bx = x + dx[i] * 2;	//下一个格子的实际地址
			int by = y + dy[i] * 2;	//下一个格子的实际地址
			if (IsEdgeLegality(ex, ey) && bx > 0 && bx < boardSize - 1 && by > 0 && by < boardSize - 1)
			{
				if (GetBoxLiberties(bx, by) == CHAINBOX)return true;
			}
			//else if (IsEdgeLegality(ex, ey))return true;
		}
	}
	return false;
}

bool Board::IsCoorSame(coor p1, coor p2)
{
	if (p1.x == p2.x && p1.y == p2.y)return true;
	return false;
}

//尝试行棋，并更新board信息
bool Board::TryMove(coor ep, int player,bool isPush)
{
	if (isPush)qMove.push(ep);
	if (IsEdgeLegality(ep.x, ep.y))
	{
		//占据edge
		board[ep.x][ep.y] = player;

		//更新邻格信息
		if (ep.x % 2 == 0 && ep.y % 2 == 1)
		{
			if (ep.x - 1 >= 0 && GetBoxLiberties(ep.x - 1, ep.y) == 0)board[ep.x - 1][ep.y] = player * 2;
			if (ep.x + 1 < boardSize && GetBoxLiberties(ep.x + 1, ep.y) == 0)board[ep.x + 1][ep.y] = player * 2;
		}
		else if (ep.x % 2 == 1 && ep.y % 2 == 0)
		{
			if (ep.y - 1 >= 0 && GetBoxLiberties(ep.x, ep.y - 1) == 0)board[ep.x][ep.y - 1] = player * 2;
			if (ep.y + 1 < boardSize && GetBoxLiberties(ep.x, ep.y + 1) == 0)board[ep.x][ep.y + 1] = player * 2;
		}
		return true;
	}
	return false;
}