/*
 * @Author: GoatWu
 * @Date: 2021-09-30 01:53:09
 * @LastEditTime: 2021-10-06 04:25:29
 * @LastEditors: Goatwu
 * @FilePath: /xv6-labs-2020/user/find.c
 * Talk is cheap. Show me the code.
 */
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int contain(char *path, char *name) {
    char *p, *q;
    for(p=path+strlen(path); p >= path && *p != '/'; p--);
    p++;
    q = name;
    if (strcmp(p, q) == 0)
        return 1;
    else
        return -1;
    return 1;
}

void find(char *path, char *name, int isroot) {
    // printf("now finding in: %s\n", path);
    char buf[512], *p;
    int fd;

    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    
    switch (st.type) {
    case T_FILE:
        if (isroot) {
            fprintf(2, "find: %s is not a directory\n", path);
            return;
        }
        if (contain(path, name) == 1) {
            printf("%s\n", path);
        }
        break;
    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)) {
            if(de.inum == 0)
                continue;
            if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            find(buf, name, 0);
        }
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "usage: find <start path> <file name>\n");
        exit(1);
    }
    find(argv[1], argv[2], 1);
    exit(0);
}