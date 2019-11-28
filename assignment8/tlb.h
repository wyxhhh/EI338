#include <stdio.h>

# define TLB_SIZE 16
# define FRAME_SIZE 256
# define PAGE_SIZE 256

# define FRAME_NUM 256
# define MEM_SIZE (FRAME_NUM*FRAME_SIZE)


typedef struct tlb_block {
    int frame;
    int page;
} Block;


// initialize all parameters
void init();
// search logical address in tlb
int search_tlb(int page);
// if can't find logical address in tlb, find frame in page table
int search_page_table(int page);
// if frame is not valid, swap frame
void swap_frame_in_mem(FILE *backing_store, int page, int frame);
// select victim by LRU (mod=0) or FIFO (mod=1)
int select_victim(int mod);
// update tlb by LRU (mod=0) or FIFO (mod=1)
void update_TLB(int page, int frame, int mod, int time);

// all in one: table function
int table(int page, int mod, int *page_fault, int *tlb_hit, FILE *backing_store, int time);

// get memory content
int get_content(int addr);
