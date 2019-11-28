#include <stdlib.h>
#include <string.h>
#include "tlb.h"


// memory
char memory[MEM_SIZE];
int valid[PAGE_SIZE];

// tlb
Block tlb[TLB_SIZE];
int tlb_freq[TLB_SIZE];
int tlb_instime[TLB_SIZE];

// page table
int page_table[PAGE_SIZE];

// index for start of free space in memory
int free_mem;

// used frequency or load time in memory
int used_freq[PAGE_SIZE];
int load_time[PAGE_SIZE];


void init() {
    // initialize tlb value
    for (int i = 0; i < TLB_SIZE; ++i) {
        tlb[i].page = -1;
        tlb[i].frame = -1;
        tlb_freq[i] = 0;
        tlb_instime[i] = -1;
    }

    // initialize page table value
    for (int i = 0; i < PAGE_SIZE; ++i) {
        page_table[i] = -1;
    }

    // initialize valid bits
    for (int i = 0; i < PAGE_SIZE; ++i) {
        valid[i] = 0;
    }

    // initialize memory
    free_mem = 0;
    for (int i = 0; i < PAGE_SIZE; ++i) {
        used_freq[i] = 0;
        load_time[i] = -1;
    }
    
    return;
}


int search_tlb(int page) {
    for (int i = 0; i < TLB_SIZE; ++i)
        if (tlb[i].page == page)
            return tlb[i].frame;
    return -1;
}

int search_page_table(int page) {
    return page_table[page];
}

void swap_frame_in_mem(FILE *backing_store, int page, int frame) {
    fseek(backing_store, page*FRAME_SIZE, SEEK_SET);
    fread(memory+frame*FRAME_SIZE, FRAME_SIZE, 1, backing_store);
}

int select_victim(int mod) {
    int pos = 0;
    switch (mod)
    {
    case 0:
        for (int i = 1; i < PAGE_SIZE; ++i)
            if (used_freq[i] < used_freq[pos])
                pos = i;
        break;
    
    case 1:
        for (int i = 1; i < PAGE_SIZE; ++i)
            if (load_time[i] < load_time[pos])
                pos = i;
        break;
    
    default:
        break;
    }

    return pos;
}

void update_TLB(int page, int frame, int mod, int time) {
    int pos = 0;
    switch (mod)
    {
    case 0:
        for (int i = 1; i < TLB_SIZE; ++i)
            if (tlb_freq[i] < tlb_freq[pos] && page_table[i] != -1)
                pos = i;
        break;
    
    case 1:
        for (int i = 1; i < TLB_SIZE; ++i)
            if (tlb_instime[i] < tlb_instime[pos] && page_table[i] != -1)
                pos = i;
    
    default:
        break;
    }
    tlb[pos].page = page;
    tlb[pos].frame = frame;
    tlb_freq[pos] = 1;
    tlb_instime[pos] = time;
}

int table(int page, int mod, int *page_fault, int *tlb_hit, FILE *backing_store, int time) {
    int frame = search_tlb(page);
    if (frame < 0) {
        if (valid[page] == 1) {
            frame = search_page_table(page);
            used_freq[page]++;
        }
        else {
            (*page_fault)++;
            if (free_mem < FRAME_NUM) {
                // enough space in memory, give a new frame for this page
                frame = free_mem;
                page_table[page] = free_mem++;
                swap_frame_in_mem(backing_store, page, frame);
            }
            else {
                // not enough space in memory
                int victim_page = select_victim(mod);
                frame = page_table[victim_page];
                valid[victim_page] = 0;
                swap_frame_in_mem(backing_store, page, frame);
                used_freq[victim_page] = 0;
                load_time[victim_page] = -1;
                used_freq[page] = 1;
                load_time[page] = time;
            }

            // set the page to be valid
            valid[page] = 1;
        }
        update_TLB(page, frame, mod, time);
    }
    else {
        tlb_freq[page]++;
        (*tlb_hit)++;
    }
    return frame;
}

int get_content(int addr) {
    return memory[addr];
}