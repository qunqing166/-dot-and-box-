#include "BoxBoard.h"
#include<iostream>
#include<queue>
using namespace std;

const int dx[] = { 0,1,0,-1 };
const int dy[] = { 1,0,-1,0 };

boxtype::boxtype()
{
	type = 0;		//Ĭ������
	belongingChainNum = 0;
}

chaininfo::chaininfo()
{
	type = NotDefine;				//����ΪΪ����
	chainBoxNum = 0;				//�����������ڿ�ʼʱ��Ϊ0
	preCircleCondition = false;				//�Ƿ�Ԥ�������Ⱦ�����
}

//boxboard::boxboard(int n)
//{
//	boxBoardSize = n;
//	std::vector<std::vector<int>>b(2 * n + 1, std::vector<int>(2 * n + 1));
//	board = b;
//	step = 0;
//	for (int i = 0; i < boxBoardSize + 1; i++)
//	{
//		boxes[i].resize(boxBoardSize);
//	}
//	chains.resize(boxBoardSize * boxBoardSize);
//}

//boxboard::boxboard()
//{
//	boxBoardSize = 5;
//	std::vector<std::vector<int>>b(2 * 5 + 1, std::vector<int>(2 * 5 + 1));
//	board = b;
//	step = 0;
//	for (int i = 0; i < boxBoardSize + 1; i++)
//	{
//		boxes[i].resize(boxBoardSize);
//	}
//	chains.resize(boxBoardSize * boxBoardSize);
//}

//boxboard::boxboard(int num):Board(num)
//{
//	boxBoardSize = num;
//	//std::vector<std::vector<int>>b(2 * 5 + 1, std::vector<int>(2 * 5 + 1));
//	//board = b;
//	step = 0;
//	boxes.resize(boxBoardSize);
//	for (int i = 0; i < boxBoardSize; i++)
//	{
//		boxes[i].resize(boxBoardSize);
//	}
//	chains.resize(boxBoardSize * boxBoardSize);
//}

/*boxboard::boxboard(Board newBoard)
{
	step = newBoard.step;
	board = newBoard.board;
	boxBoardSize = board.size() / 2;
	boxes.resize(boxBoardSize+1);

	for (int i = 0; i < boxBoardSize + 1; i++)
	{
		boxes[i].resize(boxBoardSize);
	}
	chains.resize(boxBoardSize * boxBoardSize);
}*/

//boxboard::boxboard(std::vector<std::vector<int>> newBoard, int step1)
//{
//	step = step1;
//	board = newBoard;
//	boxBoardSize = board.size() / 2;
//
//	for (int i = 0; i < boxBoardSize + 1; i++)
//	{
//		boxes[i].resize(boxBoardSize);
//	}
//	chains.resize(boxBoardSize * boxBoardSize);
//}

boxboard::boxboard()
{
	//BoxReSize(0);
	//boxBoardSize = 5;
	////std::vector<std::vector<int>>b(2 * 5 + 1, std::vector<int>(2 * 5 + 1));
	////board = b;
	//step = 0;
	//boxes.resize(boxBoardSize);
	//for (int i = 0; i < boxBoardSize; i++)
	//{
	//	boxes[i].resize(boxBoardSize);
	//}
	//chains.resize(boxBoardSize * boxBoardSize);
}

boxboard::boxboard(Board b)
{
	boxBoardSize = b.board.size() / 2;
	BoxReSize(boxBoardSize);
	board = b.board;
	step = b.step;
}

void boxboard::BoxReSize(int num)
{
	BoardReSize(num);		//����Board��

	boxBoardSize = num;
	step = 0;
	//����boxes
	boxes.resize(boxBoardSize);
	for (int i = 0; i < boxBoardSize; i++)
	{
		boxes[i].resize(boxBoardSize);
	}
	//����chains
	chains.resize(boxBoardSize * boxBoardSize);
}

int boxboard::GetFirstEmptyChainNum()
{
	for (int i = 1; i < boxBoardSize * boxBoardSize; i++)
	{
		if (chains[i].type == NotDefine)
		{
			return i;
		}
	}
	cout << "chains�ռ䲻��" << '\n';
	return 0;
}

