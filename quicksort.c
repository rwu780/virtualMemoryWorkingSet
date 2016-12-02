#include <stdio.h>
#include <stdlib.h>
#define SIZE 100000

int parition(int begin, int end);
void quicksort(int begin, int end);
void swap(int left, int right);

int main(int argc, char const *argv[]){

	int pSize = atoi(argv[1]);
	int wSize = atoi(argv[2]);

	init(pSize, wSize);
	int i;
	
	for(i = 0; i<SIZE; i++){
		unsigned ran=(unsigned int)lrand48();
		put(i, ran);
	}

	quicksort(0, SIZE-1);
	
	done();
	return 0;
}

void quicksort(int begin, int end){
	if(end > begin){
		int pivot = parition(begin, end);
		quicksort(begin, pivot-1);
		quicksort(pivot+1, end);
	}
}


int parition(int begin, int end){
	unsigned int pivotIndex = (begin+end)/2;
	unsigned int pivot = get(pivotIndex);

	swap(end, pivotIndex);

	int endIndex = end;
	end = end -1;

	while(begin<=end){
		if(get(begin)>=pivot && get(end)<pivot){
			swap(begin, end);
		}
		if(get(end)>=pivot){
			end--;		
		}
		if(get(begin)<pivot){
			begin++;
		}
	}
	swap(begin, endIndex);
	return begin;
}

void swap(int left, int right){
	unsigned int temp = get(left);
	put(left, get(right));
	put(right, temp);
}

