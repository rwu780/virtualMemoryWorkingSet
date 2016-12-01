heapsort:heapsort.o hashtable.o simulator.o
	gcc heapsort.o hashtable.o simulator.o -o heapsort.out

quicksort: quicksort.o hashtable.o simulator.o
	gcc quicksort.o hashtable.o simulator.o -o quicksort.out

quicksort.o:
	gcc -c quicksort.c
heapsort.o:
	gcc -c heapsort.c

hashtable.o:
	gcc -c hashtable.c
simulator.o:
	gcc -c simulator.c

clean:
	rm -f *.o *.out *.log