#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int main()
{
    int c;
    struct termios ts;
    tcgetattr(0, &ts);
    ts.c_lflag &= ~ICANON;
    ts.c_lflag &= ~ECHO;  //关闭回显
    tcsetattr(0, TCSANOW, &ts);  //输入立即显示
   // char buf[1024] = {};
   // int i = 0;
   // while((c = getchar()) != '\n')
    while((c = getchar()) != EOF)
    {
        putchar(c);
   //   buf[i++] = c;
   //   putchar('*');
    }

    //用完恢复回去
    ts.c_lflag |= ICANON;
    ts.c_lflag |= ECHO;
    tcsetattr(0, TCSANOW, &ts);  //恢复

   // printf("buf = [%s]", buf);
    return 0;
}
