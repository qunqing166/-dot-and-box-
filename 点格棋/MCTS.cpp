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

    //如果有死格，占领先
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
        totalChild = GetFreeMoves(nodeMoves);		//提前计算总节点数
    else
    { 
        //totalChild = GetFreeMoves(nodeMoves);		//提前计算总节点数
        totalChild = GetFilterMoves(nodeMoves,true);		//提前计算总节点数
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
        totalChild = GetFreeMoves(nodeMoves);		//提前计算总节点数
    else
        totalChild = GetFilterMoves(nodeMoves);		//提前计算总节点数
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
    //首先计算平均收益
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
        if (childNodes[i]->boardWinner == owner)//如果任意一个子节点的收益为1
        {
            boardWinner = owner;	//那么这个节点的胜利者就是其所有者
            avgValue = 0;			//那么对于父节点来说本节点的收益为0
            return 0;
        }
        if (childNodes[i]->boardWinner == -owner)
            vaildChildNum--;
        else
            value += childNodes[i]->avgValue;
    }
    avgValue = 1 - (value / vaildChildNum);//当前节点的平均收益必定是(1 - 有效子节点的平均收益 )
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
    //创建新节点
    Board CB(board, step);
    CB.TryMove(nodeMoves[existChild], owner);
    UCTNode* NewB = new UCTNode(-owner, CB.board, CB.step, true, filterRange);

    //做一次MC评估
    CB.SetBoard(NewB->board);
    NewB->avgValue = GetFilterMCEvalution(CB, -owner, owner, MCTimes, filterRange);
    return NewB;//返回NewB的地址
}

UCTNode* UCTNode::ExpandUCTNodeRave(int MCTimes, int filterRange, std::vector<std::vector<int>> raveTable)
{
    //创建新节点
    Board CB(board, step);
    CB.TryMove(nodeMoves[existChild], owner);
    UCTNode* NewB = new UCTNode(-owner, CB.board, CB.step, true, filterRange);

    //做一次MC评估
    CB.SetBoard(NewB->board);
    NewB->avgValue = GetRaveFilterMCEvalution(CB, -owner, owner, MCTimes, filterRange, raveTable);
    return NewB;//返回NewB的地址
}

