#!/bin/bash

echo "Compilo il codice $2 di $1"
echo ""

gcc -std=c11 ./$1/$2.c -o ./$1/bin/$2 -Wall

