#!/bin/bash
echo "input after output"
./server ./ask10sForInt ./client2
echo ""

echo "test timeout, (10) occueres when timeout. Never gets (10) after timeout"
./server ./timeoutAndInput ./createIntAfter2s ./ask10sForInt
echo ""

echo "reading test"
./server ./onlyReading ./createIntAfter2s ./ask10sForInt
echo ""

echo "input-output"
./server ./inputOutput ./outputinput