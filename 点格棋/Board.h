#pragma once
#include<vector>
#include<queue>
#include<fstream>
#include"st.h"
//�������ݳ���
#define RED 1
#define BLUE -1
#define EMPTY 0
#define EDGE 10
#define BOX 20
#define RED_EDGE 1
#define BLUE_EDGE -1
#define RED_BOX 2
#define BLUE_BOX -2

//�����С
//#define LEN 11			//LENΪ����ĳ��ȣ�����Ϊ������X2+1
#define MOVENUM 84		//MOVENUMΪ�����ʽ�Ĵ�С��
//#define BOXNUM 25		//�ܸ�����
//#define BOXLEN 5		//�������鳤��

//�����������
#define FULLBOX	0		//����
#define DEADBOX 1		//����
#define CHAINBOX 2		//����
#define FREEBOX 3		//���ɸ�

//TURN�����ڱ�ʾһ���µĻغϣ�����������»غϵľ����Լ�������з�
class TURN
{
public:
	int onwer;
	int boardSize;
	std::vector<std::vector<bool>>captual;
	std::vector<std::vector<int>>newBoard;

	TURN(int);
	void SetBoard(std::vector<std::vector<int>>); //��newBoard��ֵ
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
	void BoardReSize(int);							//�ı�box��С����������
	void ShowLiberties();							//��ʾ�������ɶ�
	void ShowBoard();
	int GetEmptyEdges();							//�õ�δ��ռ��ıߵ���Ŀ
	int GetEmptyBoxes();							//�õ�δ��ռ��ĸ��ӵ���Ŀ
	int GetEdgeOwner(int x, int y);					//�õ�ĳ���ߵ�������
	int GetBoxOwner(int x, int y);					//�õ�ĳ�����ӵ�������
	int GetBoxLiberties(int x, int y);				//�õ�ĳ�����ӵ����ɶ�
	int GetPlayerBoxes(int player);					//�õ�ĳ�����ռ��ĸ��ӵ�����
	int GetPlayerEdges(int player);					//�õ�ĳ�����ռ��ıߵ�����
	bool IsEdgeLegality(int x, int y);				//�õ�ĳ�����Ƿ�Ϸ��ĺ���
	int Winner();									//�õ�ʤ���ߣ�-1/0/1��
	void SetBoard(std::vector<std::vector<int>>);	//���þ���
	//void LoadBoard();								//��ȡ����

	//�����ֵ
	bool IsFreeBox(int x, int y);					//�ж�ĳ�������Ƿ�Ϊ���ɸ�
	bool IsFreeEdge(int x, int y);					//�ж��Ƿ������ɱ�
	int GetFreeEdgeNum();							//�õ����ɱߵ�����
	bool IsCTypeBoxExist(int x, int y);				//�жϾ������Ƿ���C�͸�(����)
	bool IsCTypeBox(int, int);
	bool GetCTypeBox(int player, bool isPush = false);					//ռ��һ�������е�C�͸�
	bool IsLongCTypeBoxExist();						//�жϾ������Ƿ���ڳ�C����
	void GetAllCTypeBoxes(int player, bool isPuhs = false);				//�Ե����е�C�͸�
	//coor FindNextBox(coor FoundBox, coor LastBox);	//����ĳ������

	//���ڼ���
	coor GetDoubleCrossLoc(int Player);				//�����������������һ��doublecross����������������ֵΪ�ñߵ�����
	bool GetCTypeBoxLimit(int player, bool isPush = false);				//���ư��C�͸�

	//Move�ǵ�ǰ����Ч�ʵķ�����Ҳ���ǽ��������һ������֮ǰ��ʹ��̰������
	//bool Move(int x, int y, int owner);				//ռ��һ���ߵĺ���,ռ����Gameִ�У����ﲻ����
	int GetAllMoves(std::vector<coor>&);			//�õ����еĿձ�
	int GetFreeMoves(std::vector<coor>&);			//�õ����е����ɱ�
	int GetFilterMoves(std::vector<coor>&, bool = false);			//�õ����еĹ��˿��б�(����������)
	int GetFilterMoveNum(bool = false);							//�õ����еĹ��˿��бߵ�����
	bool IsLongCTypeBox(int, int);					//�ж�һ�������Ƿ���һ������������

	bool IsCoorSame(coor p1, coor p2);				
	bool TryMove(coor, int player,bool isPush=false);
};