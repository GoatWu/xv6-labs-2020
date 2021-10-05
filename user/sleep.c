/*
 * @Author: GoatWu
 * @Date: 2021-09-30 01:33:53
 * @LastEditTime: 2021-10-06 04:25:12
 * @LastEditors: Goatwu
 * @FilePath: /xv6-labs-2020/user/sleep.c
 * Talk is cheap. Show me the code.
 */
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(2, "usage: sleep <time>\n");
        exit(1);
    }
    int timeToSleep = atoi(argv[1]);
    sleep(timeToSleep);
    exit(0);
}