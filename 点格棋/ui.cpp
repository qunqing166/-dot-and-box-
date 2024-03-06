#include"ui.h"

ui::ui(int n)
{
	height = 600 + 300;
	width = 600;
	e_length = 20;
	rec_length = 70;
	for (int i = 0; i < 3; i++)ch[i] = 0;
	size = n;
	periphery = 20 + (6 - size) * (e_length + rec_length)/2;
	turn = 1;
	saveNum = 0;
	GetScore();

	std::vector<std::vector<int>>m(2 * size + 1, std::vector<int>(2 * size + 1));
	map = m;
	pm = { -1,-1 };
	pl = p1 = pm;
	//map[1][1] = -1;

	initgraph(height,width);

	RECT rect;
	HWND hwnd = GetHWnd();
	GetWindowRect(hwnd, &rect);
	MoveWindow(hwnd, 200, 100, rect.right - rect.left, rect.bottom - rect.top, true);

	setbkcolor(WHITE);
	cleardevice();
	Update();
}

void ui::GetMap(std::vector<std::vector<int>>v, coor p)
{
	//turn = turn1;
	map = v;
	if (p.x == -1 && p.y == -1)return;
	if (p.x < 114)
	{
		if (p.y == 514)
		{
			map[pl.x][pl.y] = 0;
			pl = { -1,-1 };
		}
		else
		{
			if (pl.x != -1)map[pl.x][pl.y] = 1;
			pl = p;
			map[pl.x][pl.y] = -2;
		}
	}
	else if (p.x >= 514)
	{
		p.x -= 514;
		if (pl.x != -1 && pl.y != -1)map[pl.x][pl.y] = 0;
		pl = p;
		if (pl.y != -1)map[pl.x][pl.y] = -2;
	}
}

void ui::GetTurn(int t)
{
	turn = t;
}

void ui::ShuNm()
{
	setbkmode(TRANSPARENT);
	settextstyle(100, 0, "楷体");
	settextcolor(RGB(255, 119, 119));
	outtextxy(50, 200, "认输nm呢,继续玩");
	Sleep(2000);
	cleardevice();
	Update();
}

