SRCDIR = ./src

CC          = gcc
CFLAGS      =  -O9 -m32 
CFLAGS2     = -Wall -O9

all:  decompressor cleanO

clean: 
	rm -f *.o core DecPH

cleanO:
	rm *.o	

decompressor:     BufferedReader.o BufferedWriter.o MemoryManager.o valstring.o writer.o timer.o
	$(CC) $(CFLAGS) valstring.o BufferedReader.o BufferedWriter.o MemoryManager.o writer.o timer.o -o DecPH $(SRCDIR)/decompressor.c -lm 

BufferedReader.o:  
	$(CC) $(CFLAGS) -c $(SRCDIR)/BufferedReader.c


BufferedWriter.o:  
	$(CC) $(CFLAGS) -c $(SRCDIR)/BufferedWriter.c
	
MemoryManager.o:  
	$(CC) $(CFLAGS) -c $(SRCDIR)/MemoryManager.c

writer.o:  
	$(CC) $(CFLAGS) -c $(SRCDIR)/writer.c

valstring.o:  
	$(CC) $(CFLAGS) -c $(SRCDIR)/valstring.c

timer.o:
	$(CC) $(CFLAGS) -c $(SRCDIR)/timer.c		 
 
 
 tar:
	# datetex=DecPH[$(date +%F_%H_%M_%S)].tar.gz 
	# tar czvf "$datetex" Makefile src 
	tar czvf DecPH.tar.gz Makefile src 

