#!/usr/bin/env python

# file: $(ISIP)/exp/tuh_eeg/exp_0022/scripts/gen_feats_deltas.py
#
#  20150616 (JP): completed cleaning up the code
#  20150616 (AM): cleaned up and documented the code
#  20140601 (AH): initial version
#
# usage:
#  gen_feats_deltas.py -i input.htk -o output.htk
#
# arguments:
#  -i (--input):    .htk formatted feature file
#  -o (--output):   .htk formatted feature file with deltas
#  -w1 (--window1): window length in frames for deltas (default = 4)
#  -w2 (--window2): window length in frames for delta-deltas (default = 4)
#
# This script computes delta and delta-delta features for an HTK-formatted
# feature file.
#
# Reference:
#  http://practicalcryptography.com/miscellaneous/machine-learning/
#   guide-mel-frequency-cepstral-coefficients-mfccs/#deltas-and-delta-deltas
#
# import required modules:
#  note that the path to the module htkmfc must be included in the
#  PYTHONPATH environment variable.
#
import os
import sys
import getopt
import htkmfc
import numpy as np

# main: this is the main function of this Python
#
def main(argv):

    # declare parameters for the input and output filenames
    #
    ifile_a = ""
    ofile_a = ""

    # declare variables for the window lengths:
    #  (1) note that the windows extend from [-wlen,+wlen].
    #  (2) the default value is 4.
    #
    wlen1_a = 4
    wlen2_a = 1

    # define the command line options
    #
    try:
        opts, args = getopt.getopt(  # @UnusedVariable
            argv, "hi:o:w1:w2:", ["input=", "output=", "w1=", "w2="])

    # error handling for command line options
    #
    except getopt.GetoptError:
        print("*> %s: the option does not exist", sys.argv[0]);
        sys.exit(-1)

    # parse the command line arguments
    #
    for opt, arg in opts:

        # option: help
        #
        if opt == '-h':
            print 'Usage: gen_feats_deltas.py -i input.htk -o output.htk'
            sys.exit()

        # the input argument which is the full path of an htk file
        #
        elif opt in ("-i", "--input"):
            ifile_a = arg

        # the output argument which is the full path of an htk file
        #
        elif opt in ("-o", "--output"):
            ofile_a = arg

        # the window length argument for deltas (default value is 4)
        #
        elif opt in ("-w1", "--window1"):
            wlen1_a = int(arg)

        # the window length argument for the delta-deltas (default value is 4)
        #
        elif opt in ("-w2", "--window2"):
            wlen2_a = int(arg)

    # open the input file and read the data
    #
    inp_mfc = htkmfc.open(ifile_a)
    data = inp_mfc.getall()

    # extract N, the number of frames, and D, the number of features
    #
    (N, D) = data.shape

    # create a matrix of zeros with the size of the data
    #
    z = np.zeros((N, D))

    # create a new matrix with the data concatenated with z:
    #  note that this generates data_out with Nx(2*D) with the last column
    #  set to 0.
    #
    data_out = np.append(data, z, 1)

    # calculate the denominator of the derivative formula:
    #
    #   d_t=(sigma(n(c_t+n - c_t-n))/(2*sigma(n^2))
    #
    #  for deltas.
    #
    d1 = 0
    for t0 in range(wlen1_a):
        t = t0 + 1
        d1 = d1 + t * t
        pass
    d1 = 2 * d1

    # calculate the denominator term for delta-delta
    #
    d2 = 0
    for t0 in range(wlen2_a):
        t = t0 + 1
        d2 = d2 + t * t
        pass
    d2 = 2 * d2

    # calculate deltas: loop over all the frames
    #
    for n in range(N):
        s1 = 0
        for t0 in range(wlen1_a):
            t = t0 + 1
            if n + t > N - 1:
                ind2 = N - 1
            else:
                ind2 = n + t
            if n - t < 0:
                ind1 = 0
            else:
                ind1 = n - t

            # compute the numerator for delta
            #
            s1 = s1 + t * (data[ind2, :] - data[ind1, :])

            # end of loop
            #
            pass

        # divide by the denominator
        #
        s1 = s1 / float(d1)

        # copy the delta features to data_out
        #
        data_out[n, D:] = s1

        # end of loop
        #
        pass

    # add new columns for the delta-deltas
    #
    data_out = np.append(data_out, z, 1)

    # calculate delta-deltass: loop over all the frames
    #
    for n in range(N):
        s2 = 0
        for t0 in range(wlen2_a):
            t = t0 + 1
            if n + t > N - 1:
                ind2 = N - 1
            else:
                ind2 = n + t
            if n - t < 0:
                ind1 = 0
            else:
                ind1 = n - t

            # compute the numerator
            #
            s2 = s2 + t * (data_out[ind2, D:2 * D] - data_out[ind1, D:2 * D])

            # end of loop
            #
            pass

        # divide by the denominator
        #
        s2 = s2 / float(d2)

        # copy the delta-delta features to data_out
        #
        data_out[n, 2 * D:] = s2

        # end of loop
        #
        pass

    # obtain the shape of data_out
    #
    (N, Dout) = data_out.shape

    # create a new .htk file:
    #  ofile_a will be generated in the same place as the input file
    #
    out_mfc = htkmfc.HTKFeat_write(
        ofile_a, inp_mfc.nSamples, Dout,
        inp_mfc.sampPeriod, inp_mfc.paramKind)
    out_mfc.writeall(data_out)

# begin gracefully
#
if __name__ == "__main__":
    main(sys.argv[1:])

#
# end of file