void ui::Update()
{
	BeginBatchDraw();
	cleardevice();
	//setfillcolor(WHITE);
	//solidrectangle(0, 0, height, width);
	//setlinecolor(BLACK);
	setfillcolor(RGB(213, 213, 213));
	fillrectangle(periphery, periphery, width - periphery, width - periphery);

	for (int i = 0; i < 2 * size + 1; i++)
	{
		for (int j = 0; j < 2 * size + 1; j++)
		{
			if (i % 2 || j % 2)
			{
				int i1 = i / 2;
				int j1 = j / 2;
				if ((i + j) % 2 == 0)		//Box
				{
					if (map[i][j] == 1)setfillcolor(RGB(255, 119, 119));
					else if (map[i][j] == 0) setfillcolor(WHITE);
					else if (map[i][j] == -1)setfillcolor(RGB(203, 250, 255));
				}
				else                      //Edge 
				{
					if (map[i][j] == 1)setfillcolor(RGB(213, 213, 213));
					else if (map[i][j] == 0)setfillcolor(RGB(255, 241, 248));
					else if (map[i][j] == -1)setfillcolor(RGB(246, 238, 255));
					else if (map[i][j] == -2)setfillcolor(RGB(255, 200, 236));
				}
				if (i % 2)
				{
					if (j % 2 == 0)
					{
						solidrectangle(periphery + j1 * (e_length + rec_length), periphery + e_length + i1 * (e_length + rec_length), periphery + (j1) * (e_length + rec_length) + e_length-1, periphery + (i1 + 1) * (e_length + rec_length)-1);
					}
					else
					{
						solidrectangle(periphery + j1 * (e_length + rec_length) + e_length, periphery + e_length + i1 * (e_length + rec_length), periphery + (j1 + 1) * (e_length + rec_length), periphery + (i1 + 1) * (e_length + rec_length));
					}
				}
				else
				{
					solidrectangle(periphery + e_length + j1 * (e_length + rec_length), periphery + i1 * (e_length + rec_length), periphery + (1 + j1) * (e_length + rec_length)-1, periphery + i1 * (e_length + rec_length) + e_length-1);
				}
			}
		}
	}

	setfillcolor(RGB(255, 241, 248));
	solidroundrect(605, 5, 900-5, 200-5, 40, 40);
	
	setfillcolor(WHITE);
	//
	setbkmode(TRANSPARENT);
	//settextstyle(30, 0, "楷体");
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 30;
	_tcscpy_s(f.lfFaceName, _T("等线"));
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfWeight = FW_SEMIBOLD;
	settextstyle(&f);
	//settextcolor(RGB(255, 119, 119));
	settextcolor(BLACK);
	outtextxy(700, 50, "红方：");
	outtextxy(780, 50, sc1);
	//settextcolor(RGB(203, 250, 255));
	outtextxy(700, 120, "蓝方：");
	outtextxy(780, 120, sc2);

	setlinecolor(RGB(246, 200, 255));
	line(600, 0, 600, 600);
	roundrect(605, 5, 900 - 5, 200 - 5, 40, 40);

	POINT pt1[] = { {680,55},{680,75},{690,65} };
	POINT pt2[] = { {680,125},{680,145},{690,135} };
	setfillcolor(BLACK);
	if (turn % 2)
	{
		fillrectangle(660, 60, 680, 70);
		solidpolygon((POINT*)pt1, 3);
	}
	else
	{
		fillrectangle(660, 130, 680, 140);
		solidpolygon((POINT*)pt2, 3);
	}



	int dt = 0;
	//setlinecolor(RGB(246, 200, 255));
	for (int i = 0; i < 3; i++)
	{
		if (ch[i])
		{
			setfillcolor(RGB(246, 238, 255));
			dt = 2;
		}
		else
		{
			setfillcolor(RGB(255, 241, 248));
			dt = 0;
		}
		solidroundrect(700 + dt, 250 + i * 100 + dt, 800 + dt, 300 + i * 100 + dt, 20, 20);
		roundrect(700 + dt, 250 + i * 100 + dt, 800 + dt, 300 + i * 100 + dt, 20, 20);
	}
	settextcolor(BLACK);
	outtextxy(720, 260, "悔棋");
	outtextxy(720, 360, "认输");
	outtextxy(720, 460, "退出");
	//setfillcolor(RGB(255, 241, 248));
	//solidrectangle(700, 250, 800, 300);
	//solidrectangle(700, 350, 800, 400);
	//solidrectangle(700, 450, 800, 500);
	//line(600, 0, 600, 600);
	//rectangle(periphery, periphery, width - periphery, height - periphery);

	//FlushBatchDraw();
	EndBatchDraw();
}

void ui::GetScore(int s1, int s2)
{
	sprintf_s(sc1, "%d", s1);
	sprintf_s(sc2, "%d", s2);
}

coor ui::GetCoor()
{	
	ExMessage m;
	while (1)
	{
		m = getmessage(EX_MOUSE);
		coor p1 = CoorDinate(m);
		//printf("%d %d\n", p1.x, p1.y);
		if (p1.x == pl.x && p1.y == pl.y)
		{
			if (pm.x != 114 && pm.x >= 0 && pm.y >= 0 && map[pm.x][pm.y] == -1)map[pm.x][pm.y] = 0;
			else if (pm.x == 114)ch[pm.y] = 0;
			pm = { -1,-1 };
			Update();
		}
		else
		{
			if (p1.x == -1 && pm.x != -1)
			{
				if (pm.x != 114 && pm.x >= 0 && pm.y >= 0 && map[pm.x][pm.y] == -1)map[pm.x][pm.y] = 0;
				else if (pm.x == 114)
				{
					ch[pm.y] = 0;
				}
				pm = { -1,-1 };
				//Update();
				continue;
			}
			if (p1.x != pm.x || p1.y != pm.y)
			{
				//if (map[pm.x][pm.y] == -2)continue;

				if (pm.x != 114 && pm.x >= 0 && pm.y >= 0 && map[pm.x][pm.y] != 1)map[pm.x][pm.y] = 0;
				else if (pm.x == 114)
				{
					ch[pm.y] = 0;
				}
				if (p1.x != 114 && map[p1.x][p1.y] == 0 && (p1.x + p1.y) % 2)
				{
					pm = p1;
					map[pm.x][pm.y] = -1;
				}
				else if (p1.x == 114)
				{
					pm = p1;
					ch[pm.y] = 1;
				}
				else
				{
					pm = { -1,-1 };
				}	
			}
			Update();
		}

		printf("%d %d\n", pm.x, pm.y);
		if (m.lbutton && pm.x != -1)
		{
			//if (pm.x >= 0 && pm.x <= 12 && pm.y >= 0 && pm.y <= 12)
			//{
			//	if(pl.x!=-1)map[pl.x][pl.y] = 1;
			//	pl = pm;
			//	map[pl.x][pl.y] = -2;
			//}
			break;
		}
	}
	return pm;
}

