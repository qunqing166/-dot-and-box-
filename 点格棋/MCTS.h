#pragma once
#include"BoxBoard.h"

#define UCT_MC_TIMES 1
#define UCT_TIMES 15000000
#define UCT_LIMIT_TIME 32
#define UCT_FILTER_RANGE 30

class UCTNode :public Board
{
public:
	//UCT使用的值
	int owner;						//所属玩家
	int visitTimes;					//被尝试过的次数
	int boardWinner;				//这个局面的胜利者
	int existChild;					//子节点当前的数目
	int totalChild;					//子节点的总数
	float avgValue;					//对父节点而言，这个节点的平均收益 

	std::vector<coor>nodeMoves;			//这个子节点所有可能的招式
	std::vector<UCTNode*>childNodes;	//指向第一个子节点的指针
	//coor nodeMoves[MOVENUM];		
	//UCTNode* childNodes[MOVENUM];	

	UCTNode();
	UCTNode(int player, std::vector<std::vector<int>>anrry, int step, bool getCBox, int filterRange);
	void ReSet(int player, std::vector<std::vector<int>>anrry, int step, bool getCBox, int filterRange);


	//功能函数
	float RefreshAvgValue();									//更新平均收益
	float RefreshAvgValueWithSolver();
	float GetUCBValue(int total);								//根据现在的平均收益而获得UCB值
	float GetUCBRaveValue(int Total, coor Move, std::vector<std::vector<int>>RaveTable);
	UCTNode* ExpandUCTNode(int MC_Times, int Filter_Range);
	UCTNode* ExpandUCTNodeRave(int MC_Times, int Filter_Range, std::vector<std::vector<int>>RaveTable);
};

float UCTProcess(UCTNode& B, int& total, int MCTimes, int filterRange);
void LatterSituationMove(Board& CB, int player);
void UCTMoveWithSacrifice(Board& CB, int player);

void UCTMove(Board& CB, int player);

void GameTurnMove(Board& CB, int player);

void DeleteUCTNode(UCTNode* Root);
void DeleteUCTTree(UCTNode root);

int GetBoardWinner(Board& CB, int latterPlayer);
float GetFilterMCEvalution(Board& CB, int nextPlayer, int winner, int TIMES, int filterRange);
float GetRaveFilterMCEvalution(Board& CB, int nextPlayer, int winner, int TIMES, int filterRange, std::vector<std::vector<int>> raveTable);
int GetFilterMCWinner(Board& CB, int nextPlayer, int filterRange);