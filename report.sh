#!/bin/bash


echo "Assignment3"
echo "Enter pageSize windowSize sortAl"

for pSize in 64 128 256 512
do
	for winSize in 128 256 512 1024 2048 4096 8192 16384
	do
		make quicksort
		./quicksort.out $pSize $winSize
		make clean
	done
done
make clean