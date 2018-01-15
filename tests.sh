#!/bin/bash

echo "input after output"
./server ./ask10sForInt ./createIntAfter2s
echo ""

echo "output after input"
./server ./ask10sForInt ./create5
echo ""

echo "test timeout, (10) occueres when timeout. Never gets (10) after timeout"
./server ./timeoutAndInput ./createIntAfter2s ./ask10sForInt
echo ""

echo "read test, after input read gets nothing"
./server ./onlyReading ./createIntAfter2s ./ask10sForInt
echo ""

echo "input-output"
./server ./inputOutput ./outputinput


echo "give my tuple"