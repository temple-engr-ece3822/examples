# file: GNUMakefile
#

# define a target for the application
#
all: ex_floats

# define a target to link the application
#
ex_floats: ex_floats.o
	g++ -lm ex_floats.o -o ex_floats

# define a target to compile the application
#
ex_floats.o: ex_floats.cc
	g++ -O2 -c ex_floats.cc -o ex_floats.o

# define a target to clean the directory
#
clean:
	rm -f ex_floats.o
	rm -f ex_floats

#
# end of file


