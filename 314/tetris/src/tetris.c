#include <tetris.h>

void draw_element(int x, int y, int c)
{
    x *= 2;
	printf("\033[%d;%dH\033[?25l", y, x);
	printf("\033[3%dm\033[4%dm[]", c, c);
	fflush(stdout);
	printf("\033[0m");
}

void draw_shape(int x, int y, struct shape p, int c)
{
	for (int i=0; i<5; i++) {
		for (int j=0; j<5; j++) {
			if ( p.s[i][j] != 0 )
				draw_element(x+j, y+i, c);
		}
	}
}

void timer_tetris(struct data* p)
{
    draw_shape(p->x, p->y, shape_arr[0],BC );
    p->x++;
    p->y++;
    draw_shape(p->x, p->y, shape_arr[0],FC );
}

int tetris(struct data* p)
{
    int c = get_key();
    int ret = 0;
    if(is_left(c))
    {
        draw_shape(p->x, p->y, shape_arr[0], BC);
        p->x--;
        draw_shape(p->x, p->y, shape_arr[0], FC);
    }
    if(is_right(c))
    {
        draw_shape(p->x, p->y, shape_arr[0], BC);
        p->x++;
        draw_shape(p->x, p->y, shape_arr[0], FC);
    }
    if(is_up(c))
    {
        draw_shape(p->x, p->y, shape_arr[0], BC);
        p->y--;
        draw_shape(p->x, p->y, shape_arr[0], FC);
    }
    if(is_down(c))
    {
        draw_shape(p->x, p->y, shape_arr[0], BC);
        p->y++;
        draw_shape(p->x, p->y, shape_arr[0], FC);
    }
    if(is_esc(c))
    {
        ret = 1;
    }
    return ret;
}

