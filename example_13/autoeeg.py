#!/usr/bin/env python

# file: $(AUTOEEG)/python/v1.0/src/autoeeg/autoeeg.py
#
# revision history:
#  20150628 (JP): initial version
#
# This file contains some useful Python functions and classes that are used
# in the AUTOEEG scripts.
#------------------------------------------------------------------------------

# import required modules
#
import os
import errno
import re
import subprocess

# method: isip_mkdir
#
# arguments:
#  path: new directory path (input)
#
# return: none
#
# This method emulates the Unix command "mkdir -p". It creates
# a directory tree, recursing through each level automatically.
# If the directory already exists, it continues past that level.
#
def isip_mkdir(path_a):

    # use a system call to make a directory
    #
    try:
        os.makedirs(path_a)

    # if the directory exists, and error is thrown (and caught)
    #
    except OSError as exc:
        if exc.errno == errno.EEXIST and os.path.isdir(path_a):
            pass
        else: raise
    # end of error handling

    # exit gracefully
    #
    return True
#
# end of function

# method: isip_first_substring
#
# arguments:
#  strings: list of strings (input)
#  substring: the substring to be matched (input)
#
# return: the index of the match in strings
#
# This method finds the index of the first string in strings that
# contains the substring. This is similar to running strstr on each
# element of the input list.
#
def isip_first_substring(strings_a, substring_a):
    return next(i for i, string in enumerate(strings_a) if \
                    substring_a in string)
#
# end of function

# method: isip_first_string
#
# arguments:
#  strings: list of strings (input)
#  substring: the string to be matched (input)
#
# return: the index of the match in strings
#
# This method finds the index of the first string in strings that
# contains an exact match. This is similar to running strstr on each
# element of the input list.
#
def isip_first_string(strings_a, tstring_a):
    return next(i for i, string in enumerate(strings_a) if \
                    tstring_a == string)
#
# end of function

# method: isip_getinfo
#
# arguments:
#  path: the path to a typical hypothesis directory (input)
#
# return: an ntuple containing -
#  the number of models
#  the model list
#  the number of channels
#  the number of files
#
# This counts files in a directory to determine various counts.
# The path must be of the form:
#
#   .../grp/session_id/
#
# where the full path to the bottom of the hierarchy is:
#
#   .../grp/session_id/models/*.lab
#
# This allows a simple command to be used to count files.
#
def isip_getinfo(path_a):

    # get the number of models
    #
    cmd = "ls -1d %s/* | xargs -l1 basename" % path_a
    task = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
    res = task.stdout.read()
    assert task.wait() == 0
    model_list = res.split()
    num_models = len(model_list)

    # get the number of files by running a system find command, locating
    # all the channels across all the models, and dividing by the number
    # of models.
    #
    cmd = "find %s -type f -name *_ch*.lab | wc -l" % path_a
    task = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
    num_files = int(task.stdout.read())
    assert task.wait() == 0

    # compute the number of channels
    #
    num_channels = num_files / num_models

    # exit gracefully
    #
    return (num_models, model_list, num_channels, num_files)
#
# end of function

# method: isip_get_labels
#
# arguments:
#  hypdir: path to the directory containing the labels (input)
#  mlist: the names of the models (input)
#  chan: the number of the channel to be retrieved (input)
#
# return: a label object containing all the labels
#
# This method reads all channel label files across a session. The
# label files are found using the filename "hypdir/model_name/*_ch???.lab".
# The contents of each label file is loaded into memory.
#
def isip_get_labels(hypdir_a, mlist_a, chan_a):

    # loop over all models
    #
    labels = []
    for i in range(len(mlist_a)):

        # generate a label filename
        #
        fname = "%s/%s/*_ch%03d.lab" % (hypdir_a, mlist_a[i], chan_a)

        # fill out the name using a system call
        #
        cmd = "ls %s" % fname
        task = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
        full_fname = (task.stdout.read()).strip('\n')
        assert task.wait() == 0

        # read the file into memory
        #
        lines = [line.rstrip('\n') for line in open(full_fname)]
        labels.append(Labels())

        # collect labels:
        #  loop over all lines and save into the labels data structure
        #  Labels are an n-tuple (start time, stop time, label, log likelihood)
        #
        labels[i].num_labels = 0;
        labels[i].model_name = mlist_a[i]

        for j in range(len(lines)):
            
            # parse the line and throw away blank lines
            #
            line_parts = lines[j].split()
            if line_parts:

                # update the labels structure
                #
                labels[i].num_labels += 1                
                labels[i].start.append(line_parts[0])
                labels[i].stop.append(line_parts[1])
                labels[i].label.append(line_parts[2])
                if len(line_parts) > 3:
                    labels[i].llk.append(float(line_parts[3]))
            # end of if
        # end of for
    # end of for loop

    # exit gracefully
    #
    return (labels)
