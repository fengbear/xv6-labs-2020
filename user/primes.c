#include "kernel/types.h"
#include "user/user.h"

void prime(int pleft[2]) 
{
    
    int primes, nums;

    // 再建立向右通信的管道
    int pright[2];   
    pipe(pright);

    // 第一次read
    if (read(pleft[0], &primes, sizeof(primes))) {
        printf("prime %d\n", primes);
        if (fork() == 0) {
            close(pright[1]);
            close(pleft[0]);
            prime(pright);
        } else {
            close(pright[0]);
            while(read(pleft[0], &nums, sizeof(nums))) {
                if (nums % primes != 0) {
                    write(pright[1], &nums, sizeof(nums));
                }
            }
            close(pright[1]);
            wait(0);
        }
    } else {
        // 说明父进程没有传输
        close(pleft[0]);
        close(pright[1]);
    }
    exit(0);
}

int main(int argc, char *argv[]) 
{
    int p[2];
    pipe(p);

    if (fork() == 0) {
        // 关闭该子进程管道的写端
        close(p[1]);
        prime(p);
    } else {
        // 关闭第一个父进程管道的读端
        close(p[0]);
        for (int i = 2; i <= 35; i++) {
            write(p[1], &i, sizeof(i));
        }       
        // 全部写完，关闭写端，子进程read返回0
        close(p[1]);
        wait(0);       
    }
    exit(0);
}