#pragma once
#include"st.h"
#include"Board.h"
#include<vector>
#include"MCTS.h"

enum chaintype
{
	NotDefine,		//�޶���
	SingleChain,	//����
	LongChain,		//����
	ShortChain,		//����
	Circle,			//��
	PreCircle,		//Ԥ����
	DeadChain,		//����
	DeadCircle		//����
};

class boxtype
{
public:
	boxtype();
	coor p;				//����
	int owner;			//����
	int type;			//��������
	int belongingChainNum;		//�����ͱ��
};

class chaininfo
{
public:
	chaininfo();
	chaintype type;		//��������
	int chainBoxNum;	//��������
	coor pStart, pEnd;	//������ʼ����
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
	//boxboard(Board);									//���캯��
	//boxboard(std::vector<std::vector<int>>, int);		//���캯��
	//boxtype Boxes[BOXLEN + 1][BOXLEN + 1];			//�������̵Ļ������ͣ��ɸ������,��Ŵ�1��ʼ��
	//chaininfo Chains[BOXNUM];							//���������ض����ᳬ��ȫ��������
	int GetFirstEmptyChainNum();						//���һ���յ������
	void DefineBoxesType();								//�����и��ӵĻ�����Ϣ���롣
	int GetBoxType(int bx, int by);						//�õ�ĳ�����ӵ����ͣ��������귶Χ�ڵ�ȫ���������ɸ�����[��������Ϊ1��BOXLEN]��
	void ShowBoxType();									//��ʾ���и��ӵ�����

	//������
	void ShowBelongingChain();									//��ʾ���и����������������������Ϣ
	void InheritChain(int InheritorRegNum, int AncesterRegNum);	//һ�����̲���һ����
	void RegisterChain(coor FreeBoxLoc, coor NextLoc);			//��һ�����ڵ����ɸ�����������е���������һ�����塣
	void RegisterCircle(coor StartLoc, coor NextLoc);			//��һ���������ȷ���Ƿ�Ϊһ������
	void SearchingFromBox(coor BoxLoc);							//��һ���������ע����������������,ChianPlusӦ����û�ж���ʱ���á�
	void SearchingCircle(coor BoxLoc);							//��һ���������ע������������������
	void ResetChainsInfo();										//�ض��������ĵ���Ϣ
	void DefineAllChains(bool ChainPlus);						//�������е���

	//˵�������Ƕ�̬ע��ġ���һ���������ൽ��һ����ʱ��������ᱻ���¶���Ϊ"NotDefine"����ÿ����Ҫע������ʱ����0��ʼ���ҵ�һ��������

	//�����ж�
	void RegisterDeadChain(coor FreeBoxLoc, coor NextLoc);		//��һ�����ڵ����������������������һ�����塣
	void SearchingDeadChain(coor BoxLoc);						//��һ���������ע������������������
	void DefineDeadChain();										//������е�����
	bool IsDeadChainExist();
	bool IsDeadCircleExist();

	//ռ����
	bool CaptualAnySingleChain(int latterPlayer);				//����̲�һ������������Ϊռ���ߡ�
	bool CaptualAnyShortChain(int latterPlayer);				//����̲�һ������������Ϊռ���ߡ�
	bool CaptualShortestChain(int latterPlayer);				//�̲�һ����ǰ��̵ĳ������߻�

	//����
	coor GetOpenShortestChainLoc();
	coor GetOpenSuitableChainLoc();

	//�����ж�����
	bool IsRationalState(coor BoxNum);							//�ж�˫���Ƿ�Ϊ����״̬
	coor GetRationalStateBoxNum();								//��õ�ǰ������˫��������״̬���Ի�õĸ���������xΪ���֣�yΪ����
	int GetBoardWinner(int LatterPlayer);						//��ñ������µ�ʤ���ߣ�����Ϊ���ַ���
};