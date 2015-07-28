# file: GNUMakefile
#

# define a target for the application
#
all: foo

# define a target to link the application
#
foo: foo.o
	g++ -lm foo.o -o foo

# define a target to compile the application
#
foo.o: foo.cc
	g++ -O2 -c foo.cc -o foo.o

# define a target to clean the directory
#
clean:
	rm -f foo.o
	rm -f foo

#
# end of file


