#pragma once
#include<easyx.h>
#include<vector>
#include<string>
#include"st.h"
#include<ctime>

class ui
{
	std::vector<std::vector<int>>map;
	int height,width;	//ÆåÅÌ³¤¿í
	int periphery;		//±ß½ç¿í¶È
	int size;
	int e_length;
	int rec_length;
	coor pm, p1, pl;
	int ch[3];
	char sc1[5], sc2[5];
	int turn;
	int saveNum;

public:
	ui(int);
	
	void Update();
	coor CoorDinate(ExMessage m);
	coor GetCoor();
	void GetMap(std::vector<std::vector<int>>v, coor);
	void GetTurn(int);
	void GetScore(int = 0, int = 0);
	void ShuNm();
	void Winner(int);
	void BoardChange(std::vector<std::vector<int>>);
	void ShowBoard(std::vector<std::vector<int>>);
	void SaveImage();
};