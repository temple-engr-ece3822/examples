#!/usr/bin/env python

# file: foo.py

# import additional system modules
#
import sys
import getopt
import shutil

# main: rescore decoder output
#
def main(argv):

    # try some numbers
    #
    x = 25.0
    print "x = %f" % (x)

#    for hypdir in hypdir_list:

    f = open("foo.dat", "w")
    for i in range(1,10):
#        print "%d: %s\n" % (i, "hello world")
        f.write("%d: %s\n" % (i, "hello world"))
    # end of for loop


# f = open("foo.dat", "r")
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
