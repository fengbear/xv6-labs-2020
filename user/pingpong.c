#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) 
{
    int p1[2];
    int p2[2];

    // p1 负责子进程向父进程发送一个字节
    // p2 负责父进程向子进程发送一个字节
    pipe(p1);
    pipe(p2);

    // 子进程
    if (fork() == 0) {
        
        // 关闭p1读取端
        close(p1[0]);
        // 关闭p2写入端
        close(p2[1]);

        int son_id = getpid();
        char buf[1];
        sleep(2);
        int bt = read(p2[0], buf, 1);
        if (bt == 1) {
            printf("%d: received ping\n", son_id);
        } else {
            printf("son read error!");
            exit(1);
        }
        write(p1[1], buf, 1);
    } else {
    // 父进程
        close(p1[1]);
        close(p2[0]);

        int father_id = getpid();
        char buf[1];
        buf[0] = 'p';
        write(p2[1], buf, 1);
        sleep(2);
        int bt = read(p1[0], buf, 1);
        if (bt == 1) {
            printf("%d: received pong\n", father_id);
        } else {
            printf("father read error!");
            exit(1);
        }
        
    }
    exit(0);
}