#include <iostream>
#include <bits/stdc++.h>
#include <cstdlib>
#include <stdint.h>
#include <random>
#include <ctime>
#include <math.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

#define KB 1024
#define MB (1024 * 1024)
#define NUM_ACCESSES 100000000
#define NUM_ITER 20
#define SECONDS_PER_NS 1000000000

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

uint64_t gettime(void) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (((uint64_t)tv.tv_sec) * 1000000 + ((uint64_t)tv.tv_usec * 1000));
}


uint64_t begin_roi() {
	uint64_t usec = gettime();
	return usec;
}

uint64_t end_roi() {
	uint64_t usec = gettime();
	return usec;
}

class CacheLookAlike {
	public:
		int next[16];
};
	
int main() {
	//Compute the memory access latencies
	int size = 16;
	for(int i=0;i<NUM_ITER;i++) {
		int *arr = (int *) malloc(size * sizeof(int));
		//Size of the array!
		for(int l=0;l<=size-1;l++) {
			*(arr + l) = l;
		}
		
		srand(time(NULL));
		for(int k=size-1;k>0;k--) {
			int j = (rand() % k);
			int temp = *(arr + k);
			*(arr + k) = *(arr + j);
			*(arr + j) = temp;
		}
		
		CacheLookAlike *Cache_DS = (CacheLookAlike *) malloc(size * sizeof(CacheLookAlike));

		//Creating a linked list type of feature
		for(int j=0;j<size;j++) {
			Cache_DS[j].next[0] = *(arr+j);
		}

		//Copying the values from the first column to every column
		for(int k=0;k<size;k++) {
			for(int j=1;j<16;j++) {
				(*(Cache_DS+k)).next[j] = (*(Cache_DS+k)).next[0];
			}
		}
		
		//Swapping the row with some other row
		//Starting from the head of the list
		int count = 0;
		int data = (*(Cache_DS+0)).next[0];
		//uint64_t startTime = get_cycles_start();
		struct timeval startTime, endTime;
		gettimeofday(&startTime, 0);
		while (count < NUM_ACCESSES) {
			int curr = data;
			data = (*(Cache_DS+curr)).next[0];
			//cout<<data<<"\n";
			++count;
		}
		gettimeofday(&endTime,0);
		//uint64_t endTime = get_cycles_end();
		cout<<"size: "<<size*64<<"       Latency:    "<<(double) ((endTime.tv_sec - startTime.tv_sec)*SECONDS_PER_NS + (endTime.tv_usec - startTime.tv_usec)*1000)/NUM_ACCESSES;
		cout<<"\n";
		free(Cache_DS);
		size *= 2;
	}

	return 0;
}	
		
		
