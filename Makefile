CC	=	gcc
CFLAGS	=	

zootxt:	zootxt.c Makefile
	$(CC) $(CFLAGS) -o zootxt zootxt.c  -lcurses 

clean:
	rm -f *.o *~ \#* zootxt
