SRCDIR = ./src

CC          = gcc
CFLAGS      =   -O9 -m32 
#CFLAGS      =   -O9 -m32 -DDDEBUGCODES
CFLAGS2      = -Wall -O9

all:  compressor cleanO

clean: 
	rm -f *.o core CPH

cleanO: 
	rm -f *.o  
	
compressor:    BufferedReader.o BufferedWriter.o MemoryManager.o valstring.o  timer.o hash.o 
	$(CC) $(CFLAGS) valstring.o  BufferedReader.o BufferedWriter.o MemoryManager.o timer.o hash.o -o CPH $(SRCDIR)/compressor.c -lm 

BufferedReader.o:  
	$(CC) $(CFLAGS) -c $(SRCDIR)/BufferedReader.c

BufferedWriter.o:  
	$(CC) $(CFLAGS) -c $(SRCDIR)/BufferedWriter.c

MemoryManager.o:  
	$(CC) $(CFLAGS) -c $(SRCDIR)/MemoryManager.c
	 

valstring.o:  
	$(CC) $(CFLAGS) -c $(SRCDIR)/valstring.c

timer.o:
	$(CC) $(CFLAGS) -c $(SRCDIR)/timer.c
	
hash.o:  
	$(CC) $(CFLAGS) -c $(SRCDIR)/hash.c
	
	
tar:
	# datetex=CPH[$(date +%F_%H_%M_%S)].tar.gz 
	# tar czvf "$datetex" Makefile src 
	tar czvf CPH.tar.gz Makefile src 
		
