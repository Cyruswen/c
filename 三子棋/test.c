#define _CRT_SECURE_NO_WARNINGS
#include"game.h"
void menu()
{
	printf("*****************************************\n");
	printf("************欢迎来到三子棋游戏***********\n");
	printf("******************请选择*****************\n");
	printf("********* 1.进入游戏  0.退出 ************\n");
	printf("*****************************************\n");
}

void game()
{
	char board[ROW][COL];
	int ret = 0;
	init(board, ROW, COL);
	print(board,ROW,COL);
	while (1)
	{
		playermove(board, ROW, COL);
		system("cls");
		print(board, ROW, COL);
		ret = iswin(board, ROW, COL);
		if (ret == 1)
		{
			printf("\n恭喜你，获胜了！\n\n");
			break;
		}
		if (ret != 0)
		{
			printf("\n平局\n\n");
			break;
		}
		computermove(board, ROW, COL);
		system("cls");
		print(board, ROW, COL);
		ret = iswin(board, ROW, COL);
		if (ret == -1)
		{
			printf("\n很遗憾，电脑获胜。\n\n");
			break;
		}
		if (ret != 0)
		{
			printf("\n平局\n\n");
			break;
		}
	}
}

int main()
{
	int input = 0;
	srand(time(NULL));
	do
	{
		menu();
		scanf("%d", &input);
		switch (input)
		{
		case 1:
			game();
			break;
		case 0:
			break;
		default:
			printf("选择错误，请重新选择:>\n");
			break;
		}
	} while (input);
	return 0;
}