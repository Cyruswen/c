#include <stdio.h>

int main()
{
    int year = 1000;
    int count = 0;
    for(; year <= 2000; year+=2)
    {  
        if(year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
        {
            printf("%d  ", year);
            count++;
        }
    }
    printf("\ncount: %d\n", count);
    return 0;
}
