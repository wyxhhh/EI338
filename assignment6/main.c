#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "banker.h"

#define SIZE    100


int main(int argc, char *argv[])
{
    FILE *in;
    char *temp;
    char info[SIZE];

    init(argv);

    in = fopen("infile.txt","r");
    
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
        fgets(info,SIZE,in);
        temp = strdup(info);
        int max_tmp[NUMBER_OF_RESOURCES];
        for (int j = 0; j < NUMBER_OF_RESOURCES; ++j)
            max_tmp[j] = atoi(strsep(&temp, ","));

        set_max(i, max_tmp);

        free(temp);
    }

    fclose(in);

    auto_calc();

    char operand[3];
    printf("> ");
    scanf("%s", operand);
    while (strcmp(operand, "*") != 0) {
        int alloc_tmp[NUMBER_OF_RESOURCES];
        int customer_num;
        scanf("%d", &customer_num);
        for (int j = 0; j < NUMBER_OF_RESOURCES; ++j)
            scanf("%d", &alloc_tmp[j]);

        int err = 0;
        if (strcmp(operand, "RQ") == 0)
            err = request_resources(customer_num, alloc_tmp);
        else if (strcmp(operand, "RL") == 0)
            release_resources(customer_num, alloc_tmp);
        else
            err = -4;
        
        switch (err)        // error case
        {
        case -1:
            printf("Resource not enough!");
            return err;
        
        case -2:
            printf("Too much resource allocated!");
            return err;
        
        case -3:
            printf("Unsafe state reached!");
            return err;

        case -4:
            printf("No such operand!");
            break;
        
        default:
            break;
        }

        printf("> ");
        scanf("%s", operand);
    }

    print_result();

    return 0;
}

