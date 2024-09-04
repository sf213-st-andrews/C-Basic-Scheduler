// This is a calculator
// A way to test that the scheduler is working

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

    int a,m,b,x;
    char s;

    if (argc < 4) {
        printf("Error: incorrect number of arguments: argc = %d\n", argc);
        fflush(stdout);
        usleep(100000);
        return 1;
    }
    // Make assumptions of passed arguments; ./simple_calculator int char int

    a = atoi(argv[1]);
    m = atoi(argv[2]);
    b = atoi(argv[3]);

    switch (m) {
    case 0:
        x = a + b;
        s = '+';
        break;

    case 1:
        x = a - b;
        s = '-';
        break;

    case 2:
        x = a * b;
        s = '*';
        break;

    case 3:
        x = a / b;
        s = '/';
        break;
    
    default:
        printf("Error: incorrect argument: argv[2], %d, unnacceptable\n", m);
        fflush(stdout);
        usleep(100000);
        return 0;
    }

    printf("%d %c %d = %d\n", a, s, b, x);
    fflush(stdout);
    usleep(100000);
    
    return 1;// Success
}
