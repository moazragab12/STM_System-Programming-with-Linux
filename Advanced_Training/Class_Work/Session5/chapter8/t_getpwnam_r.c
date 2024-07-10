/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2023.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Supplementary program for Chapter 8 */

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        printf("%s username\n", argv[0]);
        return -1;
    }

    size_t bufSize = sysconf(_SC_GETPW_R_SIZE_MAX);
    char *buf = malloc(bufSize);
    if (buf == NULL) {
        printf("malloc returned null, size = %zu\n", bufSize);
        return -1;
    }

    struct passwd *result;
    struct passwd pwd;

    int s = getpwnam_r(argv[1], &pwd, buf, bufSize, &result);
    if (s != 0) {
        perror("getpwnam_r");
        return -1;
    }

    if (result != NULL)
        printf("Name: %s\n", pwd.pw_gecos);
    else
        printf("Not found\n");

    exit(EXIT_SUCCESS);
}
