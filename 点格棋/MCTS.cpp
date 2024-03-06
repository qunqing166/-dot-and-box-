#include "MCTS.h"
#include <ctime>

UCTNode::UCTNode()
{
    step = 0;
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            board[j][i] = 0;
        }
    }
    nodeMoves.reserve(MOVENUM);
    childNodes.resize(MOVENUM);
    existChild = 0;
    totalChild = 0;
}

UCTNode::UCTNode(int player, std::vector<std::vector<int>> b, int step1, bool getCBox, int filterRange)
{
    step = step1;
    board = b;
    boardSize = board.size();
    nodeMoves.reserve(MOVENUM);
    childNodes.resize(MOVENUM);

    //���������ռ����
    if (getCBox)
    {
        GetAllCTypeBoxes(player, true);
    }

    owner = player;
    visitTimes = 1;
    Board win(board, step);
    boardWinner = GetBoardWinner(win, -player);
    existChild = 0;

    //totalChild = GetFreeMoves(nodeMoves);
    int freeEdgeNum = win.GetFreeEdgeNum();
    if (freeEdgeNum >= filterRange)
        totalChild = GetFreeMoves(nodeMoves);		//��ǰ�����ܽڵ���
    else
    { 
        //totalChild = GetFreeMoves(nodeMoves);		//��ǰ�����ܽڵ���
        totalChild = GetFilterMoves(nodeMoves,true);		//��ǰ�����ܽڵ���
    }
}

void UCTNode::ReSet(int player, std::vector<std::vector<int>> b, int step1, bool getCBox, int filterRange)
{
    step = step1;
    board = b;
    boardSize = board.size();
    nodeMoves.reserve(MOVENUM);
    childNodes.resize(MOVENUM);

    if (getCBox)
    {
        GetAllCTypeBoxes(player);
    }
    owner = player;
    visitTimes = 1;
    Board win(board, step);
    boardWinner = GetBoardWinner(win, -player);
    existChild = 0;

    int freeEdgeNum = win.GetFreeEdgeNum();
    if (freeEdgeNum >= filterRange)
        totalChild = GetFreeMoves(nodeMoves);		//��ǰ�����ܽڵ���
    else
        totalChild = GetFilterMoves(nodeMoves);		//��ǰ�����ܽڵ���
}

float UCTNode::RefreshAvgValue()
{
    float value = 0;
    for (int i = 0; i < existChild; i++)
    {
        value += childNodes[i]->avgValue;
    }
    avgValue += 1 - value / existChild * 1.0;
    return avgValue;
}

float UCTNode::RefreshAvgValueWithSolver()
{
    float value = 0;
    //���ȼ���ƽ������
    bool fail = true;
    for (int i = 0; i < existChild; i++)
    {
        if (childNodes[i]->boardWinner != -owner)
        {
            fail = false;
            break;
        }
    }
    if (fail)
    {
        boardWinner = -owner;
        avgValue = 1;
        return 1;
    }
    int vaildChildNum = existChild;
    for (int i = 0; i < existChild; i++)
    {
        if (childNodes[i]->boardWinner == owner)//�������һ���ӽڵ������Ϊ1
        {
            boardWinner = owner;	//��ô����ڵ��ʤ���߾�����������
            avgValue = 0;			//��ô���ڸ��ڵ���˵���ڵ������Ϊ0
            return 0;
        }
        if (childNodes[i]->boardWinner == -owner)
            vaildChildNum--;
        else
            value += childNodes[i]->avgValue;
    }
    avgValue = 1 - (value / vaildChildNum);//��ǰ�ڵ��ƽ������ض���(1 - ��Ч�ӽڵ��ƽ������ )
    return avgValue;
}

float UCTNode::GetUCBValue(int total)
{
    return avgValue + sqrt((((log10((float)total)) * 2) / (float)visitTimes));
}

