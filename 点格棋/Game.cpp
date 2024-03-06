#include"Game.h"

Game::Game(int p1, int p2, int n,bool isGetRecord)
{
	if (isGetRecord)
	{
		std::fstream record;
		record.open("boardSave\\boardSave.txt", std::ios::in);
		char buf[20] = { 0 };
		record >> buf;
		P1 = buf[0] - '0';
		record >> buf;
		P2 = buf[0] - '0';
		if (P1)
		{
			player1 = new yuchunderen();
		}
		else
		{
			player1 = new AI();
		}
		if (P2)
		{
			player2 = new yuchunderen();
		}
		else
		{
			player2 = new AI();
		}
		record >> buf;
		turn = buf[0] - '0';
		record >> buf;
		num = (buf[0] - '0') * 2 + 1;
		board.resize(num);
		for (int i = 0; i < num; i++)
		{
			board[i].resize(num);
			for (int j = 0; j < num; j++)
			{
				record >> buf;
				if (buf[0] == '-')board[i][j] = (buf[1] - '0') * -1;
				else board[i][j] = buf[0] - '0';
			}
		}
		record.close();
		//ShowMessage();
		Recover();
		UI = new ui(num / 2);
		UI->GetMap(board, coor(-1, -1));
		UI->GetScore(player1->score, player2->score);
		UI->GetTurn(turn);
		UI->Update();
		return;
	}
	nmsl = 0;
	turn = 1;
	isExitError = true;
	p = { 0,0 };
	UI = new ui(n);
	P1 = p1;
	P2 = p2;
	if (p1)
	{
		player1 = new yuchunderen();
	}
	else
	{
		player1 = new AI();
	}
	if (p2)
	{
		player2 = new yuchunderen();
	}
	else
	{
		player2 = new AI();
	}
	

	//num = n * 2 + 1;
	num = 2 * n + 1;
	std::vector<int>temp(num);
	std::vector<std::vector<int>>Ad(num, temp);
	board = Ad;
	//for (int i = 0; i < num; i++)
	//{
	//	Board[0][i] = Board[num - 1][i] = Board[i][0] = Board[i][num - 1] = 1;

	//}
	for (int i = 0; i < num; i += 2)
	{
		for (int j = 0; j < num; j += 2)
		{
			board[i][j] = -1;
		}
	}
	ShowMessage();
}

void Game::ShowMessage()
{	
	system("cls");
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < num; j++)
		{
			//if (Board[i][j] == -1)printf("点");
			//else
				printf("%2d ", board[i][j]);
		}
		printf("\n");
	}
	printf("player1:%-5d  player2:%-5d\nnow：", player1->score, player2->score);
	if (turn % 2)printf("player1\n");
	else printf("player2\n");
}

void Game::GetMessageA()
{
	//rule.getmap(board);

	//向UI发送棋盘信息
	UI->GetMap(board, p);


	//判断行棋玩家
	if (turn % 2)player = player1;
	else player = player2;

	//玩家行棋，返回坐标p
	//p--普通坐标，114坐标
	player->get(board, UI, pow(-1, turn + 1));
	p = player->act();


	//if (step % 2)player1->score += s;
	//else player2->score += s;
}

int Game::GetBoxOwner(int x,int y)
{
	int flag = 0;
	if (turn%2)flag = 1;
	else flag = -1;
	if (board[x + 1][y] && board[x - 1][y] && board[x][y + 1] && board[x][y - 1])return flag;
	else return 0;
}

void Game::Move()
{
	board[p.x][p.y] = 1;
	int s = 0;
	int color;
	if (turn % 2)color = 1;
	else color = -1;
	if (p.x % 2)
	{
		if (p.y == 0)
		{
			board[p.x][p.y + 1] = GetBoxOwner(p.x, p.y + 1);
			s += board[p.x][p.y + 1];
		}
		else if (p.y == num - 1)
		{
			board[p.x][p.y - 1] = GetBoxOwner(p.x, p.y - 1);
			s += board[p.x][p.y - 1];
		}
		else
		{
			board[p.x][p.y + 1] = GetBoxOwner(p.x, p.y + 1);
			board[p.x][p.y - 1] = GetBoxOwner(p.x, p.y - 1);
			s += board[p.x][p.y + 1] + board[p.x][p.y - 1];
		}
	}
	else
	{
		if (p.x == 0)
		{
			board[p.x + 1][p.y] = GetBoxOwner(p.x + 1, p.y);
			s += board[p.x + 1][p.y];
		}
		else if (p.x == num - 1)
		{
			board[p.x - 1][p.y] = GetBoxOwner(p.x - 1, p.y);
			s += board[p.x - 1][p.y];
		}
		else 
		{
			board[p.x - 1][p.y] = GetBoxOwner(p.x - 1, p.y);
			board[p.x + 1][p.y] = GetBoxOwner(p.x + 1, p.y);
			s += board[p.x - 1][p.y] + board[p.x + 1][p.y];
		}
	}

	undonode p1;
	p1.p = p;
	if (s == 0)turn = (turn + 1) % 2;
	if (turn % 2)
	{
		player1->score += abs(s);
		p1.player = 1;
	}
	else 
	{
		player2->score += abs(s); 
		p1.player = 2;
	}
	step.push(p1);
}

