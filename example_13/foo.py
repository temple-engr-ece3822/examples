#!/usr/bin/env python

# file: foo.py

# import additional system modules
#
import sys
import getopt
import shutil
import numpy

# Class: Toaster
#
class Toaster:

    # define a constructor
    #
    def __init__(self):
        self.length = 0
        self.width = 0
    # end of constructor

    def set_length(self, value):
        self.length = value
    # end of function
# end of class

# main: rescore decoder output
#
def main(argv):

    my_toaster = Toaster()
    my_toaster.set_length(27.0);
    print "length = %f" % (my_toaster.length)

# end of main
#

# begin gracefully:
#  this must be the last thing in the file
#
if __name__ == "__main__":
    main(sys.argv[1:])

#
# end of file
