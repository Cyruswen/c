#pragma once
#include <stdio.h>
#include <def.h>
#include <sys/timerfd.h>
#include <string.h>
#include <stdint.h>
#include <sys/select.h>
#include <unistd.h>
#include <keyboard.h>

void draw_element(int x, int y, int c);

void draw_shape(int x, int y, struct shape p, int c);

void timer_tetris(struct data* p);

int tetris(struct data* p);
