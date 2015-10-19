#!/usr/bin/env python

# file: foo.py
#
# additional resources:
#  https://docs.python.org/2/tutorial/datastructures.html
#  http://code.tutsplus.com/articles/advanced-python-data-structures--net-32748
#  http://interactivepython.org/runestone/static/pythonds/SortSearch/TheBinarySearch.html
#  http://interactivepython.org/runestone/static/pythonds/SortSearch/Hashing.html

# import additional system modules
#
import sys
import os
from collections import deque

# Class: Toaster
#
class Toaster:

    # define a constructor
    #
    def __init__(self, arg = 'joe'):
        self.length = 0.0
        self.width = 0.0
        self.name = arg
        
    # end of constructor
# end of class

# main: a simple data structures program
#
def main(argv):

    # traverse a directory in C:
    #
    # DIR           *d;
    # struct dirent *dir;
    # d = opendir(".");
    # if (d) {
    #    while ((dir = readdir(d)) != NULL) {
    #       printf("%s\n", dir->d_name);
    #    }
    #    closedir(d);
    # }

    # traverse a directory in Python:
    #
    flist = []
    dict = {}
    stack = []
    queue = deque()

    for dirpath, dirs, files in os.walk(argv[1]):
        for file in files:
            print "%s/%s" % (dirpath, file)
            fname = "%s/%s" % (dirpath, file)

            # append a list
            #
            flist.append(fname)

            # append a dict
            #
            dict[file] = file

            # append a stack
            #
            stack.append(file)
            stack.pop()

            # append a queue
            #
            queue.append(file)


    # display the data
    #
    print "list:"
    print flist
    print "\ndict:"
    print dict
    print "\nstack:"
    print stack
    print "\nqueue:"
    print queue.popleft()

    # finally, remember that all these data structures
    # can be heterogenous
    #
    dict2 = {}

    dict2['joe'] = 27.0
    dict2['jane'] = 'blue'

    my_toaster = Toaster('proctor silex')
    dict2['bob'] = my_toaster

    print "\ndict2:"
    print dict2

# begin gracefully:
#  this must be the last thing in the file
#
if __name__ == "__main__":
    main(sys.argv[0:])

#
# end of file
