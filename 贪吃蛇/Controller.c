#define _CRT_SECURE_NO_WARNINGS
#include "Controller.h"
#include "View.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <Windows.h>

static void _SnakeInitialize(Snake *pSnake) {
	int i;
	int initLength = 3;
	int x = 5;
	int y = 5;
	pSnake->pHead = NULL;
	for (i = 0; i < initLength; i++) {
		Node* pNode = (Node*)malloc(sizeof(Node));
		assert(pNode != NULL);
		pNode->position.x = x + i;
		pNode->position.y = y;
		pNode->pNext = pSnake->pHead;
		pSnake->pHead = pNode;
	}
	pSnake->direction = RIGHT;
}

static bool _IsOverLapSnake(int x, int y, const Snake *pSnake) {
	assert(pSnake != NULL);
	Node* pos = pSnake->pHead;
	while (pos) {
		if (pos->position.x == x && pos->position.y == y) {
			return true;
		}
		pos = pos->pNext;
	}
	return false;
}

static void _GenerateFood(Position *pPosition, int width, int height, const Snake* pSnake) {
	int x, y;
	do {
		x = rand() % width;
		y = rand() % height;
	} while (_IsOverLapSnake(x, y, pSnake));
	pPosition->x = x;
	pPosition->y = y;
}

static Position _GetNextPosition(const Snake* psnake) {
	int x, y;
	x = psnake->pHead->position.x;
	y = psnake->pHead->position.y;
	switch (psnake->direction) {
	case UP:
		y -= 1;
		break;
	case DOWN:
		y += 1;
		break;
	case LEFT:
		x -= 1;
		break;
	case RIGHT:
		x += 1;

	}
	Position pos = { x,y };
	return pos;
}

static bool _IsEatFood(Position foodposition, Position nextPosition) {
	if (foodposition.x == nextPosition.x && foodposition.y == nextPosition.y) {
		return true;
	}
	return false;
}

static bool _IsKilledByWall(Position nextPosition, int width, int height) {
	if (nextPosition.x < 0 || nextPosition.x >= width) {
		return true;
	}
	if (nextPosition.y < 0 || nextPosition.y >= height) {
		return true;
	}
	return false;
}

static void _Pause() {
	while (1) {
		if (GetAsyncKeyState(VK_SPACE)) {
			break;
		}
	}
	Sleep(30);
}

static bool _IsKilledBySelf(Snake *pSnake, Position nextPosition) {
	Node *pNode;
	for (pNode = pSnake->pHead->pNext; pNode != NULL; pNode = pNode->pNext) {
		if (pNode->position.x == nextPosition.x
			&& pNode->position.y == nextPosition.y) {

			return true;
		}
	}

	return false;
}

static void _DisplaySnake(const UI *pUI, const Snake *pSnake)
{
	Node *pNode;

	for (pNode = pSnake->pHead; pNode != NULL; pNode = pNode->pNext) {
		UIDisplaySnakeBlockAtXY(pUI, pNode->position.x, pNode->position.y);
	}
}

static void _AddHeadAndDisplay(const UI *pUI, Position pos, Snake *pSnake)
{
	Node *pNode = (Node *)malloc(sizeof(Node));
	assert(pNode != NULL);

	UIDisplaySnakeBlockAtXY(pUI, pos.x, pos.y);

	pNode->position = pos;
	pNode->pNext = pSnake->pHead;
	pSnake->pHead = pNode;
}

static void _RemoveTailAndDisplay(const UI *pUI, Snake *pSnake)
{
	Node *pNode = pSnake->pHead;
	for (; pNode->pNext->pNext != NULL; pNode = pNode->pNext) {
	}

	UICleanBlockAtXY(pUI, pNode->pNext->position.x, pNode->pNext->position.y);

	free(pNode->pNext);
	pNode->pNext = NULL;
}

void GameInitialize(Game *pGame)
{
	assert(pGame != NULL);
	pGame->gameZoneWidth = 30;
	pGame->gameZoneHeight = 28;
	pGame->score = 0;
	pGame->scorePerFood = 10;
	pGame->interval = 200;

	_SnakeInitialize(&pGame->snake);
	_GenerateFood(&pGame->foodPosition, pGame->gameZoneWidth, pGame->gameZoneHeight,
		&pGame->snake);
}

void GameRun(Game *pGame)
{
	UI ui;
	UIInitialize(&ui, pGame->gameZoneWidth, pGame->gameZoneHeight);
	UIDisplayWizard(&ui);

	UIDisplayGameZone(&ui);
	UIDisplayFoodBlockAtXY(&ui, pGame->foodPosition.x, pGame->foodPosition.y);
	_DisplaySnake(&ui, &pGame->snake);

	while (1) {
		if (GetAsyncKeyState(VK_UP)) {
			if (pGame->snake.direction != DOWN) {
				pGame->snake.direction = UP;
			}
		}
		else if (GetAsyncKeyState(VK_DOWN)) {
			if (pGame->snake.direction != UP) {
				pGame->snake.direction = DOWN;
			}
		}
		else if (GetAsyncKeyState(VK_LEFT)) {
			if (pGame->snake.direction != RIGHT) {
				pGame->snake.direction = LEFT;
			}
		}
		else if (GetAsyncKeyState(VK_RIGHT)) {
			if (pGame->snake.direction != LEFT) {
				pGame->snake.direction = RIGHT;
			}
		}

		Position nextPosition = _GetNextPosition(&pGame->snake);
		if (_IsEatFood(pGame->foodPosition, nextPosition)) {
			_AddHeadAndDisplay(&ui, nextPosition, &pGame->snake);
			_GenerateFood(&pGame->foodPosition, pGame->gameZoneWidth, pGame->gameZoneHeight,&pGame->snake);
			UIDisplayFoodBlockAtXY(&ui, pGame->foodPosition.x, pGame->foodPosition.y);
			pGame->score += pGame->scorePerFood;
		}
		else {
			_RemoveTailAndDisplay(&ui, &pGame->snake);
			_AddHeadAndDisplay(&ui, nextPosition, &pGame->snake);
		}

		if (_IsKilledByWall(nextPosition, pGame->gameZoneWidth, pGame->gameZoneHeight)) {
			UIEndGame(&ui, ui.gameZoneWidth, ui.gameZoneHeight, "Ç×£¬Äã×²Ç½ÁËÅ¶£¡");
		}

		if (_IsKilledBySelf(&pGame->snake,nextPosition)) {
			UIEndGame(&ui, ui.gameZoneWidth, ui.gameZoneHeight, "Ç×£¬ÄãÒ§µ½×Ô¼ºÁËÅ¶£¡");
		}

		Sleep(pGame->interval);
	}

	UIDestroy(&ui);
}

void GameDestroy(Game *pGame)
{
	Node *pNode, *pNext;

	for (pNode = pGame->snake.pHead; pNode != NULL; pNode = pNext) {
		pNext = pNode->pNext;
		free(pNode);
	}
}
