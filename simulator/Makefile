HOST=$(shell hostname)


export OMP_NUM_THREADS=12

IDIR=.

CC=gcc
CXX=g++
CFLAGS=-g -O2 -Wall -I/home/shared/boost_1_55_0 -I$(IDIR) -fopenmp

ODIR=.


_DEPS = eval.h simulator.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = eval.o simulator.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all:	main

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

main	: $(OBJ) $(ODIR)/main.o
	$(CXX) -o $@ $(OBJ) $(ODIR)/main.o $(CFLAGS) 


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ main
