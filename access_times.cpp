#include <algorithm>
#include <sys/time.h>
#include <math.h>
#include <ctime>
#include <random>
#include <iostream>
#include <stdint.h>

using namespace std;

#define KB 1024
#define MB (1024*1024)
#define NUM_ACCESSES 10000
#define NUM_ITER 20

uint64_t get_cycles_start() {
	unsigned cycles_high, cycles_low;
	asm volatile ("cpuid\n\t"
		      "rdtsc\n\t"
		      "mov %%edx, %0\n\t"
		      "mov %%eax, %1\n\t"
		      : "=r" (cycles_high), "=r" (cycles_low)
		      :: "%rax", "%rbx", "%rcx", "%rdx");
	return (((uint64_t) cycles_high << 32) | cycles_low);
}

uint64_t get_cycles_end() {
	unsigned cycles_high, cycles_low;
	asm volatile ("rdtscp\n\t"
		      "mov %%edx, %0\n\t"
		      "mov %%eax, %1\n\t"
		      "cpuid\n\t"
		      : "=r" (cycles_high), "=r" (cycles_low)
		      :: "%rax", "%rbx", "%rcx", "%rdx");
	return (((uint64_t) cycles_high << 32) | cycles_low);
}


int main() {
	//Compute the memory access latencies
	int size = 1*KB;
	srand((unsigned) time(0));
	for(int i=0;i<NUM_ITER;i++) {
		uint64_t *ptr = (uint64_t *) malloc(size * sizeof(uint64_t));
		//Creating a linked list sort of a feature
		for(int i=0;i<size;i++) {
			*(ptr+i) = (uint64_t) (i+1);
		}
		*(ptr + size-1) = 0;
		//Swapping them to create a randomly linked list
		for(int i=1; i<=size-1; i++) {
			int j = (rand() % (size - i));
			std::swap(*(ptr+i), *(ptr+j));
		}
		uint64_t startTime = get_cycles_start();
		for(int i=0;i<NUM_ACCESSES;i++) {
			int accessIndex = rand() % size;
			uint64_t temp = *(ptr + accessIndex);
			//temp = temp & (0x7FFFFFFF);
			//*(ptr + accessIndex) = temp;	
		}
		uint64_t endTime = get_cycles_end();
		cout<<"Size: "<<size<<" Time difference: "<<(endTime - startTime)/NUM_ACCESSES;
		cout<<"\n";
		size *= 2;
	}
		
	return 0;
}
