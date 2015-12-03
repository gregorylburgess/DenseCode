SRCDIR = .

CC          = g++ 
CFLAGS      =  -O9
CFLAGS2      = -Wall -O9

all:  decompressor

decompressor:    BufferedReader.o BufferedWriter.o MemoryManager.o valstring.o writer.o timer.o
	$(CC) $(CFLAGS) valstring.o BufferedReader.o BufferedWriter.o MemoryManager.o writer.o timer.o -o DDETDC $(SRCDIR)/decompressor.c -lm 


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
	 
clean:
	rm -rf *~ *% *.o core *.bck EXE* DLETDC  

