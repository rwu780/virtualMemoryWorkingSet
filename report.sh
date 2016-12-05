#!/bin/bash


echo "Assignment3"
echo "Enter pageSize windowSize sortAl"

rm *.csv

for pSize in 64 128 256 512
do
	for winSize in 128 256 512 1024 2048 4096 8192 16384
	do
		make quicksort
		./quicksort.out $pSize $winSize
		make clean
		make heapsort
		./heapsort.out $pSize $winSize
	done
done
make clean