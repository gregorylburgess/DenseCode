SRCDIR = ./src

CC          = gcc
CFLAGS      =  -O9
CFLAGS2      = -Wall -O9

all:  compressor cleanO

cleanO: 
	rm -f *.o
	
compressor:  BufferedReader.o BufferedWriter.o MemoryManager.o valstring.o timer.o hash.o 
	$(CC) $(CFLAGS) valstring.o BufferedReader.o BufferedWriter.o MemoryManager.o timer.o hash.o -o CDETDC $(SRCDIR)/compressor.c -lm 

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
	

clean:
	rm -rf *~ *% *.o core *.bck CDETDC  

