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
#include <pthread.h>

using namespace std;

#define KB 1024
#define MB (1024 * 1024)
#define NUM_ACCESSES 100000000
#define NUM_ITER 20
#define SECONDS_PER_NS 1000000000

uint64_t gettime(void) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (((uint64_t)tv.tv_sec) * 1000000 + ((uint64_t)tv.tv_usec * 1000));
}

class CacheLookAlike {
	public:
		int next[16];
};

struct pointers {
	int data;
	int ThreadID;
};

void* accessCache(void* data) {
	int count = 0;
	int *ptr =(int *) data;
	int act_data = 0;
	while (count < NUM_ACCESSES) {
		act_data = *(ptr + (act_data << 4));
		++count;
	}
	pthread_exit(NULL);
}

	
int main(int argc, char *argv[]) {
	////cout<<"Int main";
	int numThreads = atoi(argv[1]);
	////cout<<numThreads;
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
		//cout<<"Reached here!\n";
		
		CacheLookAlike* Cache_DS = (CacheLookAlike* ) malloc(size * sizeof(CacheLookAlike));

		if(Cache_DS == NULL) {
			cout<<"Couldn't allocate the proper memory to the Cache!\n";
		}

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

		struct timeval startTime, endTime;
		pthread_t threadID[numThreads];
		gettimeofday(&startTime, 0);
		int *data;
		for(int k = 0; k < numThreads; ++k) {
			data = &(Cache_DS[0]).next[k];
			pthread_create(&threadID[k], NULL, accessCache, data);
		}

		for(int k=0;k<numThreads;k++) {
			pthread_join(threadID[k], NULL);
		}
		gettimeofday(&endTime,0);
		cout<<"Size: "<<size*64<<" Time difference: "<<(double) ((endTime.tv_sec - startTime.tv_sec)*SECONDS_PER_NS + (endTime.tv_usec - startTime.tv_usec)*1000)/NUM_ACCESSES;
		cout<<"\n";
		free(Cache_DS);
		size *= 2;
	}

	return 0;
}	
		
		