float UCTNode::GetUCBRaveValue(int Total, coor Move, std::vector<std::vector<int>> raveTable)
{
    float k = 100;
    float UCB = avgValue + sqrt((((log10((float)Total)) * 2) / (float)visitTimes));
    float RAVE = (float)raveTable[Move.x][Move.y];
    float ravek = sqrt(k / (3 * Total) + k);
    float ucbk = 1 - ravek;
    float final = ravek * RAVE + ucbk * UCB;
    return final;
}

UCTNode* UCTNode::ExpandUCTNode(int MCTimes, int filterRange)
{
    //�����½ڵ�
    Board CB(board, step);
    CB.TryMove(nodeMoves[existChild], owner);
    UCTNode* NewB = new UCTNode(-owner, CB.board, CB.step, true, filterRange);

    //��һ��MC����
    CB.SetBoard(NewB->board);
    NewB->avgValue = GetFilterMCEvalution(CB, -owner, owner, MCTimes, filterRange);
    return NewB;//����NewB�ĵ�ַ
}

UCTNode* UCTNode::ExpandUCTNodeRave(int MCTimes, int filterRange, std::vector<std::vector<int>> raveTable)
{
    //�����½ڵ�
    Board CB(board, step);
    CB.TryMove(nodeMoves[existChild], owner);
    UCTNode* NewB = new UCTNode(-owner, CB.board, CB.step, true, filterRange);

    //��һ��MC����
    CB.SetBoard(NewB->board);
    NewB->avgValue = GetRaveFilterMCEvalution(CB, -owner, owner, MCTimes, filterRange, raveTable);
    return NewB;//����NewB�ĵ�ַ
}

float UCTProcess(UCTNode& B, int& total, int MCTimes, int filterRange)
{
    B.visitTimes++;             //���ʵĴ�������һ��
    if (B.boardWinner != 0)     //�����Ϸ�Ѿ�������
    {
        if (B.boardWinner == B.owner)B.avgValue = 0;//���������ڵ���Ϸ�����ˣ��ж����档
        else B.avgValue = 1;                        //���������ڵ���Ϸ�����ˣ��ж�����
        total++;           //�����������Ϸ�����Ľڵ㣬�򱾴ε���������
        return B.avgValue;
    }
    if (B.existChild < B.totalChild)//�������δ���Թ��Ľڵ�
    {
        total++;//��׼���Σ����ε������������Դ���+1��
        B.childNodes[B.existChild] = B.ExpandUCTNode(MCTimes, filterRange);//��չһ���ӽڵ�
        B.existChild++;         //�ӽڵ����Ŀ����1
        B.RefreshAvgValue();    //ˢ������
        return B.avgValue;
    }
    else//˵��û��δ���Թ��Ľڵ�
    {
        int BestNodeNum = 0;
        double BestUCBValue = 0;
        double UCBValue[MOVENUM];
        //double* UCBValue = new double[(B.n / 2) * (B.n / 2 + 1)];
        for (int i = 0; i < B.totalChild; i++)
        {
            UCBValue[i] = B.childNodes[i]->GetUCBValue(total);
            if (UCBValue[i] >= BestUCBValue)
            {
                BestNodeNum = i;
                BestUCBValue = UCBValue[i];
            }
        }
        UCTProcess(*B.childNodes[BestNodeNum], total, MCTimes, filterRange);
        B.RefreshAvgValue();
        return B.avgValue;
    }
    return 0;
}

