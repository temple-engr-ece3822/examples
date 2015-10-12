#!/usr/bin/env python

# file: foo.py

# import additional system modules
#
import sys
import getopt
import shutil
import numpy

# main: rescore decoder output
#
def main(argv):

    # basic data types
    #
    x = 25.0
    y = 27.0
    print "x = %f, y = %f" % (x, y)
    
    # arrays
    #
    my_array = []
    my_array.append(2.0)
    my_array.append(3.0)
    print my_array;

    # dictionaries
    #
    dict = {'Name': 'Zara', 'Age': 7, 'Class': 'First'};
    print dict
    print "---"
    print "dict['Name']: ", dict['Name']
    print "---"
    print "dict['Age']: ", dict['Age']
    print "---"

    # loops
    #
    for i in range(1,10):
        print "%d: %s\n" % (i, "hello world")
#        f.write("%d: %s\n" % (i, "hello world"))
    # end of for loop

    # nested loops
    #
    for x in xrange(1, 11):
        for y in xrange(1, 11):
            print '%d * %d = %d' % (x, y, x*y)

    # conditionals
    #
    for x in xrange(5):
        if x == 3:
            break
        elif x == 1:
            print 'x is equal to (%d)' % (x)
        else:
            print 'x is less than 3 but not equal to 1 (%d)' % (x)

#    for hypdir in hypdir_list:
#    f = open("foo.dat", "w")

    # file I/O
    #
    # f = open(argv[1], "r")
    #hypdir_list = [line.rstrip('\n') for line in open(hypdirlist_a)]

# end of main
#

# begin gracefully:
#  this must be the last thing in the file
#
if __name__ == "__main__":
    main(sys.argv[1:])

#
# end of file