void boxboard::DefineBoxesType()
{
	for (int i = 0; i < boxBoardSize; i++)
	{
		for (int j = 0; j < boxBoardSize; j++)
		{
			int x = 2 * i + 1;
			int y = 2 * j + 1;
			if (board[x][y] == BOX)
			{
				boxes[i][j].owner = 0;
			}
			else
			{
				boxes[i][j].owner = board[x][y] / 2;
			}
			int a = GetBoxLiberties(x, y);
			if (a == 4)boxes[i][j].type = FREEBOX;
			else
			{
				boxes[i][j].type = a;
			}
		}
	}
}

int boxboard::GetBoxType(int x, int y)
{
	if (x >= 0 && y >= 0 && x < boxBoardSize && y < boxBoardSize)
	{
		return boxes[x][y].type;
	}
	//cout << "����Box" << '\n';
	return FREEBOX;
}

void boxboard::ShowBoxType()
{
	cout << "Box����:" << '\n';
	for (int i = 0; i < boxBoardSize; i++)
	{
		for (int j = 0; j < boxBoardSize; j++)
		{
			cout << boxes[i][j].type << " ";
		}
		cout << '\n';
	}
	cout << '\n' << '\n';
}

void boxboard::ShowBelongingChain()
{
	cout << "������:" << '\n';
	for (int i = 0; i < boxBoardSize; i++)
	{
		for (int j = 0; j < boxBoardSize; j++)
		{
			cout << boxes[i][j].belongingChainNum << ' ';
		}
		cout << '\n';
	}
	cout << '\n';
	cout << "����������:" << '\n';
	for (int i = 0; i < boxBoardSize; i++)
	{
		for (int j = 0; j < boxBoardSize; j++)
		{
			cout << chains[boxes[i][j].belongingChainNum].type << ' ';
		}
		cout << '\n';
	}
	cout << '\n';
	cout << "���  ����  ����" << endl;
	for (int i = 0; i < boxBoardSize * boxBoardSize; i++)
	{
		if(chains[i].chainBoxNum)printf("%2d  %4d  %4d\n", i, chains[i].type, chains[i].chainBoxNum);
		//cout << i << " " << chains[i].type << '\n';
	}
	cout << '\n';
}

void boxboard::InheritChain(int inheritorRegNum, int ancesterRegNum) 
{
	for (int i = 0; i < boxBoardSize; i++)
	{
		for (int j = 0; j < boxBoardSize; j++)
		{
			if (boxes[i][j].belongingChainNum == ancesterRegNum)
			{
				boxes[i][j].belongingChainNum = inheritorRegNum;//�̳���
			}
		}
	}
	//���¼̳�����Ϣ
	chains[inheritorRegNum].chainBoxNum += chains[ancesterRegNum].chainBoxNum;
	chains[inheritorRegNum].type = LongChain;
	//if(inheritorRegNum)
	//{
	//	chains[inheritorRegNum].chainBoxNum += chains[ancesterRegNum].chainBoxNum;
	//	chains[inheritorRegNum].type = LongChain;
	//}
	//else
	//{
	//	chains[inheritorRegNum].chainBoxNum = 0;
	//	chains[inheritorRegNum].type = NotDefine;
	//}

	//���±��̳�����Ϣ
	//chains.erase(chains.begin() + ancesterRegNum);
	chains[ancesterRegNum].chainBoxNum = 0;
	chains[ancesterRegNum].type = NotDefine;
}

