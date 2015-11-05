# import required modules
#
import os
import functions_2

# method: my_example
#
# arguments:
#  arg: an argument to print
#
# return: none
#
def my_example(arg_a):

    # display the argument
    #
    print "my_example: the value of the argument is: %s" % (arg_a)

    # call another function
    #
    functions_2.another_example("*** ece_3822_live_cockroaches ***")

    # exit gracefully
    #
    return True