void LatterSituationMove(Board& CB, int player)
{
    //CB.GetAllCTypeBoxes(player, true);
    //�����㷨����ʱֻ�г����ͻ���
    if (CB.IsLongCTypeBoxExist())
    {
        //���д򿪵ĳ��������������벻����֮�������״̬�����Ƿ�������
        boxboard Dead(CB);
        int sacrificeBoxNum = 0;        //��������
        if (Dead.IsDeadChainExist())sacrificeBoxNum = 2;
        if (Dead.IsDeadCircleExist())sacrificeBoxNum = 4;

        //����ȫ�����Ե���
        Dead.GetAllCTypeBoxes(player);
        coor BoxNum = Dead.GetRationalStateBoxNum();//x�����ֵģ�y�Ǻ��ֵ�

        //���ڸ��ݽ����������ܵõ��ĸ����������з���

        //�����ڵ�ǰ��ȫ���������ֿ����õ�x�������ֿ����õ�y�����ҷ�ȫ�Ժ�����õ�y+n���Է��õ�x������x-y<n���ҷ�ȫ��

        if (BoxNum.x - BoxNum.y <= sacrificeBoxNum || Dead.Winner() != 0)
        {
            CB.GetAllCTypeBoxes(player, true);
            if (Dead.Winner() == 0)LatterSituationMove(CB, player);
        }
        else
        {
            //��������ʱ����������������
            coor DCMove;
            if (sacrificeBoxNum == 2)//Ȼ��������
            {
                //���ȳԵ�̰�����ٽ��
                for (;;)
                {
                    Board Test = CB;
                    Test.GetCTypeBox(player);
                    boxboard Dead(Test);
                    if (Dead.IsDeadChainExist())CB.GetCTypeBox(player, true);
                    else break;
                }
                //Ȼ��ʼ����DoubleCross
                DCMove = CB.GetDoubleCrossLoc(player);
            }
            else
            {
                //���ȳԵ�̰�����ٽ��
                for (;;)
                {
                    Board Test = CB;
                    Test.GetCTypeBox(player);
                    boxboard Dead(Test);
                    if (Dead.IsDeadCircleExist())CB.GetCTypeBox(player,true);
                    else break;
                }
                //Ȼ��ʼ����DoubleCross
                DCMove = CB.GetDoubleCrossLoc(player);
            }

            CB.TryMove(DCMove, player, true);
            for (;;)//ֱ���޷�ռ��CTypeBox�˾ͽ���
            {
                if (!CB.GetCTypeBoxLimit(player, true)) break;
            }
            //MoveMsg(DCMove.x, DCMove.y, player);
            //��������
        }

    }
    else
    {
        //ѡ�����һ����������������״̬�����Ǵ���̵ĳ����������
        CB.GetAllCTypeBoxes(player, true);
        boxboard Test(CB);
        if (Test.IsRationalState(Test.GetRationalStateBoxNum()))
        {
            //��������Ծ���Ļ�,���δ���
            coor M = Test.GetOpenSuitableChainLoc();
            CB.TryMove(M, player, true);
        }
        else
        {
            //����������Ծ���Ļ�������̵���
            //��������Ծ���Ļ�
            coor M = Test.GetOpenShortestChainLoc();
            CB.TryMove(M, player, true);
        }
    }
}

