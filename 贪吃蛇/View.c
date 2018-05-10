#define _CRT_SECURE_NO_WARNINGS
#include "View.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void _SetConsoleCursorPos(int cursorX, int curSorY) {
	HANDLE hHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { cursorX,curSorY };
	SetConsoleCursorPosition(hHandle, coord);
}

static void _ResetCursorPos(const UI* pUI) {
	int x = pUI->marginLeft;
	int y = pUI->marginTop + (2 + pUI->gameZoneHeight) + 3;
	_SetConsoleCursorPos(x, y);
}

static void _DisplayWall(const UI* pUI) {
	int i;
	int x, y;
	//上边界
	for (i = 0; i < pUI->gameZoneWidth + 2; i++) {
		x = pUI->marginLeft + i * pUI->blockWidth;
		y = pUI->marginTop;
		_SetConsoleCursorPos(x, y);
		printf("%s", pUI->wallBlock);
	}
	//下边界
	for (i = 0; i < pUI->gameZoneWidth + 2; i++) {
		x = pUI->marginLeft + i * pUI->blockWidth;
		y = pUI->marginTop + pUI->gameZoneHeight + 1;
		_SetConsoleCursorPos(x, y);
		printf("%s", pUI->wallBlock);
	}
	//左边界
	for (i = 0; i < pUI->gameZoneHeight + 2; i++) {
		x = pUI->marginLeft;
		y = pUI->marginTop + i;
		_SetConsoleCursorPos(x, y);
		printf("%s", pUI->wallBlock);
	}
	//右边界
	for (i = 0; i < pUI->gameZoneHeight + 2; i++) {
		x = pUI->marginLeft + (pUI->gameZoneWidth + 1) * pUI->blockWidth;
		y = pUI->marginTop + i;
		_SetConsoleCursorPos(x, y);
		printf("%s", pUI->wallBlock);
	}
}

void UIInitialize(UI *pUI, int gameZoneWidth, int gameZoneHeight) {
	pUI->snakeBlock = pUI->foodBlock = "●";
	pUI->wallBlock = "■";
	pUI->gameZoneWidth = gameZoneWidth;
	pUI->gameZoneHeight = gameZoneHeight;
	pUI->marginLeft = 4;
	pUI->marginTop = 2;
	pUI->blockWidth = strlen(pUI->wallBlock );

	int windowWidth = pUI->marginLeft + (2 + pUI->gameZoneWidth) * pUI->blockWidth + 60;
	int windowHeight = pUI->gameZoneHeight + 2 + pUI->marginTop + 5;
	char cmd[1024];
	sprintf(cmd, "mode con cols=%d lines=%d", windowWidth, windowHeight);
	system(cmd);
}

void UIDisplayFoodBlockAtXY(const UI* pUI, int x, int y) {
	_SetConsoleCursorPos(pUI->marginLeft + (1 + x)*pUI->blockWidth, pUI->marginTop + 1 + y);
	printf("%s", pUI->foodBlock);
	_ResetCursorPos(pUI);
}

void UICleanBlockAtXY(const UI* pUI, int x,int y) {
	_SetConsoleCursorPos(pUI->marginLeft + (1 + x)*pUI->blockWidth, pUI->marginTop + 1 + y);
	int i;
	for (i = 0; i < pUI->blockWidth; i++) {
		printf(" ");
	}
	_ResetCursorPos(pUI);
}

void UIDisplaySnakeBlockAtXY(const UI* pUI, int x, int y) {
	_SetConsoleCursorPos(pUI->marginLeft + (1 + x)*pUI->blockWidth, pUI->marginTop + 1 + y);
	printf("%s", pUI->snakeBlock);
	_ResetCursorPos(pUI);
}

void UIDestroy(UI* pUI) {

}

void UIDisplayWizard(const UI *pUI) {
	int windowWidth = pUI->marginLeft
		+ (2 + pUI->gameZoneWidth) * pUI->blockWidth
		+ 60;
	int windowHeight = pUI->marginTop
		+ (2 + pUI->gameZoneHeight)
		+ 5;

	char *message = "欢迎来到贪吃蛇";
	_SetConsoleCursorPos(windowWidth / 2 - strlen(message) / 2, windowHeight / 2);
	printf("%s", message);
	_SetConsoleCursorPos(windowWidth / 2 - strlen(message) / 2, windowHeight / 2 + 2);
	system("pause");
	system("cls");
}

void UIEndGame(const UI* pUI, int x, int y,const char* message) {
	_SetConsoleCursorPos(( x * pUI->blockWidth) / 2, (pUI->marginTop + 1 + y) / 2);
	printf("%s\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n", message);
	_ResetCursorPos(pUI);
	system("pause");
}

void UIDisplayGameZone(const UI *pUI)
{
	_DisplayWall(pUI);
	_ResetCursorPos(pUI);
}