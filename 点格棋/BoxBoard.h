#pragma once
#include"st.h"
#include"Board.h"
#include<vector>
#include"MCTS.h"

enum chaintype
{
	NotDefine,		//无定义
	SingleChain,	//单链
	LongChain,		//长链
	ShortChain,		//短链
	Circle,			//环
	PreCircle,		//预备环
	DeadChain,		//死链
	DeadCircle		//死环
};

class boxtype
{
public:
	boxtype();
	coor p;				//坐标
	int owner;			//属主
	int type;			//格子类型
	int belongingChainNum;		//链类型编号
};

class chaininfo
{
public:
	chaininfo();
	chaintype type;		//链格类型
	int chainBoxNum;	//链格数量
	coor pStart, pEnd;	//链格起始坐标
	bool preCircleCondition;		
};

class boxboard :public Board
{
public:
	int boxBoardSize;
	std::vector<std::vector<boxtype>>boxes;
	std::vector<chaininfo> chains;
	boxboard();
	boxboard(Board b);
	void BoxReSize(int num);
	//boxboard(int = 5);
	//boxboard(Board);									//构造函数
	//boxboard(std::vector<std::vector<int>>, int);		//构造函数
	//boxtype Boxes[BOXLEN + 1][BOXLEN + 1];			//后期棋盘的基本类型，由格子组成,编号从1开始。
	//chaininfo Chains[BOXNUM];							//链的数量必定不会超过全部格子数
	int GetFirstEmptyChainNum();						//获得一个空的链编号
	void DefineBoxesType();								//将所有格子的基本信息填入。
	int GetBoxType(int bx, int by);						//得到某个格子的类型，不在坐标范围内的全部返回自由格类型[坐标限制为1到BOXLEN]。
	void ShowBoxType();									//显示所有格子的类型

	//鉴别链
	void ShowBelongingChain();									//显示所有格子所属的链与各个链的信息
	void InheritChain(int InheritorRegNum, int AncesterRegNum);	//一条链吞并另一条链
	void RegisterChain(coor FreeBoxLoc, coor NextLoc);			//从一个场内的自由格出发，给所有的派生链做一个定义。
	void RegisterCircle(coor StartLoc, coor NextLoc);			//从一个格出发，确定是否为一个环。
	void SearchingFromBox(coor BoxLoc);							//从一个格出发，注册他的所有派生链,ChianPlus应仅在没有短链时启用。
	void SearchingCircle(coor BoxLoc);							//从一个格出发，注册他的所有派生环。
	void ResetChainsInfo();										//重定义链与格的的信息
	void DefineAllChains(bool ChainPlus);						//定义所有的链

	//说明：链是动态注册的。当一条链被归类到另一条链时候，他本身会被重新定义为"NotDefine"，而每次需要注册链的时候会从0开始查找到一条空链。

	//类型判断
	void RegisterDeadChain(coor FreeBoxLoc, coor NextLoc);		//从一个场内的死格出发，给其派生链做一个定义。
	void SearchingDeadChain(coor BoxLoc);						//从一个格出发，注册他的所有派生环。
	void DefineDeadChain();										//检查所有的死格
	bool IsDeadChainExist();
	bool IsDeadCircleExist();

	//占据链
	bool CaptualAnySingleChain(int latterPlayer);				//随机吞并一条单链，参数为占领者。
	bool CaptualAnyShortChain(int latterPlayer);				//随机吞并一条短链，参数为占领者。
	bool CaptualShortestChain(int latterPlayer);				//吞并一条当前最短的长链或者环

	//打开链
	coor GetOpenShortestChainLoc();
	coor GetOpenSuitableChainLoc();

	//最终判定局面
	bool IsRationalState(coor BoxNum);							//判断双方是否为理性状态
	coor GetRationalStateBoxNum();								//获得当前局面下双方以理性状态可以获得的格子数量，x为先手，y为后手
	int GetBoardWinner(int LatterPlayer);						//获得本局面下的胜利者，参数为后手方。
};