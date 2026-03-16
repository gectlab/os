/*
Experiment 10) 
Simulate the address translation in the paging scheme as follows: The program receives
three command-line arguments in the order

● size of the virtual address space (in megabytes)
● page size (in kilobytes)
● a virtual address (in decimal notation)

The output should be the physical address corresponding to the virtual address in <frame
number, offset> format. You may assume that the page table is implemented as an array
indexed by page numbers. (NB: If the page table has no index for the page number
determined from the virtual address, you may just declare a page table miss!)
*/
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    if (argc != 4) {
        printf("Usage: %s <VAS_MB> <PageSize_KB> <VirtualAddress>\n", argv[0]);
        return 1;
    }

    // Read inputs
    long vas_mb = atol(argv[1]);          // Virtual address space size (MB)
    long page_kb = atol(argv[2]);         // Page size (KB)
    long virtual_address = atol(argv[3]); // Virtual address (decimal)

    // Convert to bytes
    long vas_bytes = vas_mb * 1024L * 1024L;
    long page_bytes = page_kb * 1024L;

    // Number of pages
    long num_pages = vas_bytes / page_bytes;

    // Create page table (identity mapping: page i -> frame i)
    long *page_table = (long *)malloc(num_pages * sizeof(long));
    if (page_table == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    for (long i = 0; i < num_pages; i++) {
        page_table[i] = i; // simple mapping
    }

    // Calculate page number and offset
    long page_number = virtual_address / page_bytes;
    long offset = virtual_address % page_bytes;

    // Check page table bounds
    if (page_number >= num_pages) {
        printf("Page Table Miss!\n");
    } else {
        long frame_number = page_table[page_number];
        printf("Physical Address = <%ld, %ld>\n", frame_number, offset);
    }

    free(page_table);
    return 0;
}