void boxboard::RegisterChain(coor freeBoxP, coor firstP)
{
	int x = freeBoxP.x;
	int y = freeBoxP.y;
	coor p = freeBoxP;
	coor np = firstP;

	int re = GetFirstEmptyChainNum();
	chains[re].pStart = p;
	chains[re].chainBoxNum = 0;
	chains[re].type = NotDefine;


	queue<coor>q;
	q.push(np);
	while (!q.empty())
	{
		coor np = q.front(); 
		q.pop();
		if (GetBoxType(np.x, np.y) == CHAINBOX)
		{
			boxes[np.x][np.y].belongingChainNum = re;
			chains[re].chainBoxNum++;
			for (int k = 0; k< 4; k++)
			{
				int nx = np.x + dx[k];
				int ny = np.y + dy[k];

				int ex = 2 * np.x + 1 + dx[k];
				int ey = 2 * np.y + 1 + dy[k];

				if ( IsEdgeLegality(ex, ey) && (nx != p.x || ny != p.y))
				{
					if(nx >= 0 && nx < boxBoardSize && ny >= 0 && ny < boxBoardSize)
					{
						//boxes[np.x][np.y].belongingChainNum = re;
						//chains[re].chainBoxNum++;
						p = { nx,ny };
						q.push(p);
						p = np;
						break;
					}
					else
					{
						//chains[re].chainBoxNum++;
						if (chains[re].chainBoxNum == 1)chains[re].type = SingleChain;
						else if (chains[re].chainBoxNum == 2)chains[re].type = ShortChain;
						else chains[re].type = LongChain;
					}
				}
			}
		}
		else
		{
			chains[re].pEnd = np;
			if (boxes[np.x][np.y].type == EMPTY && np.x == x && np.y == y)
			{
				chains[re].type = PreCircle;
				boxes[np.x][np, y].belongingChainNum = re;
				chains[re].chainBoxNum++;
			}
			else
			{
				if (chains[re].chainBoxNum == 1)chains[re].type = SingleChain;
				else if (chains[re].chainBoxNum == 2)chains[re].type = ShortChain;
				else chains[re].type = LongChain;
			}
		}
	}
}

void boxboard::RegisterCircle(coor fp, coor np)
{
	//int x = fp.x;
	//int y = fp.y;
	coor p = fp;
	
	int re = GetFirstEmptyChainNum();
	chains[re].pStart = fp;
	chains[re].chainBoxNum = 0;
	//boxes[fp.x][fp.y].belongingChainNum = re;
	chains[re].type = NotDefine;

	queue<coor>q;
	q.push(np);
	while (!q.empty())
	{
		np = q.front(); q.pop();
		if (boxes[np.x][np.y].type != CHAINBOX)
		{
			InheritChain(EMPTY, re);
			break;
		}
		else if (np.x == fp.x && np.y == fp.y)
		{
			chains[re].type = Circle;
			//chains[re].pStart = np;							//������յ㶼���Ϊ�������
			chains[re].pEnd = np;							//������յ㶼���Ϊ�������
			boxes[np.x][np.y].belongingChainNum = re;		//�����ø�����Ҳ��Ҫ��ע��
			chains[re].chainBoxNum++;						//���ĸ�����Ŀ����1
			break;
		}
		else
		{
			boxes[np.x][np.y].belongingChainNum = re;
			chains[re].chainBoxNum++;
			for (int k = 0; k < 4; k++)
			{
				int nx = np.x + dx[k];
				int ny = np.y + dy[k];

				int ex = 2 * np.x + 1 + dx[k];
				int ey = 2 * np.y + 1 + dy[k];

				if ( IsEdgeLegality(ex, ey) && (nx != p.x || ny != p.y))
				{
					if(nx >= 0 && nx < boxBoardSize && ny >= 0 && ny < boxBoardSize)
					{
						//boxes[np.x][np.y].belongingChainNum = re;
						//chains[re].chainBoxNum++;
						p = { nx,ny };
						q.push(p);
						p = np;
						break;
					}
					else
					{
						InheritChain(EMPTY, re);
						break;
						//if (chains[re].chainBoxNum == 1)chains[re].type = SingleChain;
						//else if (chains[re].chainBoxNum == 2)chains[re].type = ShortChain;
						//else if (chains[re].chainBoxNum >= 3)
						//{
						//	//chains[re].type = LongChain;
						//	InheritChain(EMPTY, re);
						//	break;
						//}
					}
				}
			}
		}
	}
}

