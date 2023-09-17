CC = gcc
CFL = -Wall -g -O2
LDIR = ./lib
IDIR = ./inc

all: test

libvector.o: $(LDIR)/libvector.c $(IDIR)/vector.h
	$(CC) $(CFL) -c $(LDIR)/$@ $^

test: test.c $(LDIR)/libvector.o
	$(CC) $(CFL) -o $@ $^

