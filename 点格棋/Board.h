#pragma once
#include<vector>
#include<queue>
#include<fstream>
#include"st.h"
//棋盘数据常量
#define RED 1
#define BLUE -1
#define EMPTY 0
#define EDGE 10
#define BOX 20
#define RED_EDGE 1
#define BLUE_EDGE -1
#define RED_BOX 2
#define BLUE_BOX -2

//定义大小
//#define LEN 11			//LEN为数组的长度，正常为格子数X2+1
#define MOVENUM 84		//MOVENUM为最大招式的大小，
//#define BOXNUM 25		//总格子数
//#define BOXLEN 5		//格子数组长度

//定义格子类型
#define FULLBOX	0		//满格
#define DEADBOX 1		//死格
#define CHAINBOX 2		//链格
#define FREEBOX 3		//自由格

//TURN类用于表示一个新的回合，里面包含了新回合的局面以及所落的招法
class TURN
{
public:
	int onwer;
	int boardSize;
	std::vector<std::vector<bool>>captual;
	std::vector<std::vector<int>>newBoard;

	TURN(int);
	void SetBoard(std::vector<std::vector<int>>); //给newBoard赋值
	//void SetCaptualInfo(std::vector<std::vector<int>> OB, std::vector<std::vector<int>> NB);
};

class Board
{
public:
	std::vector<std::vector<int>>board;
	int step;
	int boardSize;
	std::vector<coor>moves;
	std::queue<coor>qMove;

	Board();
	//Board(int = 5);
	Board(std::vector<std::vector<int>>, int);
	void BoardReSize(int);							//改变box大小，重置数据
	void ShowLiberties();							//显示格子自由度
	void ShowBoard();
	int GetEmptyEdges();							//得到未被占领的边的数目
	int GetEmptyBoxes();							//得到未被占领的格子的数目
	int GetEdgeOwner(int x, int y);					//得到某个边的所有者
	int GetBoxOwner(int x, int y);					//得到某个格子的所有者
	int GetBoxLiberties(int x, int y);				//得到某个格子的自由度
	int GetPlayerBoxes(int player);					//得到某个玩家占领的格子的总数
	int GetPlayerEdges(int player);					//得到某个玩家占领的边的总数
	bool IsEdgeLegality(int x, int y);				//得到某个边是否合法的函数
	int Winner();									//得到胜利者（-1/0/1）
	void SetBoard(std::vector<std::vector<int>>);	//设置局面
	//void LoadBoard();								//读取局面

	//局面估值
	bool IsFreeBox(int x, int y);					//判断某个格子是否为自由格
	bool IsFreeEdge(int x, int y);					//判断是否是自由边
	int GetFreeEdgeNum();							//得到自由边的数量
	bool IsCTypeBoxExist(int x, int y);				//判断局面中是否有C型格(死格)
	bool IsCTypeBox(int, int);
	bool GetCTypeBox(int player, bool isPush = false);					//占领一个局面中的C型格
	bool IsLongCTypeBoxExist();						//判断局面中是否存在长C型链
	void GetAllCTypeBoxes(int player, bool isPuhs = false);				//吃掉所有的C型格
	//coor FindNextBox(coor FoundBox, coor LastBox);	//查找某个链格

	//后期计算
	coor GetDoubleCrossLoc(int Player);				//长链留格操作，查找一个doublecross的制作方法，返回值为该边的坐标
	bool GetCTypeBoxLimit(int player, bool isPush = false);				//限制版吃C型格

	//Move是当前最有效率的方法，也就是仅考虑最后一步，在之前均使用贪婪方法
	//bool Move(int x, int y, int owner);				//占领一条边的函数,占边由Game执行，这里不考虑
	int GetAllMoves(std::vector<coor>&);			//得到所有的空边
	int GetFreeMoves(std::vector<coor>&);			//得到所有的自由边
	int GetFilterMoves(std::vector<coor>&, bool = false);			//得到所有的过滤可行边(不产生长链)
	int GetFilterMoveNum(bool = false);							//得到所有的过滤可行边的数量
	bool IsLongCTypeBox(int, int);					//判断一个格子是否是一条长死格的起点

	bool IsCoorSame(coor p1, coor p2);				
	bool TryMove(coor, int player,bool isPush=false);
};