void boxboard::SearchingFromBox(coor fp)
{
	for (int k = 0; k < 4; k++)
	{
		//�ڸ�
		int bx = fp.x + dx[k];
		int by = fp.y + dy[k];
		//�ڱ�
		int ex = 2 * fp.x + 1 + dx[k];
		int ey = 2 * fp.y + 1 + dy[k];
		//if (bx < 0 || by < 0 || bx >= n || by >= n)continue;
		if (IsEdgeLegality(ex, ey) && GetBoxType(bx, by) == CHAINBOX)
		{
			if (boxes[bx][by].belongingChainNum == EMPTY)
			{
				coor pl = { bx,by };
				RegisterChain(fp, pl);
			}
		}
	}
}

void boxboard::SearchingCircle(coor fp)
{
	for (int k = 0; k < 4; k++)
	{
		//�ڸ�
		int bx = fp.x + dx[k];
		int by = fp.y + dy[k];
		//�ڱ�
		int ex = 2 * fp.x + 1 + dx[k];
		int ey = 2 * fp.y + 1 + dy[k];

		if (IsEdgeLegality(ex, ey) && GetBoxType(bx, by) == CHAINBOX)
		{
			if (boxes[bx][by].belongingChainNum == EMPTY)
			{
				coor pl = { bx,by };
				RegisterCircle(fp, pl);
			}
		}
	}
}

void boxboard::ResetChainsInfo()
{
	for (int i = 0; i < boxBoardSize; i++)
	{
		for (int j = 0; j < boxBoardSize; j++)
		{
			boxes[i][j].belongingChainNum = 0;
		}
	}
	for (int i = 0; i < boxBoardSize * boxBoardSize; i++)
	{
		chains[i].chainBoxNum = 0;
		chains[i].type = NotDefine;
		chains[i].preCircleCondition = false;
	}
}

