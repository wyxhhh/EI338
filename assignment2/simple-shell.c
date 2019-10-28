/**
 * By Yiwen Song, 10.28
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_LINE		80 /* 80 chars per line, per command */
#define READ_END 0
#define WRITE_END 1

int main(void)
{
	char *args[MAX_LINE/2 + 1];	/* command line (of 80) has max of 40 arguments */
    int should_run = 1;
    char buffer[MAX_LINE + 1];
    char history_buffer[MAX_LINE + 1] = "";

    while (should_run){
        printf("osh>");
        fflush(stdout);

        // load buffer from stdin
        fgets(buffer, MAX_LINE, stdin);
        if (strlen(buffer) > 1 && buffer[strlen(buffer)-1] == '\n')
            buffer[strlen(buffer)-1] = '\0';
        
        if (strcmp(buffer, "!!") == 0)
            if (!strcmp(history_buffer, "")) {
                printf("No commands in history\n");
                continue;
            }
            else
                strncpy(buffer, history_buffer, MAX_LINE);
        else
            strncpy(history_buffer, buffer, MAX_LINE);


        // initialize the parameters
        int n_args = 0, n_pipes = 0;
        char *tmp = strtok(buffer, " ");
        bool wait_for_child = true, if_error = false, if_pipe = false;
        int to_file = 0, fd = -1;
        char fileName[MAX_LINE];
        int pipe_separator;
        

        /* fetch cmd in buffer into array args.
        use strtok to separate the buffer by ' '.
        */
        while (tmp != NULL) {
            if (strlen(tmp) < 1) {
                tmp = strtok(NULL, " ");
                continue;
            }
            else if (to_file) {
                strcpy(fileName, tmp);
                fileName[strlen(tmp)] = '\0';
            }
            else if (!strcmp(tmp, ">") || !strcmp(tmp, "<")) {
                to_file = tmp[0];
            }
            else if (!strcmp(tmp, "&")) {
                wait_for_child = false;
            }
            else if (!strcmp(tmp, "|")) {
                if (strlen(fileName) && to_file == '>') {
                    if_error = true;    // can't pipe after output file rediction
                }
                if_pipe = true;
                pipe_separator = n_args; // end point of pipe
                n_pipes++;
            }
            else {
                args[n_args] = (char*)malloc(sizeof(char)*(strlen(tmp)+1));
                strcpy(args[n_args], tmp);
                args[n_args][strlen(tmp)] = '\0';
                n_args++;
            }

            tmp = strtok(NULL, " ");
        }
        args[n_args] = NULL;

        if (if_error || n_args == 0 || args[0][0] == '\n') {
            continue;   // no command in command line or other error occurs, continue
        }

        if (!strcmp(args[0], "exit"))
            exit(0); // exit if cmd is "exit"

        // open file if there is file operation
        if (to_file) {
            if (to_file == '>')
                fd = open(fileName, O_RDWR|O_CREAT, 0644);
            else
                fd = open(fileName, O_RDWR, 0644);
            if (fd == -1) {
                printf("Error open files, exiting...\n");
                continue;
            }
        }

        pid_t pid = fork();
        if (pid < 0) {
            printf("Fork child process failed.\n");
            continue;
        }
        else if (pid == 0) {    // child process
            if (if_pipe) {
                printf("pipe number: %d\n", n_pipes);
                int pipe_fd[2];
                for (int i = 0; i < n_pipes; ++i) {
                    char* tmp_args1[MAX_LINE/2+1];
                    char* tmp_args2[MAX_LINE/2+1];
                    int pipe_start = 0;

                    // copy command
                    for (int j = pipe_start; j < pipe_separator; ++j) {
                        tmp_args1[j-pipe_start] = (char*)malloc(sizeof(char)*(strlen(args[j])+1));
                        strcpy(tmp_args1[j-pipe_start], args[j]);
                        printf("Arg %d/%d: %s\n", j, pipe_separator, tmp_args1[j-pipe_start]);
                    }
                    tmp_args1[pipe_separator - pipe_start] = NULL;

                    for (int j = pipe_separator; j < n_args; ++j) {
                        tmp_args2[j-pipe_separator] = (char*)malloc(sizeof(char)*(strlen(args[j])+1));
                        strcpy(tmp_args2[j-pipe_separator], args[j]);
                        printf("Arg %d/%d: %s\n", j, n_args, tmp_args2[j-pipe_separator]);
                    }
                    tmp_args2[n_args - pipe_separator] = NULL;
                    
                    if (pipe(pipe_fd) == -1) {
                        fprintf(stderr, "Pipe failed.\n");
                        continue;
                    }

                    pid_t pipe_pid = fork();
                    if (pipe_pid < 0) {
                        fprintf(stderr, "Pipe fork failed, exiting ...\n");
                        continue;
                    }
                    if (pipe_pid == 0) {
                        close(pipe_fd[READ_END]);
                        dup2(pipe_fd[WRITE_END], STDOUT_FILENO);
                        close(pipe_fd[WRITE_END]);
                        printf("Executing Pipe 1 ...\n");
                        int pipe_exe = execvp(tmp_args1[0], tmp_args1);
                        if (pipe_exe < 0) {
                            printf("Error executing %s, exiting ...\n", tmp_args1[0]);
                            exit(0);
                        }
                        should_run = 0;
                    }
                    else {
                        close(pipe_fd[WRITE_END]);
                        dup2(pipe_fd[READ_END], STDIN_FILENO);
                        close(pipe_fd[READ_END]);
                        int pipe_exe = execvp(tmp_args2[0], tmp_args2);
                        if (pipe_exe < 0) {
                            printf("Error executing %s, exiting ...\n", tmp_args2[0]);
                            exit(0);
                        }
                        should_run = 0;
                        printf("Executing Pipe 2...\n");
                    }
                }
            }
            else {
                // input
                if (to_file) {
                    int dup_file;
                    if (to_file == '<')
                        dup_file = dup2(fd, STDIN_FILENO);
                    else
                        dup_file = dup2(fd, STDOUT_FILENO);
                    if (dup_file == -1) {
                        printf("Error while dup process, exiting ...\n");
                        close(fd);
                        exit(0);
                        continue;
                    }
                }

                if (fork() == 0 || !wait_for_child) {
                    int exe = execvp(args[0], args);
                    if (exe < 0) {
                        printf("Error executing %s, exiting ...\n", args[0]);
                        exit(0);
                    }
                }	size_t children[MAX_LINE];

                should_run = 0; // do not request stdin in sub-process
                exit(0); // exit child process
            }
        }
        else {  // parent process
            if (wait_for_child)
                waitpid(pid, NULL, 0);
            if (fd != -1) {
                close(fd);  // check if all the files are closed
            }
        }

    }

	return 0;
}
