//main.h
#pragma once
using namespace std;

typedef struct DATA {
	POINT pt;
	bool stone;
}save;

vector<save> list;
enum Control { listbox, newgame, back };

HBITMAP hPan, hWhite, hBlack;
BOOL stone=true;

int panStatus[20][20];
int defeatCount = 400;

bool WinStatus=false;

POINTS pt;
HINSTANCE g_hInst;
HWND hList;

void AddListStatus(int x, int y);
void PrintWin();
void initGame(HWND *hwnd);

void SubBackListStatus();