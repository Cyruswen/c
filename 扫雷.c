#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_ROW 10
#define MAX_COL 10
#define DEFAULT_MAIN_COUNT 10

int Menu()
{
	printf("===================\n");
	printf("1. 开始游戏\n");
	printf("0. 退出游戏\n");
	printf("===================\n");
	int choice;
	scanf("%d", &choice);
	return choice;
}

void Init(char show_map[MAX_ROW + 2][MAX_COL + 2], char mine_map[MAX_ROW + 2][MAX_COL + 2])
{
	for (int row = 0; row < MAX_ROW + 2; ++row)
	{
		for (int col = 0; col < MAX_COL + 2; col++)
		{
			show_map[row][col] = '*';
		}
	}

	for (int row = 0; row < MAX_ROW + 2; ++row)
	{
		for (int col = 0; col < MAX_COL + 2; col++)
		{
			mine_map[row][col] = '0';
		}
	}

	int mine_count = DEFAULT_MAIN_COUNT;
	while (mine_count > 0)
	{
		//随机布置地雷
		//随机产生地雷的坐标
		int row = rand() % 10 + 1;
		int col = rand() % 10 + 1;
		if (mine_map[row][col] == '1')
		{
			//当前位置已经有雷，重新产生雷
			continue;
		}
		mine_map[row][col] = '1';
		--mine_count;
	}
}


void DisplayMap(char map[MAX_ROW + 2][MAX_COL + 2])
{
	printf("    ");
	//打印第一行， 纵坐标
	for (int i = 1; i <= MAX_COL; i++)
	{
		printf("%d ", i);
	}
	printf("\n");
	//打印上边框
	for (int i = 1; i <= MAX_COL; i++)
	{
		printf("---");
	}
	printf("\n");
	//按行打印具体地图
	for (int row = 1; row <= MAX_ROW; row++)
	{
		printf("%02d|", row);
		for (int col = 1; col <= MAX_COL; col++)
		{
			printf("%c ", map[row][col]);
		}
		printf("\n");
	}
}

void UpdataShowMap(char show_map[MAX_ROW + 2][MAX_COL + 2], char mine_map[MAX_ROW + 2][MAX_COL + 2], int row, int col)
{
	//这个函数要根据mine_map计算 row col 位置上周围有几个地雷， 将结果写在show_map对应位置上。
	//下面省略八个坐标， 不会越界，因为有边框
	int mine_cnt = (mine_map[row - 1][col - 1] - '0')
		+ (mine_map[row - 1][col] - '0')
		+ (mine_map[row - 1][col + 1] - '0')
		+ (mine_map[row][col - 1] - '0')
		+ (mine_map[row][col + 1] - '0')
		+ (mine_map[row + 1][col - 1] - '0')
		+ (mine_map[row + 1][col] - '0')
		+ (mine_map[row + 1][col + 1] - '0');
	show_map[row][col] = '0' + mine_cnt;
}

void Game()
{
	//使用二维数组表示地图
	//第一个地图给玩家展示地图
	//对于show_map的元素有一下情况
	// 1. 这个位置没有被掀开  *
	// 2. 这个位置已经掀开，使用一个具体的数字表示（字符类型）
	char show_map[MAX_ROW + 2][MAX_COL + 2];
	//第二个地图表示雷阵
	//对于 mine_map， 里面的元素有一下两种情况
	// 1. 是雷， ‘1’表示
	// 1. 不是雷， ‘0’表示
	int blank_count = 0;
	char mine_map[MAX_ROW + 2][MAX_COL + 2];
	//1. 对这两个元素进行初始化
	Init(show_map, mine_map);
	//2. 打印初始地图
	DisplayMap(show_map);
	while (1)
	{
		//3. 玩家输入坐标（并判定坐标是否合法）
		int row, col;
		printf("请输入坐标： ");
		scanf("%d %d", &row, &col);
		if (row <= 0 || row >= MAX_ROW || col <= 0 || col >= MAX_COL)
		{
			printf("输入错误，重新输入\n");
			continue;
		}
		//4. 判定玩家是否踩雷， 如果踩雷，游戏结束
		if (mine_map[row][col] == '1')
		{
			printf("游戏结束");
			DisplayMap(mine_map);
			break;
		}
		//5. 如果玩家没踩雷，判断是否掀开了所有的格子（玩家胜利）
		++blank_count;
		if (blank_count == MAX_ROW * MAX_COL - DEFAULT_MAIN_COUNT)
		{
			printf("扫雷成功!\n");
			DisplayMap(mine_map);
			break;
		}
		//6. 统计该位置周围有几个雷，并把这个数字更新到地图上。 
		UpdataShowMap(show_map, mine_map, row, col);
		DisplayMap(show_map);
	}


}

void Start()
{
	while (1)
	{
		int choice = Menu();
		if (choice == 0)
		{
			break;
		}
		Game();
	}
}

int main()
{
	Start();
	system("pause");
	return 0;
}