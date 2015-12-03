SRCDIR = ./src

CC          = gcc
CFLAGS      =  -O9
CFLAGS2     = -Wall -O9

all:  removeZeroes addZeroes cleanO

removeZeroes:  BufferedReader.o BufferedWriter.o 
	$(CC) $(CFLAGS) BufferedReader.o BufferedWriter.o  -o REMOVER $(SRCDIR)/RemoveZeroes.c -lm 

addZeroes:  BufferedReader.o BufferedWriter.o 
	$(CC) $(CFLAGS) BufferedReader.o BufferedWriter.o  -o ADDER $(SRCDIR)/AddZeroes.c -lm 


BufferedReader.o:  
	$(CC) $(CFLAGS) -c $(SRCDIR)/BufferedReader.c


BufferedWriter.o:  
	$(CC) $(CFLAGS) -c $(SRCDIR)/BufferedWriter.c


clean:
	rm -rf *~ *% *.o core *.bck EXE* REMOVER.exe ADDER.exe

cleanO:
	rm -f *.o 
