#!/bin/bash

echo "Compilo il codice $2 di $1"
echo ""

g++ ./$1/$2.cpp -o ./$1/bin/$2 -Wall

