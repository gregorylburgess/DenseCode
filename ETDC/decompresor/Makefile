SRCDIR = ./src

CC          = gcc
CFLAGS      = -O9 -Wall -D_FORTIFY_SOURCE=0  
CFLAGS2     = -Wall -O9 -m32

all:  decompressorUNCvoc cleanO

clean: 
	rm -f *.o core DETDC DETDC_compVOC

cleanO:
	rm *.o	

decompressorUNCvoc:     BufferedReader.o BufferedWriter.o MemoryManager.o valstring.o writer.o timer.o basics.o huff.o
	$(CC)  -DUNCOMPRESSED_VOC $(CFLAGS) valstring.o BufferedReader.o BufferedWriter.o MemoryManager.o writer.o timer.o basics.o huff.o -o DETDC $(SRCDIR)/decompressor.c -lm 

decompressorCOMvoc:     BufferedReader.o BufferedWriter.o MemoryManager.o valstring.o writer.o timer.o basics.o huff.o
	$(CC)  -DCOMPRESSED_VOC $(CFLAGS) valstring.o BufferedReader.o BufferedWriter.o MemoryManager.o writer.o timer.o basics.o huff.o -o DETDC_compVOC  $(SRCDIR)/decompressor.c -lm 

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
 


huff.o: basics.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/huff.c

basics.o: 
	$(CC) $(CFLAGS) -c $(SRCDIR)/basics.c

