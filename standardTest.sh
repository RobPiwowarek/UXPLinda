#!/bin/bash
echo "test0"
./server ./ask10sForInt ./client2
echo "test1"
./server ./timeoutAndInput ./createIntAfter2s ./ask10sForInt
echo "test2"
./server ./inputOutput ./outputinput