#
# end of function

# method: isip_print_labels
#
# arguments:
#  labels: a label structure (input)
#
# return: none
#
# This method displays a label structure.
#
def isip_print_labels(labels_a):

    # loop over all label structures
    #
    for i in range(len(labels_a)):

        # print the model name and number of labels
        #
        print "%d: model name: %s" % (i, labels_a[i].model_name)
        print "\tnumber of labels = %d" % labels_a[i].num_labels

        # print the labels
        #
        for j in range(labels_a[i].num_labels):

            # display the elements
            #
            print "\t%s\t%s\t%s\t%f" % (labels_a[i].start[j], \
                                            labels_a[i].stop[j], \
                                            labels_a[i].label[j], \
                                            labels_a[i].llk[j])
        # end of loop j
    # end of loop i

    # exit gracefully
    #
    return True
#
# end of function

# method: isip_get_ref_labels
#
# arguments:
#  refdir: path to the directory containing the labels (input)
#
# return: a label object containing all the labels
#
# This method reads all the label files into a data structure.
#
def isip_get_ref_labels(reflist_a):

    # read the file into memory
    #
    fnames = [line.rstrip('\n') for line in open(reflist_a)]

    # loop over all the files
    #
    keys = []
    labels = [];

    for i in range(len(fnames)):

        # read the file into memory
        #
        lines = [line.rstrip('\n') for line in open(fnames[i])]

        # create unique keys for each of the channels
        #
        base = os.path.basename(fnames[i])
        keys.append(os.path.splitext(base)[0])

        # collect labels:
        #  loop over all lines and save into the labels data structure
        #  Labels are an n-tuple (start time, stop time, label, log likelihood)
        #
        labels.append(Labels())
        labels[i].num_labels = 0;
        labels[i].model_name = fnames[i];

        # loop over all lines in the file
        #
        for j in range(len(lines)):
            
            # parse the line and throw away blank lines
            #
            line_parts = lines[j].split()
            if line_parts:

                # update the labels structure
                #
                labels[i].num_labels += 1                
                labels[i].start.append(line_parts[0])
                labels[i].stop.append(line_parts[1])
                labels[i].label.append(line_parts[2])
                labels[i].llk.append(float(0.0))
            # end of if
        # end of for
    # end of for loop

    # exit gracefully
    #
    return (keys, labels)
#
# end of function

# method: isip_get_hyp_labels
#
# arguments:
#  hfile: hypothesis .elab file (input)
#  nblocks: the number of label blocks (input)
#
# return: a label object containing all the labels
#
# This method reads one .elab file into a data structure. The number
# of label structures output is the number of blocks in the file.
#
def isip_get_hyp_labels(hfile_a, nblocks_a):

    # read the file into memory
    #
    lines = [line.rstrip('\n') for line in open(hfile_a)]
    l_end = len(lines)

    # loop over all lines in the file
    #
    nswps = -1
    labels = []
    for l in range(l_end):

        # skip blank lines
        #
        if (len(lines[l]) > 0):

            # a comment character begins a block
            #
            if lines[l][0] == '#':
                nswps += 1
                labels.append(Labels())
                labels[nswps].num_labels = 0
                labels[nswps].model_name = lines[l]
                
            # otherwise, parse the line for hypothesis data
            #
            else:
                line_parts = lines[l].split()
                labels[nswps].num_labels += 1
                labels[nswps].start.append(line_parts[0])
                labels[nswps].stop.append(line_parts[1])
                labels[nswps].label.append(line_parts[2])
                labels[nswps].llk.append(float(line_parts[3]))
    # end of for loop

    # exit gracefully
    #
    return (labels)
