#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "stdbool.h"

bool cmpname(char *path, char *name) {
    char* p;
    for(p=path+strlen(path); p >= path && *p != '/'; p--);
    p++;
    if (!strcmp(p, name)) return true;
    return false;
}

void find(char *path, char *name)
{
    char buf[512], *p;
    int fd;

    struct dirent de;
    struct stat st;

    // 只读方式打开目录文件失败
    if ((fd = open(path, 0)) < 0) {
        // 2: 标准错误输出
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    // 获取目录文件信息失败
    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type) {
    // 如果已经是文件，就要判断这个文件名字是不是name
    case T_FILE:
        if (cmpname(path, name))  {
            printf("%s\n", path);
        }
        break;
    // 如果是目录，就要递归咯
    case T_DIR:
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        // 使用read读取目录文件
        while(read(fd, &de, sizeof(de)) == sizeof(de)) {
            // 已经初始化可以创建文件的位置
            if (de.inum == 0) {
                continue;
            }
            // 如果该目录下遇到 . .. 直接跳过
            if (!strcmp(".", de.name) || !strcmp("..", de.name)) {
                continue;
            }
            // 其余的接起来直接递归
            //char newpath[512];
			memmove(p, de.name, DIRSIZ);
			p[DIRSIZ] = 0;

            // catname(buf, de.name, newpath);

            find(buf, name);
        }
        break;
    }
    close(fd);
    return;
}


int main(int argc, char *argv[])
{
    // 先考虑最常见的情况  find . b 一般是argc == 3
    // argv[1] 一般是要查找的目录 argv[2] 是要查找的内容
    if (argc == 2) {
        find(".", argv[1]);
    } else {
        find(argv[1], argv[2]);        
    }

    exit(0);    
}