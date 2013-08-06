CC=gcc

CFLAGS= -std=gnu99 -D_GNU_SOURCE -Wall -Wno-parentheses -Wno-switch -ggdb3 
#-I../libs
LDFLAGS= -lm
#-L../if-check -lifcheck

DBGFLAGS=-DDEBUG -DIFDEBUG -DMALLOCDEBUG 

SPLINTFLAGS=-nullret +gnuextensions +posixlib -skipisoheaders -skipposixheaders

SRC= $(wildcard *.c)
OBJS= $(SRC:.c=.o) 
LOBJS=$(filter-out main.o,$(OBJS))
DFILES= $(SRC:.c=.d)
HFILES=  $(wildcard *.h)

PROG=findObjects

.PHONY:	clean depend all dist test doc lib

all:	$(PROG)
#$(OBJS)

$(PROG): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(PROG)

%.d:	%.c $(HFILES)
	$(CC) $(CFLAGS) -MM $< > $@

test:	$(PROG)
	./$(PROG)

doc:	
	rm -f -R docs
	doxygen

dist clean:
	rm -fR $(OBJS) $(DFILES) $(PROG) $(LIBNAME) semantic.cache* test/* docs result_*.txt result_*.html

ifeq (,$(findstring $(MAKECMDGOALS),dist clean))
      -include $(DFILES)
endif
