#include"ui_start.h"

ui_start::ui_start(int p1, int p2, int x)
{
	height = 250;
	width = 400;
	labu = -1;
	player1 = -1;
	player2 = -1;
	n = x;
	isFirst = 0;
	for (int i = 0; i < 10; i++)button[i] = 0;

	//std::fstream file;
	//file.open("boardSave\\isError.txt", std::ios::in);
	//char buf[1];
	//file >> buf;
	//if (buf[0] == '1')isError = true;
	//else isError = false;
	isGetRecord = false;
	initgraph(width, height);
	//改变窗口位置
	RECT rect;
	HWND hwnd = GetHWnd();
	GetWindowRect(hwnd, &rect);
	MoveWindow(hwnd, 400, 200, rect.right - rect.left, rect.bottom - rect.top, true);
	SetWindowText(hwnd, "点格棋--选项游戏模式");

	setbkcolor(WHITE);
	cleardevice();
	Update();
}

void ui_start::Update()
{
	//FlushBatchDraw();
	BeginBatchDraw();
	cleardevice();
	//setfillcolor(WHITE);
	//solidrectangle(0, 0, 400, 250);
	int r = 85;
	int a = 45;
	setfillcolor(RGB(246, 238, 255));
	settextcolor(BLACK);
	//settextstyle(20, 0, "楷体");
	setbkmode(TRANSPARENT);
	//LOGFONT f;
	//gettextstyle(&f);
	//f.lfHeight = 20;
	//_tcscpy_s(f.lfFaceName, _T("楷体"));
	//f.lfQuality = ANTIALIASED_QUALITY;
	//settextstyle(&f);

	//
	//POINT pt1[] = { {250,40},{250,74},{225,57} };
	//POINT pt2[] = { {260,40},{260,74},{285,57} };
	setlinecolor(RGB(246, 200, 255));
	solidroundrect(50 + 1 * r, 40, 120 + 1 * r, 75, 20, 20);
	roundrect(50 + 1 * r, 40, 120 + 1 * r, 75, 20, 20);
	//solidpolygon((POINT*)pt1, 3);
	//solidpolygon((POINT*)pt2, 3);

	//

	//for (int i = 1; i < 4; i++)
	//{
	//	solidroundrect(50 + i * r, 90, 120 + i * r, 125, 20, 20);
	//}


	//


	//solidroundrect(180 - a, 140, 250 - a, 175, 20, 20);
	//solidroundrect(265 - a, 140, 335 - a, 175, 20, 20);

	//
	//solidroundrect(20, 210, 90, 235, 20, 20);
	//solidroundrect(380, 210, 310, 235, 20, 20);
	//solidroundrect(165, 200, 245, 235, 20, 20);

	int dt = 0;
	
	for (int i = 0; i < 10; i++)
	{
		if (button[i] == 0)
		{
			setfillcolor(RGB(246, 238, 255));

			dt = 0;
		}
		else if (button[i] == 1)
		{ 
			//setfillcolor(YELLOW); 
			setfillcolor(RGB(246, 238, 255));
			//setlinecolor(RGB(246, 200, 255));
			dt = 2;
		}
		else if (button[i] == 2)
		{
			//setfillcolor(GREEN);
			setfillcolor(RGB(254, 143, 255));
			//setlinecolor(RGB(246, 200, 255));
			dt = 0;
		}
		switch (i)
		{
			case 0:
			{
				POINT pt1[] = { {250 + dt,40 + dt},{250 + dt,74 + dt},{225 + dt,57 + dt} };
				solidpolygon((POINT*)pt1, 3);
				polygon((POINT*)pt1, 3);
				break;
			};
			case 1:
			{
				POINT pt2[] = { {260 + dt,40 + dt},{260 + dt,74 + dt},{285 + dt,57 + dt} };
				solidpolygon((POINT*)pt2, 3);
				polygon((POINT*)pt2, 3);
				break;
			}
			case 2:
				solidroundrect(50 + 1 * r + dt, 90 + dt, 120 + 1 * r + dt, 125 + dt, 20, 20); 
				roundrect(50 + 1 * r + dt, 90 + dt, 120 + 1 * r + dt, 125 + dt, 20, 20); break;
			case 3:
				solidroundrect(50 + 2 * r + dt, 90 + dt, 120 + 2 * r + dt, 125 + dt, 20, 20);
				roundrect(50 + 2 * r + dt, 90 + dt, 120 + 2 * r + dt, 125 + dt, 20, 20);
				break;
			case 4:
				solidroundrect(50 + 3 * r + dt, 90 + dt, 120 + 3 * r + dt, 125 + dt, 20, 20);
				roundrect(50 + 3 * r + dt, 90 + dt, 120 + 3 * r + dt, 125 + dt, 20, 20);
				break;
			case 5:
				if (isFirst)
				{
					solidroundrect(180 - a + dt, 140 + dt, 250 - a + dt, 175 + dt, 20, 20);
					roundrect(180 - a + dt, 140 + dt, 250 - a + dt, 175 + dt, 20, 20);
				}
				break;
			case 6:
				if (isFirst)
				{
					solidroundrect(265 - a + dt, 140 + dt, 335 - a + dt, 175 + dt, 20, 20);
					roundrect(265 - a + dt, 140 + dt, 335 - a + dt, 175 + dt, 20, 20);
				}
				break;
			case 7:
				solidroundrect(20 + dt, 210 + dt, 90 + dt, 235 + dt, 20, 20);
				roundrect(20 + dt, 210 + dt, 90 + dt, 235 + dt, 20, 20);
				break;
			case 8:
				solidroundrect(165 + dt, 200 + dt, 245 + dt, 235 + dt, 20, 20);
				roundrect(165 + dt, 200 + dt, 245 + dt, 235 + dt, 20, 20);
				break;
			case 9:
				solidroundrect(380 + dt, 210 + dt, 310 + dt, 235 + dt, 20, 20);
				roundrect(380 + dt, 210 + dt, 310 + dt, 235 + dt, 20, 20);
				break;
			default:break;
		}
	}

	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 20;
	_tcscpy_s(f.lfFaceName, _T("等线"));
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfWeight = FW_SEMIBOLD;
	settextstyle(&f);
	outtextxy(25, 45 + 0 * 50, _T("规格选择："));
	outtextxy(25, 45 + 1 * 50, "对战选择：");
	if(isFirst)outtextxy(25, 45 + 2 * 50, "先手选择：");

	char s[5];
	sprintf_s(s, "%d", n);
	outtextxy(165, 47, s);
	outtextxy(145, 98, "人-人");
	outtextxy(230, 98, "人-机");
	outtextxy(315, 98, "机-机");
	if (isFirst)
	{
		outtextxy(140, 148, "人先手");
		outtextxy(225, 148, "人后手");
	}
	outtextxy(35, 213, "帮助");
	outtextxy(185, 208, _T("开始"));
	outtextxy(325, 213, "退出");

	//FlushBatchDraw();
	EndBatchDraw();
}

