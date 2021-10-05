/*
 * @Author: GoatWu
 * @Date: 2021-09-30 01:53:09
 * @LastEditTime: 2021-10-06 04:25:47
 * @LastEditors: Goatwu
 * @FilePath: /xv6-labs-2020/user/xargs.c
 * Talk is cheap. Show me the code.
 */
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXLINE 1024

int main(int argc, char *argv[]) {
    // default command: echo
    char defaul[] = "echo";
    char command[16];
    char *arg[MAXARG];
    int argCount = 0;
    char buf[512]; // the buffer and it's pointer
    int n; // number of bytes to read
    if (argc == 1) { // use default command
        strcpy(command, defaul);
    } else { // command: argv[1]
        strcpy(command, argv[1]);
    }

    arg[argCount++] = command;
    for (int i = 2; i < argc; i++) {
        arg[argCount++] = argv[i];
    }

    while ((n = read(0, buf, MAXLINE)) > 0) {
        if (fork() == 0) {
            char *tmp = (char*) malloc(sizeof(buf));
            int i, j;
            for (i = 0, j = 0; i < n; i++) {
                if (buf[i] == ' ' || buf[i] == '\n') {
                    tmp[j] = 0;
                    arg[argCount++] = tmp;
                    tmp = (char*) malloc(sizeof(buf));
                    j = 0;
                } else {
                    tmp[j++] = buf[i];
                }
            }
            
            arg[argCount] = 0;
            exec(command, arg);
        } else {
            wait(0);
        }
    }
    
    exit(0);
}