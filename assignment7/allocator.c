#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXINT 100000000;

typedef struct memblock
{
    int low;
    int high;
    char name[10];
    struct memblock *next;
}memblock_t;

memblock_t *head;

void init(int space);
int valid(int space);  //check whether memory allocation is possible
int task_in(char name[]); //check whether a task in memory
void request_mem(char name[], int required_size, char opt);
void release_mem(char name[]);
void mergemem();
void printstate();

// first-fit, best-fit, worst-fit, returns the address of the target node.
memblock_t* ffit(int required_size);
memblock_t* bfit(int required_size);
memblock_t* wfit(int required_size);

void compaction();

int main(int argc, char **args){
    int size = 0;
    char comm[64];
    char task[10];
    int required_size;
    char opt;
    if (argc > 1)
        size = atoi(args[1]);
    init(size);
    while(1){
        printf("allocator>");
        scanf("%s", comm);
        if(comm[0] == 'X') break;
        if(comm[0] == 'C') {
            compaction();
            continue;
        } 
        switch (comm[1])
        {
        case 'X':
            break;
        case 'Q':
            scanf("%s%d%c%c", task, &required_size, &opt, &opt);
            if(valid(required_size)){
                request_mem(task, required_size, opt);
            }
            else printf("Memory Exceed!\n");
            break;
        case 'L':
            scanf("%s", task);
            if(task_in(task)) {
                release_mem(task);
                }
            else printf("No Such Task!\n");
            break;
        case 'T':
            printstate();
            break;
        default:
            printf("Invalid Command!\n");
        }

    }
    return 0;
}

void init(int space){
    head = (memblock_t*)malloc(sizeof(memblock_t));
    memblock_t *block = (memblock_t*)malloc(sizeof(memblock_t));
    block->low = 0;
    block->high = space - 1;
    strcpy(block->name, "Unused");
    block->next = NULL;
    head->next = block;
}

int valid(int space){
    memblock_t *tmp;
    tmp = head->next;
    while(tmp!=NULL){
        if(strcmp(tmp->name, "Unused") == 0 && (tmp->high - tmp->low + 1) >= space) return 1;
        tmp = tmp->next;
    }
    return 0;
}

memblock_t* ffit(int required_size){
    memblock_t *tmp;
    tmp = head->next;
    while(tmp!= NULL){
        // find the first available block
        if(strcmp(tmp->name, "Unused") == 0 && (tmp->high - tmp->low + 1) >= required_size) return tmp;
        tmp = tmp->next;
    }
    printf("%d\n", tmp->low);
    return NULL;
}

memblock_t* bfit(int required_size){
    memblock_t *tmp, *best;
    int diff = MAXINT;
    best = head->next;
    tmp = head->next;
    while(tmp!= NULL){
        // find the best-fit block
        if(strcmp(tmp->name, "Unused") == 0 && (tmp->high - tmp->low + 1) >= required_size){
            if(tmp->high - tmp->low + 1 - required_size < diff){
                diff = tmp->high - tmp->low + 1 - required_size;
                best = tmp;
            }
        }
        tmp = tmp->next;
    }
    return best;
}

memblock_t* wfit(int required_size){
    memblock_t *tmp, *best;
    int diff = 0;
    best = head->next;
    tmp = head->next;
    while(tmp!= NULL){
        // find the largestest block
        if(strcmp(tmp->name, "Unused") == 0 && (tmp->high - tmp->low + 1) >= required_size){
            if(tmp->high - tmp->low + 1 - required_size > diff){
                diff = tmp->high - tmp->low + 1 - required_size;
                best = tmp;
            }  
        }
        tmp = tmp->next;
    }
    return best;
}

void request_mem(char name[], int required_size, char opt){
    memblock_t *target, *dst;
    switch(opt){
        case 'F':
            target = ffit(required_size);
            break;
        case 'B':
            target = bfit(required_size);
            break;
        case 'W':
            target = wfit(required_size);
            break;
        default:
            target = (memblock_t*)malloc(sizeof(memblock_t));
            target = NULL;
            break;
    }
    // we intend to insert destiny node before target node
    dst = (memblock_t*)malloc(sizeof(memblock_t));
    dst->next = target->next;
    target->next = dst;
    // insert the node
    dst->high = target->high;
    target->high = target->low + required_size - 1;
    dst->low = target->low + required_size;
    strcpy(dst->name, "Unused");
    strcpy(target->name, name);
    // swap information 
}

void release_mem(char name[]){ // release the block, change the block name to "Unused".
    memblock_t *tmp;
    tmp = head->next;
    while(tmp != NULL){
        if(strcmp(name, tmp->name) == 0){
            strcpy(tmp->name, "Unused");
            break;
        }
        tmp = tmp->next;
    }
    mergemem();
}

void mergemem(){ // merge adjacent unused memroy blocks.
    memblock_t *tmp;
    memblock_t *suspect;
    tmp = head->next;
    while(tmp != NULL){
        if(strcmp(tmp->name, "Unused") == 0){
            suspect = tmp->next;
            if(suspect == NULL) break;
            while(strcmp(suspect->name, "Unused") == 0){
                // delete nodes
                memblock_t *victim = suspect;
                tmp->next = suspect->next;
                tmp->high = suspect->high;
                suspect = suspect->next;
                free(victim); // free the next unused node
                if (suspect == NULL) break;
            }
        }
        tmp = tmp->next;
    }
}

void compaction(){
    int offset = 0;
    memblock_t *iter = head;
    while (iter->next != NULL)
    {
        if(iter->next->name[0] == 'U')
        {
            // update last hole
            if(iter->next->next == NULL){
                iter->next->low -= offset;
                break;
            }
            // delete hole
            else{
                offset += iter->next->high - iter->next->low + 1;
                memblock_t *tmp;
                tmp = iter->next;
                iter->next = tmp->next;
                free(tmp);
            }
        }
        // update address
        else
        {
            iter = iter->next;
            iter->low -= offset;
            iter->high -= offset;
        }
    }
}

int task_in(char name[]){
    memblock_t *tmp;
    tmp = head->next;
    while(tmp != NULL){
        if(strcmp(tmp->name, name) == 0) return 1;
        tmp = tmp->next;
    }
    return 0;
}

void printstate(){
    memblock_t *tmp;
    tmp = head->next;
    while(tmp != NULL){
        printf("Address [%d : %d] Process: %s\n", tmp->low, tmp->high, tmp->name);
        tmp = tmp->next;
    }
}