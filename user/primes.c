/*
 * @Author: GoatWu
 * @Date: 2021-09-30 01:53:09
 * @LastEditTime: 2021-10-06 04:26:01
 * @LastEditors: Goatwu
 * @FilePath: /xv6-labs-2020/user/primes.c
 * Talk is cheap. Show me the code.
 */
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXN 35
#define READEND 0
#define WRITEEND 1

void initProcess();
void Process(int *p);

void initProcess() {
    int p[2];
    pipe(p);
    int pid = fork();
    if (pid < 0) {
        fprintf(2, "fork error.\n");
        exit(1);
    } else if (pid > 0) { // parent
        printf("prime 2\n");
        close(p[READEND]);
        for (int i = 3; i < MAXN; i++) {
            if (i % 2 != 0) {
                write(p[WRITEEND], &i, 4);
            }
        }
        close(p[WRITEEND]);
        wait(0);
    } else { // child
        close(p[WRITEEND]);
        Process(p);
    }
}

void Process(int *p) {
    int p2[2];
    pipe(p2);
    int prime, i;
    int status;
    if ((status = read(p[READEND], &prime, 4)) > 0) { // The first number received is a prime
        printf("prime %d\n", prime);
        int pid = fork();
        if (pid > 0) { // parent
            close(p2[READEND]);
            while (read(p[READEND], &i, 4) > 0) {
                if (i % prime != 0) {
                    write(p2[WRITEEND], &i, 4);
                }
            }
            close(p2[WRITEEND]);
            wait(0);
        } else { // child
            close(p2[WRITEEND]);
            Process(p2);
        }
    } else if (status < 0) {
        fprintf(2, "%d: gread error", getpid());
        exit(1);
    } else {
        return;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 1) {
        fprintf(2, "usage: primes\n");
        exit(1);
    }
    initProcess();

    exit(0);
}