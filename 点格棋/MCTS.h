#pragma once
#include"BoxBoard.h"

#define UCT_MC_TIMES 1
#define UCT_TIMES 15000000
#define UCT_LIMIT_TIME 32
#define UCT_FILTER_RANGE 30

class UCTNode :public Board
{
public:
	//UCTʹ�õ�ֵ
	int owner;						//�������
	int visitTimes;					//�����Թ��Ĵ���
	int boardWinner;				//��������ʤ����
	int existChild;					//�ӽڵ㵱ǰ����Ŀ
	int totalChild;					//�ӽڵ������
	float avgValue;					//�Ը��ڵ���ԣ�����ڵ��ƽ������ 

	std::vector<coor>nodeMoves;			//����ӽڵ����п��ܵ���ʽ
	std::vector<UCTNode*>childNodes;	//ָ���һ���ӽڵ��ָ��
	//coor nodeMoves[MOVENUM];		
	//UCTNode* childNodes[MOVENUM];	

	UCTNode();
	UCTNode(int player, std::vector<std::vector<int>>anrry, int step, bool getCBox, int filterRange);
	void ReSet(int player, std::vector<std::vector<int>>anrry, int step, bool getCBox, int filterRange);


	//���ܺ���
	float RefreshAvgValue();									//����ƽ������
	float RefreshAvgValueWithSolver();
	float GetUCBValue(int total);								//�������ڵ�ƽ����������UCBֵ
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