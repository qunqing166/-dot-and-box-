#pragma once
#include<easyx.h>
#include<stdio.h>
#include<fstream>

class ui_start
{
	int height, width;
	int player1, player2, n;
	int button[10];
	int labu;
	int isFirst;
	bool isError;
	bool isGetRecord;
public:
	ui_start(int, int, int);
	int GetMessage_();
	void Update();
	int Quit();
	int Start();
	int Run(int&, int&, int&, bool&);
	int CoorToButton(ExMessage);
	void GetSize(int);
	void GetMod(int);
	void GetFirst(int);
	int  Help();
	//void Recover();
};