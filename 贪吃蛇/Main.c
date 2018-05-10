#define _CRT_SECURE_NO_WARNINGS
#include "Controller.h"
#include <stdlib.h>
#include <time.h>


int main()
{
	srand((unsigned int)time(NULL));

	Game game;

	GameInitialize(&game);
	GameRun(&game);
	GameDestroy(&game);
	

	return 0;
}