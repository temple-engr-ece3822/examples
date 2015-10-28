# https://docs.python.org/2/tutorial/errors.html

(x,y) = (5,0)
try:
    z = x/y
except ZeroDivisionError:
    print "divide by zero"
print "hello joe"

#for arg in sys.argv[1:]:
#    try:
#        f = open(arg, 'r')
#    except IOError:
#        print 'cannot open', arg
#    else:
#        print arg, 'has', len(f.readlines()), 'lines'
#        f.close()
