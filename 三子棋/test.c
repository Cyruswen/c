#define _CRT_SECURE_NO_WARNINGS
#include"game.h"
void menu()
{
	printf("*****************************************\n");
	printf("************��ӭ������������Ϸ***********\n");
	printf("******************��ѡ��*****************\n");
	printf("********* 1.������Ϸ  0.�˳� ************\n");
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
			printf("\n��ϲ�㣬��ʤ�ˣ�\n\n");
			break;
		}
		if (ret != 0)
		{
			printf("\nƽ��\n\n");
			break;
		}
		computermove(board, ROW, COL);
		system("cls");
		print(board, ROW, COL);
		ret = iswin(board, ROW, COL);
		if (ret == -1)
		{
			printf("\n���ź������Ի�ʤ��\n\n");
			break;
		}
		if (ret != 0)
		{
			printf("\nƽ��\n\n");
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
			printf("ѡ�����������ѡ��:>\n");
			break;
		}
	} while (input);
	return 0;
}