#!/bin/bash

echo "input after output, ask10 should receive tuple (10)"
./server ./ask10sForInt ./createIntAfter2s
echo ""

echo "output after input, ask10 should receive tuple (5)"
./server ./ask10sForInt ./create5
echo ""

echo "test timeout, (10) occueres when timeout. Never ask10s gets (10) after timeout of timeoutAndInputClient. If timeoutAndInputClient will received the (10) tuple, ask10 can not."
./server ./timeoutAndInput ./createIntAfter2s ./ask10sForInt ./createString
echo ""

echo "read test, after input read gets nothing"
./server ./onlyReading ./createIntAfter2s ./ask10sForInt
echo ""

echo "input-output"
./server ./inputOutput ./outputinput
echo ""

echo "thousands of request, server gets thousands of output request and 1 input in the meantime. Client should get the requested tuple. "
./server ./createRandomInts ./ask10sForInt

