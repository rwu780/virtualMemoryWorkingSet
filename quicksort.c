#include "quicksort.h"

int main(){
	int size, i;
	printf("Enter a value: ");
	scanf("%d", &size);
	//N = MAX;
	init(128, 1000);
	for(i = 0; i<size; i++){
		unsigned ran=(unsigned int)lrand48();
		put(i, ran);
	}
	display(size);
	quicksort(0, size-1);
	display(size);

	done();
	return 0;
}
/**
quick sort function, require three inputs
1. array
2. begin index
3. end index
**/
void quicksort(int begin, int end){
	if(end > begin){
		int pivot = parition(begin, end);
		quicksort(begin, pivot-1);
		quicksort(pivot+1, end);
	}
}

/**
Create the parition for quicksort
**/
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
/**
Swap element, require 3 inputs:
1. array
2. first index
3. second index
**/
void swap(int left, int right){
	unsigned int temp = get(left);
	put(left, get(right));
	put(right, temp);
}

/**
Print out the current data, require two inputs
1. array
2. array size
**/
void display(int size) {

	printf("==========\n");
   int i;
   // navigate through all items 
   for(i = 0;i<size;i++) {
      printf("%d: %d \n",i+1, get(i));
   }
   printf("==========\n");
}
