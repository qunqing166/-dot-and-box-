#pragma once
class coor
{
public:
	int x, y;

	coor()
	{
		x = y = 0;
	}
	coor(int a, int b)
	{
		x = a;
		y = b;
	}
};

struct undonode
{
	coor p;
	int player;
};

struct mod
{
	int n;
	int player1, player2;
};