void boxboard::DefineAllChains(bool chainPlus)
{
	DefineBoxesType();
	ResetChainsInfo();

	//��������
	for (int i = 0; i < boxBoardSize; i++)
	{
		for (int j = 0; j < boxBoardSize; j++)
		{
			if (boxes[i][j].type == FREEBOX)
			{
				coor p = { i,j };
				SearchingFromBox(p);
			}
		}
	}

	for (int i = 0; i < boxBoardSize; i++)
	{
		SearchingFromBox(coor(i, -1));
		SearchingFromBox(coor(-1, i));
		SearchingFromBox(coor(i, boxBoardSize));
		SearchingFromBox(coor(boxBoardSize, i));
	}

	//������
	for (int i = 0; i < boxBoardSize; i++)
	{
		for (int j = 0; j < boxBoardSize; j++)
		{
			if (boxes[i][j].type == CHAINBOX && boxes[i][j].belongingChainNum == EMPTY)
			{
				coor p = { i,j };
				SearchingCircle(p);
			}
		}
	}

	if (!chainPlus)return;
	//���ҳ������ϲ���β��ͬ����LongChainsΪPreChians
	int boxNum = boxBoardSize * boxBoardSize;
	for (int i = 1; i < boxNum; i++)
	{
		if (chains[i].type == LongChain)
		{
			for (int j = i + 1; j < boxNum; j++)
			{
				if (chains[j].type == LongChain && IsCoorSame(chains[i].pStart, chains[j].pStart) && IsCoorSame(chains[i].pEnd, chains[j].pEnd))
				{
					//�ϲ�i��j��
					InheritChain(i, j);

					//����ԭ����յ㼰i����Ϣ
					boxes[chains[i].pStart.x][chains[i].pStart.y].belongingChainNum = i;
					boxes[chains[i].pEnd.x][chains[i].pEnd.y].belongingChainNum = i;
					chains[i].chainBoxNum += 2;
					chains[i].pStart = chains[i].pEnd;
					chains[i].type = PreCircle;
				}
			}
		}
	}

	//return;

	//����PreCircle��PreChain
	for (int i = 0; i < boxBoardSize; i++)
	{
		for (int j = 0; j < boxBoardSize; j++)
		{
			if (GetBoxType(i, j) == FREEBOX && chains[boxes[i][j].belongingChainNum].type == PreCircle)
			{
				for (int k = 0; k < 4; k++)
				{
					int nx = i + dx[k];
					int ny = j + dy[k];

					int ex = 2 * i + 1 + dx[k];
					int ey = 2 * j + 1 + dy[k];

					//�ҵ������ķ����Դ˲�������
					if (IsEdgeLegality(ex, ey) && chains[boxes[nx][ny].belongingChainNum].type == LongChain)
					{
						chains[boxes[nx][ny].belongingChainNum].preCircleCondition = true;
					}
				}
			}
		}
	}
	
	//return;

	//�ҵ�δ��������ɸ񣬽��������������������ϲ�
	for (int i = 0; i < boxBoardSize; i++)
	{
		for (int j = 0; j < boxBoardSize; j++)
		{
			if (GetBoxType(i, j) == FREEBOX && boxes[i][j].belongingChainNum == EMPTY)
			{
				std::vector<int>chainsRegNum;//��������������������
				for (int k = 0; k < 4; k++)
				{
					int nx = i + dx[k];
					int ny = j + dy[k];

					int ex = 2 * i + 1 + dx[k];
					int ey = 2 * j + 1 + dy[k];

					//�ڸ�Ϊ����
					if (IsEdgeLegality(ex, ey) && GetBoxType(nx, ny)==CHAINBOX)
					{
						chainsRegNum.push_back(boxes[nx][ny].belongingChainNum);
					}
				}
				if (chainsRegNum.size() < 2)return;
				//�������
				int firstChainNum = 0;
				int firstChainsBoxes=0;
				for (int chainNum : chainsRegNum)
				{
					if (chains[chainNum].chainBoxNum >= firstChainsBoxes)
					{
						firstChainNum = chainNum;
						firstChainsBoxes = chains[firstChainNum].chainBoxNum;
					}
				}

				//���Ҵγ���
				int secChainNum = 0;
				int secChainsBoxes = 0;
				for (int chainNum : chainsRegNum)
				{
					if (chains[chainNum].chainBoxNum >= secChainsBoxes && chains[chainNum].chainBoxNum != firstChainsBoxes)
					{
						secChainNum = chainNum;
						secChainsBoxes = chains[secChainNum].chainBoxNum;
					}
				}
				boxes[i][j].belongingChainNum = firstChainNum;
				chains[firstChainNum].chainBoxNum++;
				InheritChain(firstChainNum, secChainNum);
			}
		}
	}
}

void boxboard::RegisterDeadChain(coor fp, coor np)
{
	//ע��ʱpStart��δ����type--�Ѹ�

	coor p = fp;
	int re = GetFirstEmptyChainNum();
	chains[re].pStart = fp;
	chains[re].chainBoxNum = 1;
	boxes[p.x][p.y].belongingChainNum = re;//����ʼ����Ϣ
	chains[re].type = NotDefine;
	
	queue<coor>q;
	q.push(np);
	while (!q.empty())
	{
		np = q.front();
		q.pop();

		if (GetBoxType(np.x, np.y) == CHAINBOX)
		{
			boxes[np.x][np.y].belongingChainNum = re;
			chains[re].chainBoxNum++;

			for (int k = 0; k < 4; k++)
			{
				int nx = np.x + dx[k];
				int ny = np.y + dy[k];

				int ex = 2 * np.x + 1 + dx[k];
				int ey = 2 * np.y + 1 + dy[k];

				if (IsEdgeLegality(ex, ey) && (nx != p.x || ny != p.y))
				{
					if (nx >= 0 && nx < boxBoardSize && ny >= 0 && ny < boxBoardSize)
					{
						//boxes[np.x][np.y].belongingChainNum = re;
						//chains[re].chainBoxNum++;
						q.push(coor(nx, ny));
						p = np;
						break;
					}
					else
					{
						chains[re].type = DeadChain;
					}
				}
			}
		}
		else
		{
			chains[re].pEnd = np;					//���������յ�
			if (GetBoxType(np.x, np.y) == DEADBOX)	//����ն���һ������Ļ��������һ��DeadCircle
			{
				chains[re].type = DeadCircle;
				boxes[np.x][np.y].belongingChainNum = re;//�����ø�����Ҳ��Ҫ��ע��
				chains[re].chainBoxNum++;
			}
			else//�����յ㲻һ������Ϊ����������Ŀ�ж�������
			{
				chains[re].type = DeadChain;
			}
			break;
		}
	}
}

