typedef struct node {
    char name[10];
    int memStart, memEnd, memSize;
    struct node* next;
} Node;


// initialization
void init(int total_mem);
// if a task is in memory
int in_memory(char *tsk_name);
// process request memory, return -1 if error occurs
int request_memory(char* tsk_name, long byte, char mod);
// process release memory and terminates, return -1 if error occurs
int release_memory(char* tsk_name);
// compact memory, remove segments
void compact_memory();
// print status
void status_report();