void UCTMoveWithSacrifice(Board& CB, int player)
{
    //coor pl;
    //int player1;

    boxboard dead(CB);

    bool isDeadChainExist = dead.IsDeadChainExist();
    bool isDeadCircleExist = dead.IsDeadCircleExist();

    if (isDeadChainExist || isDeadCircleExist)
    {
        coor DCMove;
        if (isDeadChainExist)   //
        {
            while (1)    //ռ��DeadChain���γ�˫��
            {
                Board testBoard = CB;
                testBoard.GetCTypeBox(player);  //����ռ�쳤��������
                boxboard dead1(testBoard);
                if (dead1.IsDeadChainExist())   //���ռ�������Ȼ���ڣ�����
                {
                    CB.GetCTypeBox(player,true);
                    //�ǵøı亯����move�������
                }
                else break;
            }
            DCMove = CB.GetDoubleCrossLoc(player);  //�����γ�˫��������
        }
        else
        {
            while (1)    //ռ��DeadCircle���γ�˫��
            {
                Board testBoard = CB;
                testBoard.GetCTypeBox(player);  //����ռ�쳤��������
                boxboard dead1(testBoard);
                if (dead1.IsDeadCircleExist())   //���ռ�������Ȼ���ڣ�����
                {
                    CB.GetCTypeBox(player, true);
                    //�ǵøı亯����move�������
                }
                else break;
            }
            DCMove = CB.GetDoubleCrossLoc(player);  //�����γ�˫��������
        }

        UCTNode greedyChild = UCTNode(player, CB.board, CB.step, true, UCT_FILTER_RANGE);		//���ݵ�ǰ���洴��̰��UCT�ĸ��ڵ�
        UCTNode sacrificeChild = UCTNode(-player, CB.board, CB.step, false, UCT_FILTER_RANGE);	//���ݵ�ǰ���洴������UCT�ĸ��ڵ�


        sacrificeChild.TryMove(DCMove, player);	//�������ڵ��γ�˫��
        while (1)       //ռ������CBox
        {
            if (!sacrificeChild.GetCTypeBox(player))break;
        }
        //��ʱ��Sacrifice��׼�������ľ�����
        sacrificeChild.totalChild = sacrificeChild.GetFilterMoves(sacrificeChild.nodeMoves);	//�õ����˿��б�
        Board Win(sacrificeChild.board, sacrificeChild.step);		//������ǰ������жϵ�
        sacrificeChild.boardWinner = GetBoardWinner(Win, player);	//��������ʤ���ߣ�ע���ʱ�������ǶԷ�
        if (sacrificeChild.boardWinner == 0)
        {
            //
            int Total = 0;//UCT�Ĵ�������
            int Total2 = 0;
            clock_t start;	//���ü�ʱ���ı���
            start = clock();
            for (int i = 0; i < UCT_TIMES / 2; i++)//����һ������
            {
                UCTProcess(sacrificeChild, Total, UCT_MC_TIMES, UCT_FILTER_RANGE);
                UCTProcess(greedyChild, Total2, UCT_MC_TIMES, UCT_FILTER_RANGE);
                //cout<<"UCT Iteration have finished "<<i+1<<" times"<<endl;
                double totaltime = (double)(clock() - start) / CLOCKS_PER_SEC;
                if (totaltime >= UCT_LIMIT_TIME)
                    break;
            }
            //

            //�ж��������
            int BestNodeNum = 0;
            float BestAvgValue = 0;
            for (int i = 0; i < greedyChild.existChild; i++)
            {
                if (greedyChild.childNodes[i]->avgValue >= BestAvgValue)
                {
                    BestNodeNum = i;
                    BestAvgValue = greedyChild.childNodes[i]->avgValue;
                }
            }

            //��̰��
            if (BestAvgValue >= sacrificeChild.avgValue)
            {
                CB.GetAllCTypeBoxes(player, true);
                // greedyChild.nodeMoves[BestNodeNum];
                //player1 = player;
                CB.TryMove(greedyChild.nodeMoves[BestNodeNum], player, true);
                //if (Msg)
                //    MoveMsg(GreedyChild.NodeMoves[BestNodeNum].x, GreedyChild.NodeMoves[BestNodeNum].y, Player);
            }
            //ʹ������
            else
            {
                //pl = DCMove;
                CB.TryMove(DCMove, player, true);
                for (;;)//ֱ���޷�ռ��CTypeBox�˾ͽ���
                {
                    if (!CB.GetCTypeBoxLimit(player, true))
                        break;
                }
                //if (Msg)
                //    MoveMsg(DCMove.x, DCMove.y, Player);
            }
            DeleteUCTTree(greedyChild);
            DeleteUCTTree(sacrificeChild);
        }
    }
    else
    {
       UCTMove(CB, player);
    }

    //return pl;
}