void boxboard::SearchingDeadChain(coor p)
{
	for (int k = 0; k < 4; k++)
	{
		int bx = p.x + dx[k];
		int by = p.y + dy[k];

		int ex = 2 * p.x + 1 + dx[k];
		int ey = 2 * p.y + 1 + dy[k];

		if (IsEdgeLegality(ex, ey) && GetBoxType(bx, by) == CHAINBOX)
		{
			if (boxes[bx][by].belongingChainNum == EMPTY)
			{
				RegisterDeadChain(p, coor(bx, by));
			}
		}
	}
}

void boxboard::DefineDeadChain()
{
	//����boxes��chains��Ϣ
	DefineBoxesType();
	ResetChainsInfo();

	//������������
	for (int i = 0; i < boxBoardSize; i++)
	{
		for (int j = 0; j < boxBoardSize; j++)
		{
			if (GetBoxType(i, j) == DEADBOX)
			{
				SearchingDeadChain(coor(i, j));
			}
		}
	}
}

bool boxboard::IsDeadChainExist()
{
	DefineDeadChain();
	for (int i = 1; i < boxBoardSize * boxBoardSize; i++)
	{
		if (chains[i].type == DeadChain)return true;
	}
	return false;
}

bool boxboard::IsDeadCircleExist()
{
	DefineDeadChain();
	for (int i = 1; i < boxBoardSize * boxBoardSize; i++)
	{
		if (chains[i].type == DeadCircle && chains[i].chainBoxNum > 3)return true;
	}
	return false;
}

bool boxboard::CaptualAnyShortChain(int latterPlayer)
{
	DefineAllChains(false);
	int boxNum = boxBoardSize * boxBoardSize;
	for (int i = 1; i < boxNum; i++)
	{
		if (chains[i].type == ShortChain)
		{
			//����ռ������м��Edge
			for (int x = 0; x < boxBoardSize; x++)
			{
				for (int y = 0; y < boxBoardSize; y++)
				{
					if (boxes[x][y].belongingChainNum == i)
					{
						for (int k = 0; k < 4; k++)
						{
							int bx = x + dx[k];
							int by = y + dy[k];

							int ex = 2 * x + 1 + dx[k];
							int ey = 2 * y + 1 + dy[k];

							if (IsEdgeLegality(ex, ey) && bx >= 0 && by >= 0 && bx < boxBoardSize && by < boxBoardSize && boxes[bx][by].belongingChainNum == i)
							{
								TryMove(coor(ex, ey), -latterPlayer);
								break;
							}
						}
					}
				}
			}

			//����ռ��ʣ��������
			for (int x = 0; x< boxBoardSize; x++)
			{
				for (int y = 0; y < boxBoardSize; y++)
				{
					if (boxes[x][y].belongingChainNum == i)
					{
						for (int k = 0; k < 4; k++)
						{
							int ex = 2 * x + 1 + dx[k];
							int ey = 2 * y + 1 + dy[k];
							if (IsEdgeLegality(ex, ey))
							{
								TryMove(coor(ex, ey), latterPlayer);
								break;
							}
						}
					}
					boxes[x][y].belongingChainNum = NotDefine;
				}
			}
			InheritChain(EMPTY, i);
			DefineBoxesType();
			return true;
		}
	}
	return false;
}

