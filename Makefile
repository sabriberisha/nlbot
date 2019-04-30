CC		= gcc
CFLAGS		= -W -Wall 
OBJS		= main.o myconnect.o nio.o parse.o exec.o

all:		nlbot

clean:		
		rm -f *.o
		rm -f *.a
		rm -f *.core
		rm -f nlbot

nlbot:		$(OBJS)
		$(CC) -o nlbot $(OBJS) 
		strip nlbot

main.o:		main.c
myconnect.o:	myconnect.c
nio.o:          nio.c
parse.o:        parse.c
exec.o:		exec.c