int ui_start::Run(int& p1, int& p2, int& num, bool& getRecord)
{
	int k = GetMessage_();
	if (k == 1)
	{
		p1 = player1;
		p2 = player2;
		num = n;
		getRecord = isGetRecord;
	}
	return k;
}

int ui_start::GetMessage_()
{
	ExMessage m;
	while (1)
	{
		m = getmessage(EX_MOUSE);
		int k = CoorToButton(m);
		//printf("%d\n", k);
		if (k == labu && !m.lbutton)continue;
		else if (k != labu)
		{
			if (labu == k || button[k] == 2)continue;
			else
			{
				if (labu != -1 && button[labu] != 2)button[labu] = 0;
				if (k != -1)button[k] = 1;
				labu = k;
				Update();
			}
		}
		//printf("%d\n", button[3]);
		//printf("%d %d\n", labu, k);
		if (m.lbutton)
		{
			//printf("%d\n",k);
			if (k >= 0 && k <= 1)GetSize(k);
			else if (k >= 2 && k <= 4)GetMod(k);
			else if (k >= 5 && k <= 6)GetFirst(k);
			else if (k == 7)return Help();
			else if (k == 8)return Start();
			else if (k == 9)return Quit();
			Update();
			break;
		}
	}
	return 0;
}

void ui_start::GetSize(int i)
{
	if (i)
	{
		if (n == 6)return;
		else n++;
	}
	else
	{
		if (n == 1)return;
		else n--;
	}
}

void ui_start::GetMod(int i)
{
	for (int j = 2; j <= 4; j++)button[j] = 0;
	button[i] = 2;
	if (i == 2)
	{
		player1 = player2 = 1;
		isFirst = 0;
		cleardevice();
		Update();
	}
	else if (i == 3)
	{
		isFirst = 1;
		GetFirst(5);
	}
	else if (i == 4)
	{
		player1 = player2 = 0;
		isFirst = 0;
		cleardevice();
		Update();
	}

}

void ui_start::GetFirst(int i)
{
	button[5] = button[6] = 0;
	if (isFirst == 0)return;
	else
	{
		button[i] = 2;
	}
	if (i == 5)
	{
		player1 = 1;
		player2 = 0;
	}
	else if (i == 6)
	{
		player1 = 0;
		player2 = 1;
	}
}

int ui_start::CoorToButton(ExMessage m)
{
	int x = m.x;
	int y = m.y;
	if (x >= 225 && x <= 250 && y >= 40 && y <= 74)return 0;
	if (x >= 260 && x <= 285 && y >= 40 && y <= 74)return 1;
	if (x >= 50 + 85 && x <= 120 + 85 && y >= 90 && y <= 125)return 2;
	if (x >= 50 + 85 * 2 && x <= 120 + 85 * 2 && y >= 90 && y <= 125)return 3;
	if (x >= 50 + 85 * 3 && x <= 120 + 85 * 3 && y >= 90 && y <= 125)return 4;
	if (x >= 180 - 45 && x <= 250 - 45 && y >= 140 && y <= 175)return 5;
	if (x >= 265 - 45 && x <= 335 - 45 && y >= 140 && y <= 175)return 6;
	if (x >= 20 && x <= 90 && y >= 210 && y <= 235)return 7;
	if (x >= 165 && x <= 245 && y >= 200 && y <= 235)return 8;
	if (x >= 310 && x <= 380 && y >= 210 && y <= 235)return 9;
	return -1;
}

int ui_start::Start()
{
	if (isGetRecord)return 1;
	if (player1 == -1 || player2 == -1 || n == -1)
	{
		settextcolor(RED);
		settextstyle(50, 0, "楷体");
		outtextxy(30, 80, "请选择游戏模式");
		Sleep(1000);
		cleardevice();
		Update();
		return 0;
	}
	return 1;
}

int ui_start::Quit()
{
	return 2;
}

int ui_start::Help()
{
	//settextcolor(RED);
	//settextstyle(30, 0, "楷体");
	//outtextxy(100, 80, "不帮，滚");
	//outtextxy(40, 80, "这个按钮只是为了美观");
	isGetRecord = true;
	IMAGE help;
	loadimage(&help, "image\\doge.png");
	putimage(40, 70, &help);
	Sleep(1000);
	cleardevice();
	Update();
	return 0;
}

//void ui_start::Recover()
//{
//}