float UCTProcess(UCTNode& B, int& total, int MCTimes, int filterRange)
{
    B.visitTimes++;             //访问的次数增加一次
    if (B.boardWinner != 0)     //如果游戏已经结束了
    {
        if (B.boardWinner == B.owner)B.avgValue = 0;//如果在这个节点游戏结束了，判定收益。
        else B.avgValue = 1;                        //如果在这个节点游戏结束了，判定收益
        total++;           //如果搜索到游戏结束的节点，则本次迭代结束。
        return B.avgValue;
    }
    if (B.existChild < B.totalChild)//如果还有未尝试过的节点
    {
        total++;//基准情形，本次迭代结束，尝试次数+1。
        B.childNodes[B.existChild] = B.ExpandUCTNode(MCTimes, filterRange);//扩展一个子节点
        B.existChild++;         //子节点的数目自增1
        B.RefreshAvgValue();    //刷新收益
        return B.avgValue;
    }
    else//说明没有未尝试过的节点
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
    //后期算法，此时只有长链和环。
    if (CB.IsLongCTypeBoxExist())
    {
        //已有打开的长链，根据牺牲与不牺牲之后的理性状态决定是否牺牲。
        boxboard Dead(CB);
        int sacrificeBoxNum = 0;        //牺牲数量
        if (Dead.IsDeadChainExist())sacrificeBoxNum = 2;
        if (Dead.IsDeadCircleExist())sacrificeBoxNum = 4;

        //假设全部都吃掉了
        Dead.GetAllCTypeBoxes(player);
        coor BoxNum = Dead.GetRationalStateBoxNum();//x是先手的，y是后手的

        //现在根据接下来局面能得到的格子数来进行分析

        //假设在当前链全被消灭后后手可以拿到x个，先手可以拿到y个。我方全吃后可以拿到y+n，对方拿到x个。若x-y<n则我方全吃

        if (BoxNum.x - BoxNum.y <= sacrificeBoxNum || Dead.Winner() != 0)
        {
            CB.GetAllCTypeBoxes(player, true);
            if (Dead.Winner() == 0)LatterSituationMove(CB, player);
        }
        else
        {
            //牺牲，此时必有死长链或死环
            coor DCMove;
            if (sacrificeBoxNum == 2)//然后处理死链
            {
                //首先吃到贪婪的临界点
                for (;;)
                {
                    Board Test = CB;
                    Test.GetCTypeBox(player);
                    boxboard Dead(Test);
                    if (Dead.IsDeadChainExist())CB.GetCTypeBox(player, true);
                    else break;
                }
                //然后开始考虑DoubleCross
                DCMove = CB.GetDoubleCrossLoc(player);
            }
            else
            {
                //首先吃到贪婪的临界点
                for (;;)
                {
                    Board Test = CB;
                    Test.GetCTypeBox(player);
                    boxboard Dead(Test);
                    if (Dead.IsDeadCircleExist())CB.GetCTypeBox(player,true);
                    else break;
                }
                //然后开始考虑DoubleCross
                DCMove = CB.GetDoubleCrossLoc(player);
            }

            CB.TryMove(DCMove, player, true);
            for (;;)//直到无法占据CTypeBox了就结束
            {
                if (!CB.GetCTypeBoxLimit(player, true)) break;
            }
            //MoveMsg(DCMove.x, DCMove.y, player);
            //牺牲结束
        }

    }
    else
    {
        //选择打开哪一条长链。根据理性状态决定是打开最短的长链还是如何
        CB.GetAllCTypeBoxes(player, true);
        boxboard Test(CB);
        if (Test.IsRationalState(Test.GetRationalStateBoxNum()))
        {
            //如果是理性局面的话,依次打开链
            coor M = Test.GetOpenSuitableChainLoc();
            CB.TryMove(M, player, true);
        }
        else
        {
            //如果不是理性局面的话，打开最短的链
            //如果是理性局面的话
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
            while (1)    //占领DeadChain到形成双交
            {
                Board testBoard = CB;
                testBoard.GetCTypeBox(player);  //尝试占领长链的死格
                boxboard dead1(testBoard);
                if (dead1.IsDeadChainExist())   //如果占领后长链仍然存在，行棋
                {
                    CB.GetCTypeBox(player,true);
                    //记得改变函数把move存入队列
                }
                else break;
            }
            DCMove = CB.GetDoubleCrossLoc(player);  //储存形成双交的坐标
        }
        else
        {
            while (1)    //占领DeadCircle到形成双交
            {
                Board testBoard = CB;
                testBoard.GetCTypeBox(player);  //尝试占领长链的死格
                boxboard dead1(testBoard);
                if (dead1.IsDeadCircleExist())   //如果占领后长链仍然存在，行棋
                {
                    CB.GetCTypeBox(player, true);
                    //记得改变函数把move存入队列
                }
                else break;
            }
            DCMove = CB.GetDoubleCrossLoc(player);  //储存形成双交的坐标
        }

        UCTNode greedyChild = UCTNode(player, CB.board, CB.step, true, UCT_FILTER_RANGE);		//根据当前局面创建贪婪UCT的根节点
        UCTNode sacrificeChild = UCTNode(-player, CB.board, CB.step, false, UCT_FILTER_RANGE);	//根据当前局面创建牺牲UCT的根节点


        sacrificeChild.TryMove(DCMove, player);	//在牺牲节点形成双交
        while (1)       //占领所有CBox
        {
            if (!sacrificeChild.GetCTypeBox(player))break;
        }
        //此时的Sacrifice是准备牺牲的局面了
        sacrificeChild.totalChild = sacrificeChild.GetFilterMoves(sacrificeChild.nodeMoves);	//得到过滤可行边
        Board Win(sacrificeChild.board, sacrificeChild.step);		//创建当前局面的判断点
        sacrificeChild.boardWinner = GetBoardWinner(Win, player);	//这个局面的胜利者，注意此时的先手是对方
        if (sacrificeChild.boardWinner == 0)
        {
            //
            int Total = 0;//UCT的次数函数
            int Total2 = 0;
            clock_t start;	//设置计时器的变量
            start = clock();
            for (int i = 0; i < UCT_TIMES / 2; i++)//迭代一定次数
            {
                UCTProcess(sacrificeChild, Total, UCT_MC_TIMES, UCT_FILTER_RANGE);
                UCTProcess(greedyChild, Total2, UCT_MC_TIMES, UCT_FILTER_RANGE);
                //cout<<"UCT Iteration have finished "<<i+1<<" times"<<endl;
                double totaltime = (double)(clock() - start) / CLOCKS_PER_SEC;
                if (totaltime >= UCT_LIMIT_TIME)
                    break;
            }
            //

            //判定最佳收益
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

            //用贪婪
            if (BestAvgValue >= sacrificeChild.avgValue)
            {
                CB.GetAllCTypeBoxes(player, true);
                // greedyChild.nodeMoves[BestNodeNum];
                //player1 = player;
                CB.TryMove(greedyChild.nodeMoves[BestNodeNum], player, true);
                //if (Msg)
                //    MoveMsg(GreedyChild.NodeMoves[BestNodeNum].x, GreedyChild.NodeMoves[BestNodeNum].y, Player);
            }
            //使用牺牲
            else
            {
                //pl = DCMove;
                CB.TryMove(DCMove, player, true);
                for (;;)//直到无法占据CTypeBox了就结束
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
    UCTNode UCTB = UCTNode(player, CB.board, CB.step, true, UCT_FILTER_RANGE);//根据当前局面创建UCT的根节点
    if (UCTB.boardWinner == 0)
    {
        int total = 0;//UCT的次数函数
        clock_t start;	//设置计时器的变量
        start = clock();
        for (int i = 0; i < UCT_TIMES; i++)//迭代一定次数
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
        //判定最佳收益
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

        //释放内存
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
    bool isLatterSituation = testBoard.GetFilterMoveNum() == 0;      //是否为后期局面

    if (isLatterSituation)
    {
        LatterSituationMove(CB, player);    //后期算法
    }
    else
    {
        UCTMoveWithSacrifice(CB, player);   //前期算法
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

//前期双方轮流随机行棋，得到前期的赢家
int GetBoardWinner(Board& CB, int latterPlayer)
{
    std::vector<coor>a;
    if (CB.GetFilterMoves(a) != 0)return 0;
    boxboard advanced(CB);
    int w = advanced.GetBoardWinner(latterPlayer);
    return w;
}

//随机占领一条可行边
void RndTurn(Board& CB, int player)
{
    std::vector<coor> moves;
    CB.GetAllCTypeBoxes(player);
    int moveNum = CB.GetAllMoves(moves);//确定这个局面下TurnNum的数量
    if (moveNum == 0)return;
    int rnd = rand() % moveNum;//在0-MoveNum中抽取一个随机数
    CB.TryMove(moves[rnd], player);
}

//轮流行棋
int GetMCWinner(Board& CB, int nextPlayer)
{
    int player = nextPlayer;
    //while (CB.GetFreeEdgeNum() != 0)//当还存在自由边的时候
    //{
    //    RndTurn(CB, player);
    //    player = -player;
    //}
    std::vector<coor>a;
    while (CB.GetFilterMoves(a,true) != 0)//当还存在自由边的时候
    {
        RndTurn(CB, player);
        player = -player;
    }
    int W = GetBoardWinner(CB, -player);
    return W;
}

float GetFilterMCEvalution(Board& CB, int nextPlayer, int winner, int TIMES, int filterRange)
{
    Board MCB = CB;//先复制一个棋盘
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
    if (FreeEdge < filterRange)//仅在FreeEdge数量小于25的情况下考虑Filter
        //moveNum = CB.GetFreeMoves(moves);
        moveNum = CB.GetFilterMoves(moves, true);//确定这个局面下MoveNum的数量
    else
        moveNum = CB.GetFreeMoves(moves);//确定这个局面下MoveNum的数量

    if (moveNum != 0)//在某些时候，由于吃掉了前面的C型格。可能导致MoveNum的数量为0.这时候只要跳过这一步自然就会开始判断胜利。
    {
        srand((unsigned)time(NULL));
        int rnd = rand() % moveNum;//在0-MoveNum中抽取一个随机数
        int boxNum = CB.GetEmptyBoxes();
        CB.TryMove(moves[rnd], player);
        if (CB.GetEmptyBoxes() == boxNum)
            return -player;//换手
        else return player;
    }
    else
    {
        return player;//不换手
    }
}

int GetFilterMCWinner(Board& CB, int NextPlayer, int filterRange)
{
    int player = NextPlayer;
    //while (CB.GetFreeEdgeNum() != 0)//当还存在自由边的时候
    //{
    //    player = RndFilterTurn(CB, player, filterRange);
    //    //CB.ShowBoard();
    //    //system("pause");
    //}
    while (CB.GetFilterMoveNum() != 0)//当还存在自由边的时候
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
    Board MCB = CB;//先复制一个棋盘
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