#
# end of function

# method: isip_get_model_list
#
# arguments:
#  labels: a label model structure
#
# return: an ntuple containing:
#  the number of models
#  the model name list
#
# This method reads a label structure and returns a list of models. It
#  extracts this by doing a unique sort on the labels.
#
def isip_get_model_list(labels_a):

    # accumulate the labels
    #
    tmp_list = []
    for i in range(len(labels_a)):
        tmp_list += labels_a[i].label
    
    # do a unique sort
    #
    model_list = list(set(tmp_list))
    model_list.sort()

    # exit gracefully
    #
    return (len(model_list), model_list)
#
# end of function

# method: isip_get_conf
#
# arguments:
#  cfile: a file containing confusion matrices
#
# return: an ntuple containing:
#  the model list
#  the sweep parameters
#  the confusion matrices
#
# This method reads a label structure and returns a list of models. It
#  extracts this by doing a unique sort on the labels.
#
def isip_get_conf(cfile_a):

    # load the file into memory
    #
    lines = [line.rstrip('\n') for line in open(cfile_a)]

    # get the model list:
    #  note we throw away the delimiter
    #
    ind1 = isip_first_substring(lines, "models:")
    tmp_line = re.sub(r'\s+', '', lines[ind1])
    tmp_list = tmp_line.split(":")
    model_list = tmp_list[1].split(",")
    num_models = len(model_list)

    # get the sweep values:
    #  these must come after the model list
    #
    prm = ParamSweep();
    ind1 = isip_first_substring(lines, "sweep:")
    tmp_list = lines[ind1].split(":")[1:]
    prm.min = float(tmp_list[0])
    prm.max = float(tmp_list[1])
    prm.inc = float(tmp_list[2])
    prm.nswps = int((prm.max - prm.min)/prm.inc) + 1;

    # skip over the next three lines because we can ignore the
    # penalty labels and weights
    #
    ind1 += 3
    lines = lines[ind1:]

    # loop over all the matrices
    #
    cnf = [];
    swp_vals = []

    for i in range(prm.nswps):

        # get the sweep value and skip past the next row of labels
        #
        ind1 = isip_first_substring(lines, "sweep_value =")
        swp_vals.append(float(lines[ind1].split("=")[1]))
        ind1 += 2

        # read the matrix:
        #  assume the next num_models+1 lines are the matrix
        #
        tmp_cnf = [[0 for x in range(num_models)] for x in range(num_models)]
        cnf.append(tmp_cnf)
        for j in range(num_models):
            tmp_list = lines[ind1+j].split()
            for k in range(1, num_models+1):
                cnf[i][j][k-1] = int(tmp_list[k])

        # shift past this matrix so we locate the next matrix
        #
        ind1 += num_models + 1
        lines = lines[ind1:]
    # end of loop

    # exit gracefully
    #
    return (model_list, prm, cnf)
# end of function

# method: isip_get_map
#
# arguments:
#  mfile: a file containing a mapping (input)
#
# return: an ntuple containing:
#  the det map
#  the confusion map
#
# This method reads a mapping file and returns two matrices containing the
# mapping information.
#
def isip_get_map(mfile_a):

    # load the file into memory
    #
    lines = [line.rstrip('\n') for line in open(mfile_a)]

    # read the mapping file:
    #  get the DET curve mapping
    #
    ind1 = isip_first_substring(lines, "DET {")
    ind1 += 1
    det_map = []

    while lines[ind1].find("}") == -1:
        tmp_line = re.sub(r'\s+', '', lines[ind1])
        tmp_list = tmp_line.split(":")
        tmp_list[1:] = tmp_list[1].split(",")
        det_map.append(tmp_list)
        ind1 += 1
    # end of while

    # read the mapping file:
    #  get the MAP curve mapping
    #
    ind1 = isip_first_substring(lines, "MAP {")
    ind1 += 1
    cnf_map = []

    while lines[ind1].find("}") == -1:
        tmp_line = re.sub(r'\s+', '', lines[ind1])
        tmp_list = tmp_line.split(":")
        tmp_list[1:] = tmp_list[1].split(",")
        cnf_map.append(tmp_list)
        ind1 += 1
    # end of while

    # exit gracefully
    #
    return (det_map, cnf_map)
