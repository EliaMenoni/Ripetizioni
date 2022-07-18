#!/bin/bash

echo "Cancello binari di $1:"

ls ./$1/bin
rm ./$1/bin/*
