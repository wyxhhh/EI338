#include "alloc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// processes
Node* process_head;

void init(int total_mem) {
    process_head = (Node*)malloc(sizeof(Node));
    Node *block = (Node*)malloc(sizeof(Node));
    block->memStart = 0; block->memEnd = total_mem;
    block->memSize = total_mem;
    strcpy(block->name, "-");
    block->next = NULL;
    process_head->next = block;
}

int in_memory(char *tsk_name) {
    Node *tmp = process_head->next;
    while (tmp != NULL && strcmp(tmp->name, tsk_name) != 0)
        tmp = tmp->next;
    return (tmp == NULL);
}

int request_memory(char* tsk, long byte, char mod) {
    Node *block = NULL;
    Node* tmp = process_head->next;
    switch (mod)
    {
    case 'F':
        // first-fit
        while (tmp != NULL) {
            if (strcmp(tmp->name, "-") == 0 && (tmp->memSize >= byte)) {
                block = tmp; break;
            }
            tmp = tmp->next;
        }
        break;
    
    case 'B': {
        // best-fit
        int best_mem = 100000;
        while (tmp != NULL) {
            if (strcmp(tmp->name, "-") == 0 && (tmp->memSize >= byte) && (tmp->memSize < best_mem)) {
                block = tmp; best_mem = tmp->memSize;
            }
            tmp = tmp->next;
        }
        break;
    }
    
    case 'W': {
        //worst-fit
        int worst_mem = -1;
        while (tmp != NULL) {
            if (strcmp(tmp->name, "-") == 0 && (tmp->memSize >= byte) && (tmp->memSize > worst_mem)) {
                block = tmp; worst_mem = tmp->memSize;
            }
            tmp = tmp->next;
        }
        break;
    }
    
    default:
        break;
    }
    
    // if block == NULL, can't find such block, return err
    if (block == NULL)
        return -1;
    
    printf("block [%d : %d]\n", block->memStart, block->memEnd);
    
    Node* target = malloc(sizeof(Node));
    strcpy(target->name, "-");
    strcpy(block->name, tsk);
    target->memEnd = block->memEnd;
    block->memEnd = block->memStart + byte;
    block->memSize = byte;
    target->memStart = block->memEnd;
    target->memSize = target->memEnd - target->memStart;
    target->next = block->next;
    block->next = target;
    if (target->memSize == 0) {
        block->next = target->next;
        free(target);
    }

    return 0;
}


int release_memory(char* tsk) {
    Node *tmp = process_head;
    // find the node before it
    while (tmp->next != NULL) {
        if (strcmp(tsk, tmp->next->name) == 0)
            break;
        tmp = tmp->next;
    }
    if (tmp->next == NULL)
        return -1;
    strcpy(tmp->next->name, "-");
    if (strcmp(tmp->name, "-") == 0) {
        // merge two holes
        tmp->memEnd = tmp->next->memEnd;
        tmp->memSize += tmp->next->memSize;
        Node* to_free = tmp->next;
        tmp->next = to_free->next;
        free(to_free);
    }
    tmp = tmp->next;
    if (tmp->next != NULL && strcmp(tmp->next->name, "-") == 0) {
        // merge two holes
        tmp->memEnd = tmp->next->memEnd;
        tmp->memSize += tmp->next->memSize;
        Node* to_free = tmp->next;
        tmp->next = to_free->next;
        free(to_free);
    }
    return 0;

}

void compact_memory() {
    Node *head_tmp = process_head;
    while (head_tmp->next != NULL) {
        Node *tmp = head_tmp->next;
        if (strcmp(tmp->name, "-") == 0) {
            if (tmp->next != NULL && strcmp(tmp->next->name, "-") == 0) {
                // merge two holes
                tmp->memEnd = tmp->next->memEnd;
                tmp->memSize += tmp->next->memSize;
                Node* to_free = tmp->next;
                tmp->next = to_free->next;
                free(to_free);
            }
            else if (tmp->next != NULL) {
                // swap two blocks
                Node *next_tmp = tmp->next;
                next_tmp->memStart = tmp->memStart;
                next_tmp->memEnd = next_tmp->memStart + next_tmp->memSize;
                tmp->memStart = next_tmp->memEnd;
                tmp->memEnd = tmp->memStart + tmp->memSize;

                // swap the nodes
                tmp->next = next_tmp->next;
                head_tmp->next = next_tmp;
                next_tmp->next = tmp;
            }
        }
        head_tmp = head_tmp->next;
    }
}

void status_report() {
    Node *tmp = process_head->next;
    while (tmp != NULL) {
        printf("Addr [%d : %d]\tProcess: %s\n", tmp->memStart, tmp->memEnd, tmp->name);
        tmp = tmp->next;
    }
}
