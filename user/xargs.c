#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

char* xgets(char *buf, int max)
{
    int i, cc;
    char c;

    for(i = 0; i + 1 < max; ){
        cc = read(0, &c, 1);
        if(cc < 1)
            break;
        if (c == '\n' || c == '\r') {
            break;
        }
        buf[i++] = c;
    }
    buf[i] = '\0';
    return buf;
}

int xgetcmd(char *buf, int nbuf)
{
    // 将buf填充为0
    memset(buf, 0, nbuf);
    xgets(buf, nbuf);
    if(buf[0] == 0) // EOF
        return -1;
    return 0;
}

void append(char *a[], int *i, char *b) {
    a[*i] = b;
    (*i)++;
    a[*i] = 0;
}
// 从标准输入中按行读取，并且为每一行执行一个命令，将行作为参数提供给命令

int main(int argc, char *argv[])
{
    char *s_argv[MAXARG]; 
    // printf("d: %d\n", argc);
    int num_arg = argc - 1;
    // s_argv[0] 是命令，之后为参数    
    for (int i = 0; i < num_arg; i++) {
        s_argv[i] = argv[i + 1];
    }

    // 临时存储数组
    char buf[100];
    // 装参数字符的数组
    char arg[MAXARG][100];
    // 然后开始处理额外的参数，从标准输入中读取 
    int len = 0;
    while (xgetcmd(buf, sizeof(buf)) >= 0) {           
        strcpy(arg[len++], buf);
    }

    for (int j = 0; j < len; j++) {
        if (fork() == 0) {
            // printf("xf: %s\n", arg[j]);
            append(s_argv, &num_arg, arg[j]);
            // printf("x: %s\n", s_argv[num_arg-3]);
            // printf("x: %s\n", s_argv[num_arg-2]);
            // printf("x: %s\n", s_argv[num_arg-1]);
            // printf("x: %s\n", s_argv[num_arg]);
            exec(s_argv[0], s_argv);
        } else {
            wait(0);
        }

    }
    exit(0);
}