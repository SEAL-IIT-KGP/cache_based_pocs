#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ARRAY_SIZE 100

static inline uint64_t rdtsc() {
    __asm__ __volatile__("mfence");
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    __asm__ __volatile__("mfence");
    return ((uint64_t)hi << 32) | lo;
}

void flush(void *p) { asm volatile("clflush 0(%0)\n" : : "c"(p) : "rax");  }

int main(){

    // Allocate array
    int *array = (int *)malloc(ARRAY_SIZE * sizeof(int));
    if (!array) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Access memory multiple times to ensure
    // data is in L1d cache
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < ARRAY_SIZE; j++)
            array[i];
    }

    uint64_t start, end;
    int value;

    // Access the array and time it.
    for(int i = 0; i < ARRAY_SIZE; i++){
        flush(&array[i]); // Flush the data from cache
        start = rdtsc();
        value = array[i];
        end = rdtsc();

        printf("Access time: %ld\n", (end - start));
    }
}
