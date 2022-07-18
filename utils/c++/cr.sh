#!/bin/bash

echo "Compilo e eseguo il codice $2 di $1"
echo ""

g++ ./$1/$2.cpp -o ./$1/bin/$2 -Wall && ./$1/bin/$2



