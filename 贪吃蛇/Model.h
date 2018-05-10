#pragma once
typedef struct Position {
	int x;
	int y;
} Position;

typedef struct Node {
	Position position;
	struct Node* pNext;
} Node;

typedef enum Direction {
	UP,DOWN,LEFT,RIGHT
} Direction;

typedef struct Snake {
	Direction direction;
	Node* pHead;
} Snake;

typedef struct Game {
	int gameZoneWidth;
	int gameZoneHeight;

	Snake snake;
	Position foodPosition;
	int score;
	int scorePerFood;
	int interval;
} Game;