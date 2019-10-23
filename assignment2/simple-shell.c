/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_LINE		80 /* 80 chars per line, per command */

int main(void)
{
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
    	int should_run = 1;
        char *history[MAX_LINE/2 + 1];
        char historycmd[MAX_LINE/2 + 1];
        char historyfile[MAX_LINE/2 + 1];

    	while (should_run){
        	printf("osh>");
        	fflush(stdout);

            int pid = fork();
            int cnt = 0;
            if (pid == 0) {
                char cmd[MAX_LINE/2 + 1];
                char arg[MAX_LINE/2 + 1];
                char file[MAX_LINE/2 + 1];
                scanf("%s", cmd);
                scanf("%s", arg);
                if (strcmp(cmd, "!!") == 0) {
                    if (history == NULL) {
                        printf("Error");
                    }
                    else {
                        memcpy(cmd, historycmd, sizeof(char)*(MAX_LINE/2+1));
                        memcpy(args, history, sizeof(char)*(MAX_LINE/2+1)*cnt);
                        memcpy(file, historyfile, sizeof(char)*(MAX_LINE/2+1));
                    }
                }
                else {
                    cnt = 0;
                    while (arg) {
                        if (arg == ">" || arg == "<") {
                            scanf("%s", file);
                        }
                        else {
                            args[cnt] = (char *)malloc(sizeof(char)*(MAX_LINE/2+1));
                            strcpy(args[cnt], arg);
                            cnt++;
                            scanf("%s", arg);
                        }
                    }
                }
                execvp(cmd, args);
                if (file != NULL) {
                    int fd = open(file, O_CREAT|O_RDWR); // create the file if not exist, R/W
                    dup2(fd, STDOUT_FILENO);
                }
            }
        	/**
         	 * After reading user input, the steps are:
         	 * (1) fork a child process
         	 * (2) the child process will invoke execvp()
         	 * (3) if command includes &, parent and child will run concurrently
         	 */
    	}

	return 0;
}
