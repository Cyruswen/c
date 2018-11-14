#define _CRT_SECURE_NO_WARNINGS
#include "game.h"
void init(char board[ROW][COL], int row, int col)
{
	memset(board, ' ', row*col);
}

void print(char board[ROW][COL], int row, int col)
{
	int i = 0;
	printf(" ________ ________ ________\n");
	for (i = 0; i < row; i++)
	{
		int j = 0;
		printf("|        |        |        |\n");
		printf("|   %c    |   %c    |   %c    |\n", board[i][0], board[i][1], board[i][2]);
		printf("|        |        |        |\n");
		for (j = 0; j < col; j++)
		{
			printf("|________");
		}
		printf("|\n");
	}
}

void computermove(char board[ROW][COL], int row, int col)
{
	int x = 0;
	int y = 0;
	/*int m = 0;
	printf("\n电脑走：>\n");
	for (m = 0; m < row; m++)
	{
		int n = 0;
		for (n = 0; n < col; n++)
		{
			if()
		}
	}*/
	do
	{
		int x = rand() % row;
		int y = rand() % col;
		if (board[x][y] == ' ')
		{
			board[x][y] = '*';
			break;
		}
	} while (board[x][y] != ' ');
}

void playermove(char board[ROW][COL], int row, int col)
{
	int m = 0;
	int n = 0;
	do
	{
		printf("请输入坐标:>\n");
		scanf("%d%d", &m, &n);
		if (board[m - 1][n - 1] == ' ')
		{
			board[m - 1][n - 1] = 'o';
			break;
		}
		else
		{
			printf("输入的坐标非法，请重新输入!\n");
		}
	} while (board[m - 1][n - 1] != ' ');
}

int iswin(char board[ROW][COL], int row, int col)
{
	int m = 0;
	for (m = 0; m < ROW; m++)
	{
		if (board[m][0] == 'o'&&board[m][1] == 'o'&&board[m][2] == 'o')
		{
			return 1;
		}
		else if (board[0][m] == 'o'&&board[1][m] == 'o'&&board[2][m] == 'o')
		{
			return 1;
		}
	}
	if (board[0][0] == 'o'&&board[1][1] == 'o'&&board[2][2] == 'o'\
		|| board[0][2] == 'o'&&board[1][1] == 'o'&&board[2][0] == 'o')
	{
		return 1;
	}
	int n = 0;
	for (n = 0; n < ROW; n++)
	{
		if (board[n][0] == '*'&&board[n][1] == '*'&&board[n][2] == '*')
		{
			return -1;
		}
		else if (board[0][n] == '*'&&board[1][n] == '*'&&board[2][n] == '*')
		{
			return -1;
		}
	}
	if (board[0][0] == '*'&&board[1][1] == '*'&&board[2][2] == '*'\
		|| board[0][2] == '*'&&board[1][1] == '*'&&board[2][0] == '*')
	{
		return -1;
	}
	int i = 0;
	for (i = 0; i < ROW; i++)
	{
		int j = 0;
		for (j = 0; j < COL; j++)
		{
			if (board[i][j] == ' ')
			{
				return 0;
			}
		}
	}

}
