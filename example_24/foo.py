#!/usr/bin/env python

# import modules
#
import sys
import functions

# main: foo.py
#
def main(argv):

    # allocate variables for arguments
    #
    print "program name: argv[0] = %s" % (argv[0])
    print "first argument: argv[1] = %s" % (argv[1])

    # call a function
    #
    functions.my_example("ece_3822")

# begin gracefully
#
if __name__ == "__main__":
    main(sys.argv[0:])

#
# end of file
#

