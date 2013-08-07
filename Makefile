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
	./$(PROG) ./tests/test1.txt
	./$(PROG) ./tests/test2.txt
	diff ./result_full.txt ./tests/result_full.txt
	diff ./result_short.txt ./tests/result_short.txt
	diff ./result_custom.txt ./tests/result_custom.txt
	diff ./result_full.html ./tests/result_full.html
	diff ./result_short.html ./tests/result_short.html
	diff ./result_custom.html ./tests/result_custom.html

doc:	
	rm -f -R docs
	doxygen

dist clean:
	rm -fR $(OBJS) $(DFILES) $(PROG) $(LIBNAME) semantic.cache* test/* docs result_*.txt result_*.html

ifeq (,$(findstring $(MAKECMDGOALS),dist clean))
      -include $(DFILES)
endif
