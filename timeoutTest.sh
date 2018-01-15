#!/bin/bash

echo "test timeout, (10) occueres when timeout. Never gets (10) after timeout"
./server ./timeoutAndInput ./createIntAfter2s ./ask10sForInt
echo ""
