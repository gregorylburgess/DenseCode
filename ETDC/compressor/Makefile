SRCDIR = ./src

CC          = gcc -w
CFLAGS      =  -O9 -D_FORTIFY_SOURCE=0  -Wall
CFLAGS2     = -Wall -O9
XCPY         = mv CETDC ..
all:  compressorUNCvoc cleanO
	$(XCPY)
clean: 
	rm -f *.o core CETDC CETDCcompVOC

cleanO: 
	rm -f *.o   

compressorUNCvoc:   BufferedReader.o BufferedWriter.o MemoryManager.o valstring.o timer.o hash.o basics.o huff.o
	$(CC) -DUNCOMPRESSED_VOC $(CFLAGS) valstring.o BufferedReader.o BufferedWriter.o MemoryManager.o timer.o hash.o basics.o  huff.o -o CETDC $(SRCDIR)/compresor.c -lm 

compressorCOMvoc:   BufferedReader.o BufferedWriter.o MemoryManager.o valstring.o timer.o hash.o basics.o huff.o
	$(CC) -DCOMPRESSED_VOC $(CFLAGS) valstring.o BufferedReader.o BufferedWriter.o MemoryManager.o timer.o hash.o basics.o  huff.o -o CETDCcompVOC $(SRCDIR)/compresor.c -lm 

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
	


huff.o: basics.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/huff.c

basics.o: 
	$(CC) $(CFLAGS) -c $(SRCDIR)/basics.c
