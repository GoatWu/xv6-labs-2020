/*
 * @Author: GoatWu
 * @Date: 2021-09-30 01:53:09
 * @LastEditTime: 2021-10-06 04:24:49
 * @LastEditors: Goatwu
 * @FilePath: /xv6-labs-2020/user/pingpong.c
 * Talk is cheap. Show me the code.
 */
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READEND 0
#define WRITEEND 1

int main(int argc, char *argv[]) {
    if (argc != 1) {
        fprintf(2, "usage: pingpong\n");
        exit(1);
    }
    char buf[10];
    int parent_fd[2], child_fd[2];
    pipe(parent_fd);
    pipe(child_fd);
    int pid = fork();
    if (pid < 0) {
        fprintf(2, "fork error\n");
    } else if (pid == 0) {
        close(parent_fd[WRITEEND]);
        close(child_fd[READEND]);
        read(parent_fd[READEND], buf, sizeof buf);
        printf("%d: received %s\n", getpid(), buf);
        write(child_fd[WRITEEND], "pong", 4);
        close(child_fd[WRITEEND]);
    } else {
        close(parent_fd[READEND]);
        close(child_fd[WRITEEND]);
        write(parent_fd[WRITEEND], "ping", 4);
        close(parent_fd[WRITEEND]);
        read(child_fd[READEND], buf, sizeof buf);
        printf("%d: received %s\n", getpid(), buf);
    }
    exit(0);
}