bool Game::IsEdgeLegality(coor)
{
	int x = p.x; int y = p.y;
	if ((x + y) % 2 == 0 ||board[x][y] > 0)return false;
	return true;
}

bool Game::GameRun()
{
	int k = GetWinner();
	if (k)
	{
		UI->Winner(k);
		return 1;
	}
	GetMessageA();
	if (p.x == 114)
	{
		if (p.y == 0)
		{
			Undo();
		}
		else if (p.y == 1)
		{
			UI->ShuNm();
		}
		else if (p.y == 2)
		{
			return Quit();
		}
	}
	else
	{
		if (IsEdgeLegality(p))
		{
			//nmsl = 0;
			Move();
			//u.updata();
			//Draw();
			UI->GetMap(board, p);
			UI->GetScore(player1->score, player2->score);
			UI->GetTurn(turn);
			//printf("%d %d\n", player1->score, player2->score);
			UI->Update();
			UI->SaveImage();
			SaveBoard();
			//Sleep(500);
		}
	}
	return false;
}

void Game::Undo()
{
	//turn = (turn + 1) % 2;
	if (step.empty())return;
	undonode n = step.top(),n1;
	int pl = n.player;
	int flag = 0;
	while (1)
	{
		if (step.empty())break;
		n = step.top();
		if (pl != n.player)flag = 1;
		if (pl == n.player && flag)break;
		step.pop();
		if (!step.empty())
		{
			n1 = step.top();
		}
		else
		{
			n1.p = { -1,514 };
		}
		coor p = n.p;
		int s = 0;
		if (p.x % 2)
		{
			if (p.y == 0)
			{
				board[p.x][p.y + 1] = GetBoxOwner(p.x, p.y + 1);
				s += board[p.x][p.y + 1];
			}
			else if (p.y == num - 1)
			{
				board[p.x][p.y - 1] = GetBoxOwner(p.x, p.y - 1);
				s += board[p.x][p.y - 1];
			}
			else
			{
				s += board[p.x][p.y + 1] + board[p.x][p.y - 1];
				board[p.x][p.y + 1] = 0;
				board[p.x][p.y - 1] = 0;
				
			}
		}
		else
		{
			if (p.x == 0)
			{
				s += board[p.x + 1][p.y];
				board[p.x + 1][p.y] = 0;
				
			}
			else if (p.x == num - 1)
			{
				s += board[p.x - 1][p.y];
				board[p.x - 1][p.y] = 0;
				
			}
			else
			{
				s += board[p.x - 1][p.y] + board[p.x + 1][p.y];
				board[p.x - 1][p.y] = 0;
				board[p.x + 1][p.y] = 0;
				
			}
		}
		board[p.x][p.y] = 0;
		//rule.getmap(board);
		if (s == 0)turn = (turn + 1) % 2;
		else
		{
			if (turn % 2)player1->score -= abs(s);
			else player2->score -= abs(s);
		}
		coor p2 = n1.p;
		p2.x += 514;
		UI->GetMap(board, p2);
		UI->GetScore(player1->score, player2->score);
		UI->Update();
		Sleep(500);
	}
	p = n1.p;
}

bool Game::Quit()
{
	return true;
}

int Game::GetWinner()
{
	int n = num / 2;
	if (player1->score > n * n / 2)
	{
		return 1;
	}
	else if (player2->score > n * n / 2)
	{
		return 2;
	}
	else return 0;
}

void Game::SaveBoard()
{
	std::fstream boardSave;
	boardSave.open("boardSave\\boardSave.txt",std::ios::out);
	boardSave << P1 << " " << P2 << " " << turn << " " << num / 2 << std::endl;
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < num; j++)
		{
			boardSave << board[i][j] << " ";
		}
		boardSave << std::endl;
	}
	boardSave << std::endl;
	boardSave.close();
}

void Game::Recover()
{
	for (int i = 1; i < num; i += 2)
	{
		for (int j = 1; j < num; j += 2)
		{
			if (board[i][j] == 1)player1->score++;
			else if (board[i][j] == -1)player2->score++;
		}
	}
}
