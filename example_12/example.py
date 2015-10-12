#!/usr/bin/env python

# file: $(ISIP)/exp/tuh_eeg/exp_0022/scripts/rescore_p1_elab.py
#
# revision history:
#  20150619 (JP): initial version
#
# usage:
#  rescore_p1_elab.py -i hyp.list -p param.txt -o output_dir
#
# arguments:
#  -i (--input): a list of lab file directories
#  -p (--param): a text file containing the sweep parameters
#  -o (--output): an output directory
#
# This script generates extended lab files by sweeping the penalty.
#------------------------------------------------------------------------------

# import isip modules
#
import autoeeg

# import additional system modules
#
import sys
import getopt
import shutil

# main: rescore decoder output
#
def main(argv):

    # allocate variables for arguments
    #
    hypdirlist_a = ""
    pfile_a = ""
    odir_a = ""

    # define the command line options
    #
    try:
        opts, args = getopt.getopt(
            argv, "hi:p:o:", ["input=", "param=", "output="])

    # error handling for command line options
    #
    except getopt.GetoptError:
        print "*> %s: the option does not exist" % sys.argv[0]
        sys.exit(-1)

    # parse the command line arguments
    #
    for opt, arg in opts:

        # option: help
        #
        if opt == '-h':
            print "usage: rescore_p1_elab.py -i hyp.list -p param.txt -o output_dir"
            sys.exit(-1)

        # the hyp directory
        #
        elif opt in ("-i", "--input"):
            hypdirlist_a = arg

        # the parameter file
        #
        elif opt in ("-p", "--param"):
            pfile_a = arg

        # the output directory
        #
        elif opt in ("-o", "--output"):
            odir_a = arg

        # unknown option
        #
        else:
            print "illegal option: %s" % (arg)
            sys.exit()

    # load the sweep parameter file
    #
    prm = autoeeg.ParamSweep().load_sweep_parameters(pfile_a)

    # read a list of hypothesis directories into memory
    #
    hypdir_list = [line.rstrip('\n') for line in open(hypdirlist_a)]
    num_hypdirs = len(hypdir_list)

    # get the number of models and the number of channels:
    #  assume all hypotheses are the same, so use the first one
    #
    num_models, model_list, num_channels, num_files = \
        autoeeg.isip_getinfo(hypdir_list[0])

    # create the output directory
    #
    autoeeg.isip_mkdir(odir_a);

    # loop over all hypotheses
    #
    for hypdir in hypdir_list:

        # create a new filename:
        #  assume the path is structured so that each session is in its
        #  own directory. use odir_a, a group and a session id
        #
        tmp_parts = hypdir.split("/");
        len_tmp = len(tmp_parts)
        grp_name = tmp_parts[len_tmp - 2]
        session_id = tmp_parts[len_tmp - 1]
        odir_sess = "%s/%s/%s" % (odir_a, grp_name, session_id)

        # create the output directory
        #
        shutil.rmtree(odir_sess, True)
        autoeeg.isip_mkdir(odir_sess)

        # loop over all channels:
        #  for each hypothesis directory, we must loop over all channels,
        #  over all labels in each label, and then generate an elab file
        #  with scores adjusted based on the penalties
        #
        for i in range(num_channels):

            # load all the labels for this channel (loop over models)
            #
            labels = autoeeg.isip_get_labels(hypdir, model_list, i)
            
            # create the "elab" output file:
            #
            fname_new = "%s/%s_%s_ch%03d.elab" % \
                (odir_sess, grp_name, session_id, i)
            f = open(fname_new, "w")

            # loop over all sweep parameters:
            #  output a rescored label block for each sweep value
            #
            for i in range(int(prm.nswps)):

                # write delimiter
                #
                value = prm.min + prm.inc * i
                f.write("# penalty = %f\n" % value)

                # loop over all labels and find the maximum score across models
                #
                for j in range(labels[0].num_labels):

                    # copy the likelihoods so we can modify them
                    #
                    tmp_lbl = []
                    tmp_llk = []
                    for k in range(num_models):
                        tmp_lbl.append(labels[k].label[j])
                        tmp_llk.append(labels[k].llk[j])

                    # loop over the parameters and adjust the weights
                    #
                    for k in range(len(prm.lbl)):
                        ind1 = model_list.index(prm.lbl[k])
                        tmp_llk[ind1] += float(prm.wgt[k]) * value
                        
                    # find the maximum
                    #
                    max_value = max(tmp_llk)
                    max_index = tmp_llk.index(max_value)

                    # write the maximum value label:
                    #  note that we use the 0th label for the start/stop
                    #  time because they are all the same across all models
                    #
                    f.write("%s\t%s\t%s\t%f\n" % (labels[0].start[j],
                                                  labels[0].stop[j],
                                                  model_list[max_index],
                                                  tmp_llk[max_index]))
                # end of for loop

                # terminate with a blank line
                #
                f.write("\n")
            # end of while loop over penalty sweep

            # close the file
            #
            f.close()

       # end of for loop over channels
    # end of for loop over all hyp files

# end of main
#

# begin gracefully:
#  this must be the last thing in the file
#
if __name__ == "__main__":
    main(sys.argv[1:])

#
# end of file
