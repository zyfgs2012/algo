#include <cstdio>
#include "../heap_algo.h"

int main() {
	Heap<int> heap(10);
	for(int i=1;i<=5;i++) {
		heap.push(i*2-1);
		heap.push(i*2);
	}
	for(int i=0;i<10;i++) {
		printf("%d ",heap.pop());
	}
	printf("\n");
	return 0;
}
