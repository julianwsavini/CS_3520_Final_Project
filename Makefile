IDIR =../include
CC=g++
CFLAGS= -I$(IDIR) -g -O0 -std=c++11

ODIR=.

LIBS=-lncurses

_DEPS =
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ1 = System.cpp


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: System


System: $(_OBJ1)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


clean:
	rm -f *~ core $(INCDIR)/*~
	rm -f System

etags:
	find . -type f -iname "*.[ch]" | xargs etags --append