void UCTMove(Board& CB, int player)
{
    //coor pl;
    CB.GetAllCTypeBoxes(player, true);
    UCTNode UCTB = UCTNode(player, CB.board, CB.step, true, UCT_FILTER_RANGE);//���ݵ�ǰ���洴��UCT�ĸ��ڵ�
    if (UCTB.boardWinner == 0)
    {
        int total = 0;//UCT�Ĵ�������
        clock_t start;	//���ü�ʱ���ı���
        start = clock();
        for (int i = 0; i < UCT_TIMES; i++)//����һ������
        {
            UCTProcess(UCTB, total, UCT_MC_TIMES, UCT_FILTER_RANGE);
            //printf("%d\n", i);
            //cout<<"UCT Iteration have finished "<<i+1<<" times"<<endl;
            double totaltime = (double)(clock() - start) / CLOCKS_PER_SEC;
            if (totaltime >= UCT_LIMIT_TIME)break;
        }
        //for (int i = 0; i < 60; i++)
        //{
        //    UCTProcess(UCTB, total, UCT_MC_TIMES, UCT_FILTER_RANGE);
        //    printf("%d\n", i);
        //}
        //�ж��������
        int BestNodeNum = 0;
        float BestAvgValue = 0;
        int LargerTimesNodeNum = 0;
        int LargerTimesValue = 0;
        for (int i = 0; i < UCTB.existChild; i++)
        {
            if (UCTB.childNodes[i]->avgValue >= BestAvgValue)
            {
                BestNodeNum = i;
                BestAvgValue = UCTB.childNodes[i]->avgValue;
            }
            if (UCTB.childNodes[i]->visitTimes >= LargerTimesValue)
            {
                LargerTimesNodeNum = i;
                LargerTimesValue = UCTB.childNodes[i]->visitTimes;
            }
        }

        //pl = UCTB.nodeMoves[BestNodeNum];
        CB.TryMove(UCTB.nodeMoves[BestNodeNum], player, true);

        //�ͷ��ڴ�
        DeleteUCTTree(UCTB);
    }
    else
    {
        CB.GetAllCTypeBoxes(player);
        ////using evalution function
        ////cout << "then using evalution function" << endl;
        LatterSituationMove(CB, player);
    }

    //return pl;
}

void GameTurnMove(Board& CB, int player)
{
    Board testBoard = CB;
    testBoard.GetAllCTypeBoxes(player);
    bool isLatterSituation = testBoard.GetFilterMoveNum() == 0;      //�Ƿ�Ϊ���ھ���

    if (isLatterSituation)
    {
        LatterSituationMove(CB, player);    //�����㷨
    }
    else
    {
        UCTMoveWithSacrifice(CB, player);   //ǰ���㷨
    }
}

void DeleteUCTNode(UCTNode* root)
{
    if (root->existChild > 0)
    {
        for (int i = 0; i < root->existChild; i++)
        {
            DeleteUCTNode(root->childNodes[i]);
            delete root->childNodes[i];
        }
    }
}

void DeleteUCTTree(UCTNode root)
{
    for (int i = 0; i < root.existChild; i++)
    {
        DeleteUCTNode(root.childNodes[i]);
        delete root.childNodes[i];
    }
}

//ǰ��˫������������壬�õ�ǰ�ڵ�Ӯ��
int GetBoardWinner(Board& CB, int latterPlayer)
{
    std::vector<coor>a;
    if (CB.GetFilterMoves(a) != 0)return 0;
    boxboard advanced(CB);
    int w = advanced.GetBoardWinner(latterPlayer);
    return w;
}

//���ռ��һ�����б�
void RndTurn(Board& CB, int player)
{
    std::vector<coor> moves;
    CB.GetAllCTypeBoxes(player);
    int moveNum = CB.GetAllMoves(moves);//ȷ�����������TurnNum������
    if (moveNum == 0)return;
    int rnd = rand() % moveNum;//��0-MoveNum�г�ȡһ�������
    CB.TryMove(moves[rnd], player);
}

