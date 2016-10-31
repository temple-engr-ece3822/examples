#!/usr/bin/env python
# Prints input one character at a time 
from time import *
import os, sys

char_start = 0
char_end = 1
string = sys.stdin.read() #use standard input to allow more than 1 line

if len(sys.argv) > 1: #checks for command line arguments
    delay = float( 0.5 ) #uses argument as delay time

else:
    delay = 0.5

strlength = len(string)+1

while char_end <= strlength:
    printchar = string[char_start:char_end]
    sys.stdout.write(printchar)
    sys.stdout.flush()
    sleep(delay) #delay time between characters
    char_end += 1
    char_start += 1

exit