#include "tlb.h"
#include <string.h>
#include <stdlib.h>

#define MASK 255

int get_page(int addr) {
    return (addr>>8)&MASK;
}

int get_offset(int addr) {
    return (addr) & MASK;
}


int main(int argc, char **args){
    printf("123");

    int tlb_hit = 0, page_fault = 0;
    int total_addr_cnt = 0;
    int addr;
    int mod = 0; // default: LRU

    // no input
    if (argc < 2) return -1;

    // open files
    FILE* addr_file = fopen(args[1], "r");
    FILE* backing_store = fopen("./BACKING_STORE.bin", "rb");

    // select mod
    mod = atoi(args[2]);
    if (mod != 0 && mod != 1) return -1;

    // initialize all
    init();

    // read addr
    while (fscanf(addr_file, "%d", &addr) != EOF) {
        int page = get_page(addr);
        int frame = table(page, mod, &page_fault, &tlb_hit, backing_store, total_addr_cnt);
        int offset = get_offset(addr);
        total_addr_cnt++;
        int physical_addr = (frame<<8)+offset;
        printf("Virual Address: %d Physical Address: %d Value: %d\n", addr, physical_addr, get_content(physical_addr));
    }
    
    // close files
    fclose(addr_file);
    fclose(backing_store);

    // display result
    printf("TLB hit rate: %f%%\n", tlb_hit*1.0/total_addr_cnt);
    printf("Page fault rate: %f%%\n", page_fault*1.0/total_addr_cnt);

    return 0;
}