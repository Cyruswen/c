#pragma once

#define ROW 3
#define COL 3

#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>

void init(char board[ROW][COL], int row, int col);
void print(char board[ROW][COL], int row, int col);
void computermove(char board[ROW][COL], int row, int col);
void playermove(char board[ROW][COL], int row, int col);
int iswin(char board[ROW][COL], int row, int col);