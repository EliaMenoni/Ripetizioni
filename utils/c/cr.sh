#!/bin/bash

echo "Compilo e eseguo il codice $2 di $1"
echo ""

gcc -std=c11 ./$1/$2.c -o ./$1/bin/$2 -Wall && ./$1/bin/$2



