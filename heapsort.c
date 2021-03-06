#include <stdio.h>
#include <stdlib.h>
#define SIZE 5000

void max_heapify(int start, int end);
void heap_sort(int size);
void swap(int index1, int index2);
void process();
int PSIZE;
int WSIZE;

int main(int argc, char const *argv[])
{
    PSIZE = atoi(argv[1]);
    WSIZE = atoi(argv[2]);
    process();
    return 0;
}
void process(){

    init(PSIZE, WSIZE);

    int i;
    for(i = 0; i<SIZE; i++){
        unsigned int ran = (unsigned int)lrand48();
        put(i,ran);
    }
    heap_sort(SIZE);
    done();
}

void swap(int index1, int index2){
    unsigned int temp = get(index1);
    put(index1, get(index2));
    put(index2, temp);
}

void max_heapify(int start, int end){
    
    int parent = start;
    int son = parent*2 + 1;

    while(son <= end){
        if((son+1) <= end && get(son)<get(son+1)){
            son++;
        }
        if(get(parent)>get(son)){
            return;
        }
        else{
            swap(parent, son);
            parent = son;
            son = parent*2 +1;
        }
    }
}

void heap_sort(int size){
    int i;
    for(i = size/2 - 1; i>=0;i--){
        max_heapify(i, size-1);
    }
    for(i = size-1;i>0;i--){
        swap(0,i);
        max_heapify(0, i-1);
    }
}

