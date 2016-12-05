heapsort:heapsort.o simulator.o
	gcc heapsort.o simulator.o -o heapsort

quicksort: quicksort.o simulator.o
	gcc quicksort.o simulator.o -o quicksort

quicksort.o:
	gcc -c quicksort.c
heapsort.o:
	gcc -c heapsort.c

simulator.o:
	gcc -c simulator.c

clean:
	rm -f *.o *.out *.log heapsort quicksort