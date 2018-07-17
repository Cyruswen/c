#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/timerfd.h>
#include <signal.h>
#include <string.h>
void handler(int s)
{
    printf("recv %d\n", s);
}
int main()
{
    signal(SIGALRM, handler);
    int tfd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC);
    if( tfd == -1 )
    {
        perror("timerfd_create");
        exit(1);
    }
    struct itimerspec isp;
    memset(&isp, 0x00, sizeof(isp));
    isp.it_value.tv_sec = 3;
    isp.it_interval.tv_sec = 1;
    timerfd_settime(tfd, 0, &isp, NULL);
    int kfd = fileno(stdin);
    int maxfd = tfd > kfd? tfd:kfd;

    fd_set set;
    while(1)
    {
        FD_ZERO(&set);
        FD_SET(tfd, &set);
        FD_SET(kfd, &set);
        int nready = select(maxfd + 1, &set, NULL, NULL, NULL);
        if(nready <= 0)
        {
            continue;
        }
        if(FD_ISSET(tfd, &set))
        {
            uint64_t es;
            int r = read(tfd, &es, sizeof(es));
            if(r <= 0) break;
            printf("r=%d\n", r);
        }
        if(FD_ISSET(kfd, &set))
        {
            char buf[1024] = {};
            int r;
lab:            
            r = read(kfd, buf, 1024);
            if(r <= 0)
            {
                if(r == -1 && errno == EINTR)
                {
                    goto lab;
                }
                break;
            }
            printf("buf[%s]\n", buf);
        }
    }

}