//��������
int GetMCWinner(Board& CB, int nextPlayer)
{
    int player = nextPlayer;
    //while (CB.GetFreeEdgeNum() != 0)//�����������ɱߵ�ʱ��
    //{
    //    RndTurn(CB, player);
    //    player = -player;
    //}
    std::vector<coor>a;
    while (CB.GetFilterMoves(a,true) != 0)//�����������ɱߵ�ʱ��
    {
        RndTurn(CB, player);
        player = -player;
    }
    int W = GetBoardWinner(CB, -player);
    return W;
}

float GetFilterMCEvalution(Board& CB, int nextPlayer, int winner, int TIMES, int filterRange)
{
    Board MCB = CB;//�ȸ���һ������
    int MCE = 0;
    for (int i = 0; i < TIMES; i++)
    {
        if (GetFilterMCWinner(MCB, nextPlayer,filterRange) == winner)
        {
            MCE++;
        }
    }
    float score = ((float)MCE) / ((float)TIMES);
    return score;
}

int RndFilterTurn(Board& CB, int player, int filterRange)
{
    std::vector<coor>moves;
    CB.GetAllCTypeBoxes(player);

    int moveNum;
    int FreeEdge = CB.GetFreeEdgeNum();
    if (FreeEdge < filterRange)//����FreeEdge����С��25������¿���Filter
        //moveNum = CB.GetFreeMoves(moves);
        moveNum = CB.GetFilterMoves(moves, true);//ȷ�����������MoveNum������
    else
        moveNum = CB.GetFreeMoves(moves);//ȷ�����������MoveNum������

    if (moveNum != 0)//��ĳЩʱ�����ڳԵ���ǰ���C�͸񡣿��ܵ���MoveNum������Ϊ0.��ʱ��ֻҪ������һ����Ȼ�ͻῪʼ�ж�ʤ����
    {
        srand((unsigned)time(NULL));
        int rnd = rand() % moveNum;//��0-MoveNum�г�ȡһ�������
        int boxNum = CB.GetEmptyBoxes();
        CB.TryMove(moves[rnd], player);
        if (CB.GetEmptyBoxes() == boxNum)
            return -player;//����
        else return player;
    }
    else
    {
        return player;//������
    }
}

int GetFilterMCWinner(Board& CB, int NextPlayer, int filterRange)
{
    int player = NextPlayer;
    //while (CB.GetFreeEdgeNum() != 0)//�����������ɱߵ�ʱ��
    //{
    //    player = RndFilterTurn(CB, player, filterRange);
    //    //CB.ShowBoard();
    //    //system("pause");
    //}
    while (CB.GetFilterMoveNum() != 0)//�����������ɱߵ�ʱ��
    {
        player = RndFilterTurn(CB, player, filterRange);
        //CB.ShowBoard();
        //system("pause");
    }
    //CB.ShowBoard();
    //cout << "Finish" << endl;
    //int W = GetBoardWinner(CB, -player);
    boxboard advanced(CB);
    int w = advanced.GetBoardWinner(-player);
    return w;
    //return W;
}

float GetRaveFilterMCEvalution(Board& CB, int nextPlayer, int winner, int TIMES, int filterRange, std::vector<std::vector<int>> raveTable)
{
    Board MCB = CB;//�ȸ���һ������
    int MCE = 0;
    for (int i = 0; i < TIMES; i++)
    {
        if (GetFilterMCWinner(MCB, nextPlayer, filterRange) == winner)
        {
            MCE++;
            for (int y = 0; y < CB.boardSize; y++)
            {
                for (int x = 0; x < 0; x++)
                {
                    if (MCB.board[x][y] == winner)
                        raveTable[x][y]++;
                }
            }
        }
    }
    float score = ((float)MCE) / ((float)TIMES);
    //MCB.ShowBoard();
    //cout << "This one score is " << score << endl;
    return score;
}
