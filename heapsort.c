#include "heapsort.h"

int main()
{
    int N;
    printf("Enter a value: ");
    scanf("%d", &N);
    init(128, 1000);

    int i;
    for(i = 0; i<N; i++){
        unsigned int ran = (unsigned int)lrand48();
        put(i,ran);
    }
    display(N);

    heapsort(N);
    display(N);
    done();
    return 0;
}
void swap(int index1, int index2){
    unsigned int temp = get(index1);
    put(index1, get(index2));
    put(index2, temp);
}
void buildHeap(int size){
    int i;
    for(i=1; i<size; i++){
        unsigned int value = get(i);
        int currentNode = i;
        int parentNode = (i-1)/2;

        while(currentNode>0 && value > get(parentNode)){
            swap(parentNode, currentNode);
            currentNode = parentNode;
            parentNode = (parentNode-1)/2;
        }
    }
}

void heapsort(int size){
    buildHeap(size);
    swap(0, size-1);
    if(size >1){
        heapsort(size-1);
    }
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