coor ui::CoorDinate(ExMessage m)
{
	coor mo = { -1,-1 };
	if ((m.y<periphery || m.y>width - periphery) && m.x < 600 || m.x < periphery)return mo;
	if (m.x <= 600)
	{
		int x = (m.x - periphery) / (e_length + rec_length);
		int y = (m.y - periphery) / (e_length + rec_length);
		bool a = periphery + x * (e_length + rec_length) + e_length > m.x;
		bool b = periphery + y * (e_length + rec_length) + e_length > m.y;
		if (a && !b)
		{
			x = x * 2;
			y = y * 2 + 1;
			mo = { y,x };
		}
		else if (b && !a)
		{
			x = x * 2 + 1;
			y = y * 2;
			mo = { y,x };
		}
		else
		{
			x = x * 2;
			y = y * 2;
			mo = { y,x };
		}
		
	}
	else
	{
		if (m.x >= 700 && m.x<=800)
		{
			if (m.y > 250 && m.y < 300)
			{
				mo = { 114,0 };
			}
			else if (m.y > 350 && m.y < 400)
			{
				mo = { 114,1 };
			}
			else if (m.y > 450 && m.y < 500)
			{
				mo = { 114,2 };
			}
		}
	}
	if (mo.x == 114)return mo;
	if (mo.x >= map.size() || mo.y >= map.size())
	{
		mo = { -1,-1 };
	}
	return mo;
}

void ui::Winner(int k)
{
	setbkmode(TRANSPARENT);
	settextstyle(50, 0, "楷体");
	settextcolor(RGB(255, 119, 119));
	if (k == 1)
	{
		//outtextxy(50, 200, "先手赢，后手小菜鸡");
		outtextxy(50, 200, "红方胜利");
	}
	else if (k == 2)
	{
		//outtextxy(50, 200, "后手赢，先手小辣鸡");
		outtextxy(50, 200, "蓝方胜利");
	}
	outtextxy(50, 300, "双击任意一处返回开始界面");
	//Update();
	int n = 0;
	ExMessage m0;
	m0.lbutton = 0;
	clock_t start;
	while (1)
	{
		ExMessage m1 = getmessage(EX_MOUSE);
		if (m1.lbutton && n == 0)start = clock();
		if (m0.lbutton && !m1.lbutton)n++;
		m0 = m1;
		//if (m1.lbutton)n++;
		if (n == 2)
		{
			double dtime = (double)((clock() - start) / (clock_t)1000);
			if (dtime < 0.01)break;
			else n = 0;
		}
	}
}

void ui::BoardChange(std::vector<std::vector<int>>board)
{
	int len = board.size();
	for (int i = 0; i < len; i++)
	{
		for (int j = 0; j < len; j++)
		{
			if (i % 2 || j % 2)
			{
				if ((i + j) % 2 == 0)//Box
				{
					if (board[i][j] == 20)map[i][j] = 0;
					else if (board[i][j] == 2)map[i][j] = 1;
					else if (board[i][j] == -2)map[i][j] = -1;
				}
				else
				{
					if (board[i][j] == 10)map[i][j] = 0;
					else
					{
						map[i][j] = 1;
					}
				}
			}
		}
	}
}

void ui::ShowBoard(std::vector<std::vector<int>>board)
{
	BoardChange(board);
	Update();
}

void ui::SaveImage()
{
	//IMAGE* saveImage = NULL;
	//getimage(saveImage, 0, 0, 600, 900);
	char saveName[20] = "";
	sprintf_s(saveName, "imageSave\\%d.png", saveNum);
	saveNum++;
	saveimage(saveName);
}
