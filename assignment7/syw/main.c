#include "alloc.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **args) {
    int mem_size;
    char operator[5], name[10];
    char mod;
    int process_size;

    mem_size = atoi(args[1]);
    init(mem_size);

    while (1) {
        printf("allocator> ");
        scanf("%s", operator);
        if (strcmp(operator, "X") == 0) break;
        else if (strcmp(operator, "C") == 0) {
            compact_memory();
        }
        else if (strcmp(operator, "RQ") == 0) {
            scanf("%s %d %c", name, &process_size, &mod);
            if (request_memory(name, process_size, mod) < 0)
                printf("Error: Memory Limit Exceed.\n");
        }
        else if (strcmp(operator, "RL") == 0) {
            scanf("%s", name);
            if (release_memory(name) < 0)
                printf("Error: No such process.\n");
        }
        else if (strcmp(operator, "STAT") == 0) {
            status_report();
        }
        else {
            printf("Error: No such command.");
        }
    }
    return 0;
}
