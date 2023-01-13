#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) 
{
    if (argc <= 1) {
        printf("error input!");
        exit(1);
    } else {
        int num = atoi(argv[1]);
        sleep(num);
    }
    exit(0);
}