bool boxboard::CaptualShortestChain(int latterPlayer)
{
	DefineAllChains(false);

	int leastNum = 0;
	int leastBoxNum = boxBoardSize * boxBoardSize;

	for (int i = 1; i < boxBoardSize * boxBoardSize; i++)
	{
		if (chains[i].type == LongChain || chains[i].type == Circle)
		{
			if (chains[i].chainBoxNum <= leastBoxNum)
			{
				leastNum = chains[i].chainBoxNum;
				leastNum = i;
			}
		}
	}

	if (leastNum == 0)return false;

	bool isFinish = false;
	for (int i = 0; i < boxBoardSize; i++)
	{
		for (int j = 0; j < boxBoardSize; j++)
		{
			if (boxes[i][j].belongingChainNum == i && isFinish == false)
			{
				for (int k = 0; k < 4; k++)
				{
					int bx = i + dx[k];
					int by = j + dy[k];

					int ex = 2 * i + 1 + dx[k];
					int ey = 2 * j + 1 + dy[k];

					if (IsEdgeLegality(ex, ey) && bx >= 0 && by >= 0 && bx < boxBoardSize && by < boxBoardSize && boxes[bx][by].belongingChainNum == leastNum)
					{
						TryMove(coor(ex, ey), -latterPlayer);
						isFinish = true;
						break;
					}
				}
			}
		}
	}

	for (int i = 0; i < boxBoardSize; i++)
	{
		for (int j = 0; j < boxBoardSize; j++)
		{
			if (boxes[i][j].belongingChainNum == leastNum)
			{
				for (int k = 0; k < 4; k++)
				{
					int ex = 2 * i + 1 + dx[k];
					int ey = 2 * j + 1 + dy[k];

					if (IsEdgeLegality(ex, ey))
					{
						TryMove(coor(ex, ey), latterPlayer);
						break;
					}
				}
			}
			boxes[i][j].belongingChainNum = 0;
		}
	}
	InheritChain(EMPTY, leastNum);
	DefineBoxesType();
	return true;
}

coor boxboard::GetOpenShortestChainLoc()
{
	DefineAllChains(false);
	int leastNum;
	int leastBoxNum = boxBoardSize * boxBoardSize;
	int boxNum = leastBoxNum;
	
	for (int i = 1; i < boxNum; i++)
	{
		if (chains[i].type == LongChain || chains[i].type == Circle)
		{
			if (leastBoxNum > chains[i].chainBoxNum)
			{
				leastBoxNum = chains[i].chainBoxNum;
				leastNum = i;
			}
		}
	}

	for (int i = 0; i < boxBoardSize; i++)
	{
		for (int j = 0; j < boxBoardSize; j++)
		{
			if (boxes[i][j].belongingChainNum == leastNum)
			{
				for (int k = 0; k < 4; k++)
				{
					int bx = i + dx[k];
					int by = j + dy[k];

					int ex = 2 * i + 1 + dx[k];
					int ey = 2 * j + 1 + dy[k];

					if (bx >= 0 && by >= 0 && bx < boxBoardSize && by < boxBoardSize)
					{
						if (IsEdgeLegality(ex, ey) && boxes[bx][by].belongingChainNum == leastNum)
						{
							return coor(ex, ey);
						}
					}
				}
			}
		}
	}
	return coor(0, 0);
}

coor boxboard::GetOpenSuitableChainLoc()
{
	DefineAllChains(false);

	//���ҵ���̵���������/��
	int suitChainNum = 99;
	int boxNum = boxBoardSize * boxBoardSize;
	for (int i = 1; i < boxNum; i++)
	{
		if (chains[i].type == Circle)
		{
			suitChainNum = i;
		}
	}

	if (suitChainNum == 99)
	{
		for (int i = 1; i <boxNum; i++)
		{
			if (chains[i].type == LongChain && chains[i].preCircleCondition)
			{
				suitChainNum = i;
			}
		}
	}

	if (suitChainNum == 99)
	{
		for (int i = 1; i < boxNum; i++)
		{
			if (chains[i].type == LongChain)
			{
				suitChainNum = i;
			}
		}
	}

	//=======
	for (int i = 0; i < boxBoardSize; i++)
	{
		for (int j = 0; j < boxBoardSize; j++)
		{
			if (boxes[i][j].belongingChainNum == suitChainNum)
			{
				for (int k = 0; k < 4; k++)
				{
					int bx = i + dx[k];
					int by = j + dy[k];

					int ex = 2 * i + 1 + dx[k];
					int ey = 2 * j + 1 + dy[k];

					if (bx >= 0 && by >= 0 && bx < boxBoardSize && by < boxBoardSize)
					{
						if (IsEdgeLegality(ex, ey) && boxes[bx][by].belongingChainNum == suitChainNum)
						{
							return coor(ex, ey);
						}
					}
				}
			}
		}
	}
	return coor(0, 0);
}

