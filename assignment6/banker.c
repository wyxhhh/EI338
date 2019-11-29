#include "banker.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* the available amount of each resource */
int available[NUMBER_OF_RESOURCES];
/*the maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
/* the amount currently allocated to each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
/* the remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];


void set_max(int customer_num, int max_resource[]) {
    for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) 
        maximum[customer_num][i] = max_resource[i];
}


void init(char *argv[]) {
    for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
        available[i] = atoi(argv[i+1]);
        for (int j = 0; j < NUMBER_OF_CUSTOMERS; ++j) {
            maximum[j][i] = 0; allocation[j][i] = 0; need[j][i] = 0;
        }
    }
}


void auto_calc() {
    for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
        for (int j = 0; j < NUMBER_OF_CUSTOMERS; ++j) {
            need[j][i] = maximum[j][i] - allocation[j][i];
        }
    }
}

int can_consume(int need_vec[], int aval_vec[]) {
    for (int i = 0; i < NUMBER_OF_RESOURCES; ++i)
        if (need_vec[i] > aval_vec[i])
            return 0;
    return 1;
}

int safe_state(){
    int aval[NUMBER_OF_RESOURCES];
    // copy available to aval
    for (int i = 0; i < NUMBER_OF_RESOURCES; ++i)
        aval[i] = available[i];

    int customer_num = NUMBER_OF_CUSTOMERS;
    int customer_vec[NUMBER_OF_CUSTOMERS];  // if this customer has been served
    // initialize customer vec
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i)
        customer_vec[i] = 1;

    // traverse all customers
    while (customer_num > 0) {
        int flag = 0;   // flag: if any customer has been served in this iteration
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
            if (customer_vec[i] == 1 && can_consume(need[i], aval)) {
                // this customer has been served
                flag = 1;
                customer_vec[i] = 0;
                // return resources
                for (int j = 0; j < NUMBER_OF_RESOURCES; ++j)
                    aval[j] += allocation[i][j];
            }
        }
        if (flag == 0)
            return -1;
    }
    return 0;
}

int request_resources(int customer_num, int request[]) {
    for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
        if (available[i] < request[i])
            return -1;                  // error, not enough for request
        available[i] -= request[i];
        allocation[customer_num][i] += request[i];
        need[customer_num][i] -= request[i];
        if (need[customer_num][i] < 0)
            return -2;                  // error, too much requested
    }
    if (safe_state() < 0)
    return -3;                  // unsafe sate
    return 0;
}


void release_resources(int customer_num, int release[]) {
    for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
        int rel = release[i];
        if (release[i] > allocation[customer_num][i])
            rel = allocation[customer_num][i];
        available[i] += rel;
        allocation[customer_num][i] -= rel;
        need[customer_num][i] += rel;
    }
    return;
}


void print_result() {
    printf("Available:\n");
    for (int i = 0; i < NUMBER_OF_RESOURCES; ++i)
        printf("%d\t", available[i]);
    printf("\n\nMaximum:\n");
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; ++j)
            printf("%d\t", maximum[i][j]);
        printf("\n");
    }
    printf("\nAllocation:\n");
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; ++j)
            printf("%d\t", allocation[i][j]);
        printf("\n");
    }
    printf("\nNeed:\n");
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; ++i) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; ++j)
            printf("%d\t", need[i][j]);
        printf("\n");
    }
}

