#include <stdio.h>
#include <def.h>
#include <sys/timerfd.h>
#include <string.h>
#include <stdint.h>
#include <sys/select.h>
#include <unistd.h>
#include <keyboard.h>

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

int main( void )
{
    init_keyboard();
    int tfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    struct itimerspec ts;
    memset(&ts, 0x00, sizeof ts);
    ts.it_value.tv_nsec = 1;
    ts.it_interval.tv_sec = 1;
    timerfd_settime(tfd, 0, &ts, NULL);

    fd_set rset;
    int kfd = fileno(stdin);
    int maxfd = tfd > kfd ? tfd:kfd;
    while(1)
    {
        FD_ZERO(&rset);
        FD_SET(tfd, &rset);
        FD_SET(kfd, &rset);
        int n = select(maxfd+1, &rset, NULL, NULL, NULL);
        if(FD_ISSET(tfd, &rset))
        {
            uint64_t clk;
            read(tfd, &clk, sizeof(clk));
            timer_tetris(&t);
        }
        if(FD_ISSET(kfd, &rset))
        {
            if(tetris(&t) == 1)
            {
                break;
            }

        }

    }
    printf("\033[?25h");
    recover_keyboard();
}
