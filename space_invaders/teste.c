#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main()
{
    int i, c;
    srand(time(NULL));
    
    for (c=0; c<10; c++)
    {
        i = rand();
        printf("%d\n", i % 100);
    }
    return 0;
}
