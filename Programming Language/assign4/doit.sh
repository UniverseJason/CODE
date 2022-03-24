#!/bin/bash

for i in {1..8}; do
  echo ========= mm$i
  gcc -Wall mm$i.c -o mm$i
  ./mm$i 3 -12 14 0 7 -8 3
done

echo ========= python
python3 mm.py 3 -12 14 0 7 -8 3