bool boxboard::IsRationalState(coor boxNum)
{
	if (boxNum.x >= boxNum.y)
		return true;
	return false;
}

coor boxboard::GetRationalStateBoxNum()
{
	//��֪��ǰ�����Ƿ���һ�����Ծ��棬������Ҫ�жϵ�ǰ�������Ŀ��
	DefineAllChains(true);

	//�����������������Ŀ
	//����
	int LCNum = 0;
	int LCBox = 0;
	//��
	int CNum = 0;
	int CBox = 0;
	//Ԥ����
	int PCNum = 0;
	int PCBox = 0;

	bool onlyPreChain = true;//�Ƿ����Ԥ������Ҳ����Ԥ�������Ⱦ��������ǵĻ������һ���ض���Ԥ����
	for (int i = 1; i < boxBoardSize * boxBoardSize; i++)
	{
		if (chains[i].type != NotDefine)
		{
			if (chains[i].type == LongChain)
			{
				if (chains[i].preCircleCondition == false)onlyPreChain = false;
				LCNum++;
				LCBox += chains[i].chainBoxNum;
			}
			else if (chains[i].type == Circle)
			{
				CNum++;
				CBox += chains[i].chainBoxNum;
			}
			else if (chains[i].type == PreCircle)
			{
				PCNum++;
				PCBox += chains[i].chainBoxNum;
			}
		}
	}

	//��ʼ����������Ŀ
	int total = LCBox + PCBox + CBox;//�ܸ�����
	int sacrifice = 0;
	if (onlyPreChain)
	{
		if (LCNum == 0)//�����״���²����ڳ��������������ǻ�
		{
			sacrifice = (CNum - 1) * 4;
		}
		else//���ڳ�����������Ԥ���������һ���ض�ΪԤ����
		{
			sacrifice = (PCNum * 4) + (CNum * 4) + (LCNum * 2) - 4;
		}
	}
	else
	{
		//�г�����ʱ�����һ���ض��ǳ���
		sacrifice = (PCNum * 4) + (CNum * 4) + (LCNum * 2) - 2;
	}
	return coor(total - sacrifice, sacrifice);
}

int boxboard::GetBoardWinner(int LatterPlayer)
{
	if (GetFilterMoveNum() > 0)
		cout << "Wrong";
	bool debug = false;
	int player = LatterPlayer;
	DefineBoxesType();
	coor boxNum;
	for (;;)//������������̲�����
	{
		DefineAllChains(true);//�ȶ���Ϊ��ȫ״̬�ж�һ��
		boxNum = GetRationalStateBoxNum();//Ȼ�����ж�һ���������
		if (IsRationalState(boxNum))
			break;
		else
		{
			if (!CaptualShortestChain(player))//����Բ���ȥ��Ҳ�˳�
				break;
			else
				player = -player;
		}
	}
	//ShowBelongingChain();
	//system("pause");
	if (Winner() == 0)//Ҳ���ǻ�ûʤ��
	{
		int r, b;
		if (player == RED)
		{
			r = boxNum.x + GetPlayerBoxes(RED);
			b = boxNum.y + GetPlayerBoxes(BLUE);
		}
		else
		{
			r = boxNum.y + GetPlayerBoxes(RED);
			b = boxNum.x + GetPlayerBoxes(BLUE);
		}
		//cout << "Game Finish!\n Red [" << r << "] Blue [" << b << "]" << endl;
		if (r > b)
			return RED;
		else
			return BLUE;
	}
	else
		return Winner();
}
