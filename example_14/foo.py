#!/usr/bin/env python

# file: foo.py

# import additional system modules
#
import sys
from PyQt4 import QtGui
import pyqtgraph as pg

# main: rescore decoder output
#
def main(argv):

    # create a plotting app
    #
    app = QtGui.QApplication(sys.argv)

    # read the file into memory
    #
    lines = [line.rstrip('\n') for line in open(argv[0])]
    print lines

    # convert to numbers
    #
    data = [float(line.rstrip('\n')) for line in lines]
    print data

    # plot the data
    #
    pg.plot(data, title="Simplest possible plotting example")

    # wait for keyboard input so we can see the plot
    #
    raw_input("Press Enter to continue...")

# begin gracefully:
#  this must be the last thing in the file
#
if __name__ == "__main__":
    main(sys.argv[1:])

#
# end of file
