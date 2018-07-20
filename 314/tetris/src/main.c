#include <keyboard.h>
#include <tetris.h>

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
    return 0;
}
