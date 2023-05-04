#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) 
{
    int c;
    while ((c = getopt (argc, argv, "z")) != -1)
        switch (c)
        {
            case 'z':
                printf("z\n");
                break;
            default:
                abort();
        }
    

}
