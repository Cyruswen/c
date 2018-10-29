#include <stdio.h>
//方法一
#if 1
int main()
{
    double result = 0;
    int i = 1;
    for(; i < 101; i++)
    {
        if(i % 2 != 0)
            result += 1.0/i;
        else
            result -= 1.0/i;
    }
    printf("result:%f\n", result);
    return 0;
}
#endif

#if 0
int main()
{
    double result = 0;
    int i = 1;
    int flag = 1;
    for(; i < 101; i++)
    {
        result += flag * 1.0 / i;
        flag = -flag;
    }
    printf("result:%f\n",result);
    return 0;
}
#endif