# end of function

# method: isip_print_conf
#
# arguments:
#  cnf: a confusion matrix (input)
#  prm: the sweep parameter data structure (input)
#  map: the symbol map (input)
#  fo: a file pointer where the results are written (input/output)
#
# return: none
#
# This method displays a confusion matrix.
#
def isip_print_conf(cnf_a, prm_a, map_a, fo_a):

    # declare local variables
    #
    num_mats = len(cnf_a)
    num_models = len(map_a)

    # write a section header
    #
    fo_a.write("# Confusion Matrices:\n\n")

    # display the results in a table
    #
    for i in range(num_mats):

        # display the header of the table
        #
        swp_val = prm_a.min + prm_a.inc * i
        fo_a.write(" sweep_value = %f\n" % swp_val)
        fo_a.write("  Ref/Hyp: ")
        for j in range(num_models):
            fo_a.write("%8s:          " % map_a[j][0])
        fo_a.write("\n")

        # loop over all models
        #
        total = 0
        for j in range(num_models):

            # compute the sum of the elements in a row
            #
            rtotal = 0
            for k in range(num_models):
                rtotal += cnf_a[i][j][k]
            if rtotal == 0:
                rtotal = 1
            else:
                total += rtotal

            # write a row of the table, label first
            #
            fo_a.write("  %8s: " % map_a[j][0])
            for k in range(num_models):
                pcnt = cnf_a[i][j][k] / float(rtotal) * 100.0
                fo_a.write("%8d (%6.2f%%) " % (cnf_a[i][j][k], pcnt))
            fo_a.write("\n")

        # display the overall error rates
        #
        pcnt = 0.0
        for j in range(num_models):
            pcnt += cnf_a[i][j][j]
        pcnt *= 100.0 / float(total)
        fo_a.write("  Correct = %8.3f%%, Error = %8.3f%%\n\n" % \
                       (pcnt, 100.0 - pcnt))
    # end of loop

    # exit gracefully
    #
    return True
#
# end of function

# Class: ParamSweep
#
# This class facilitates handling a parameter file that controls
# how many sweeps are done to generate a det curve.
#
class ParamSweep:

    # define a constructor
    #
    def __init__(self):
        self.lbl = []
        self.wgt = []
        self.min = 0.0
        self.max = 0.0
        self.inc = 0.0
        self.nswps = int(0)
    # end of constructor

    # method: load_sweep_parameters
    #
    # arguments:
    #  file: parameter filename (input)
    #
    # return:
    #  a ParamSweep object containing the sweep parameter information
    #
    # This method reads a sweep parameter file and returns an object
    # containing all the relevant information.
    #
    def load_sweep_parameters(self, file_a):

        # initialize an object
        #
        prm = ParamSweep()

        # read the entire file into memory:
        #  this is much faster and the memory is released quickly
        #
        lines = [line.rstrip('\n') for line in open(file_a)]

        # loop over all lines
        #
        for i in range(len(lines)):
            
            # throw away blank lines and comments
            #
            tmp_line = lines[i].rstrip('\n')
            tmp_line = tmp_line.lstrip()
            if (len(tmp_line) > 0) and (tmp_line[0] != '#'):

                # split the line and remove whitespace
                #
                line_parts = re.sub(r'\s', '', tmp_line).split(':')

                # case: sweep parameters
                #
                if tmp_line[0:5] == "sweep":
                    prm.min = float(line_parts[1].strip())
                    prm.max = float(line_parts[2].strip())
                    prm.inc = float(line_parts[3].strip())
                    prm.nswps = ((prm.max - prm.min) / prm.inc) + 1
                    
                # case: channel weights
                #
                else:
                    prm.lbl.append(line_parts[0])
                    prm.wgt.append(float(line_parts[1]))
                # end of if
            # end of if
        # end of for
        
        # exit gracefully
        #
        return prm
    # end of function
# end of class

# Class: Labels
#
# This class facilitates handling labels in a file
# by encapsulating the information into a single data structure.
#
class Labels:

    # define a constructor
    #
    def __init__(self):
        self.num_labels = 0
        self.model_name = ""
        self.start = []
        self.stop = []
        self.label = []
        self.llk = []
    # end of constructor
# end of class

#
# end of file
                
