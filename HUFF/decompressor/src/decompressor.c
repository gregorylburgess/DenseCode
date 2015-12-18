/* Plain Huffman. -- 
A semistatic word-based byte oriented compressor for text files.

Programmed by Antonio Fari�a
Copyright (C) 2006  Nieves R. Brisaboa, Gonzalo Navarro, Antonio Fari�a and Jos� R. Param�
Author's contact: antonio.fari@gmail.com
WEB: http://vios.dc.fi.udc.es/codes

Programmed as a baseline for the paper: 

N. Brisaboa, A. Fari�a, G. Navarro, and J. Param�. Lightweight natural 
language text compression. Information Retrieval. 
Information Retrieval(10), pp. 1-33, 2007. (online) 
(doi:10.1007/s10791-006-9001-9)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

Some of the ideas, and functions of these code belong to Edleno Silva (derived from the 
original implementation of Tagged Huffman and Plain Huffman.
The canonical Huffman is implemented based on http://www.cs.mu.oz.au/~alistair/abstracts/inplace.html
*/


#include "decompressor.h"

/*------------------------------------------------------------------
 Inicia as estruturas necessarias ao funcionamento do modulo Hash.c
 ---------------------------------------------------------------- */
void initialize (unsigned long tamArq){
	_memMgr = createMemoryManager(); //_memMgr.initialize();
	N=tamArq;
	words  = (t_word *) malloc(N * sizeof(t_word));
}


/*-----------------------------------------------------------------------
 Insert a new element into vocabulary (in the array of words)
 ---------------------------------------------------------------------*/
 void insertElement (const unsigned char *aWord, register unsigned long len , register unsigned long position) {

	if (position == N){
		printf("Not enough memory, vocabulary exceeds maximun size !\n");
		exit(1);
	}

	getMemoryBlock(_memMgr,( char **)&(words[position].word),len+1);
	
	{
		unsigned char *w,*src;
		src=(unsigned char*) aWord;
		
		w=words[position].word;
		register int i;
		for (i=0;i<=len;i++) {  //copies also the '\0'
		 *(w++)=*(src++);
		}
	}
	words[position].size=len;
	//fprintf(stderr,"\n inserted word [%s]",words[position].word);
}




/*------------------------------------------------------------------
 * Performs the full decoding process.
------------------------------------------------------------------ */
void mainDecompressor (char* infile, char* outfile) {

	int previousAlfan = 0;
	unsigned char *palabra;
	unsigned long codeSize1;//,q;
	unsigned char *tmpW;
	register unsigned char abyte;

	BufferRead B,V;

	int f,v;
	unsigned char *text,*top, *nextbuf;
	char aux[MAX_SIZE_OF_WORD];
	char voc[2048];
	
	sprintf(voc,"%s.voc",infile);


	// Inicialization of the buffers used to read the files
	f = open (infile, O_RDONLY);
	if (f == -1)
		printf("Cannot read file %s", infile);
	v = open (voc, O_RDONLY);
	if (f == -1)
		printf("Cannot read file %s",voc);
	
	B = bufCreate(MAX_SIZE_OF_WORD+5);    //n�mero de "bytes" que hay que dejar al final del buffer siempre, por culpa
	V = bufCreate(MAX_SIZE_OF_WORD+5);	  //de que pudiese venir una nueva palabra"que fuese cortada".

	bufSetFile (B,f);
	bufSetFile (V,v);


	// Inicialization of output buffer
	outWriter = createWriter (outfile);

	fprintf(stderr, "\nSTARTING DECOMPRESSION...");
	register unsigned char *pbeg,*pend,*codeStart;


	/* loading the vocabulary into memory*/

	bufCurrent (V,(char**)&text,(char**)&top);
	pbeg = text; pend = top;

	{ //Reads the number of words of the vocabulary (stored in the text file as a long value)
		unsigned long numbertmp;
		int z;
		unsigned char *c;
		c= (unsigned char*) &numbertmp;
		for (z=1; z<=4;z++) {
			*c =*pbeg;
			c++;
			pbeg++;
		}
		fprintf(stderr,"\n\t*Number of words in the vocabulary is: %ld",numbertmp);

	initialize(numbertmp); //initializes decompressor for up to numbertmp words
	}
	

	  /** structure of the canonical Huffman tree **/
		//Min levels value (min niveles)
		codeStart = pbeg;
		while (*pbeg !='\0') {
			pbeg++;
		}
		{ register unsigned long len;
		  register int i;
		  len = pbeg-codeStart;
		  for (i=0;i<=len;i++) {  //copia tambien el '\0'
			 aux[i]=*(codeStart++);
		   }
		}
		tablaNodosEnNiveles.minNivel= (unsigned int) strtoul(aux, (char **)NULL, 10);
		pbeg++;

		//Max levels value (max niveles)
		codeStart = pbeg;
		while (*pbeg !='\0') {
			pbeg++;
		}		
		{ register unsigned long len;
		  register int i;
		  len = pbeg-codeStart;
		  for (i=0;i<=len;i++) {  //copia tambien el '\0'
			 aux[i]=*(codeStart++);
		   }
		}
		tablaNodosEnNiveles.maxNivel= (unsigned int) strtoul(aux, (char **)NULL, 10);
		pbeg++;
		
		//initializing first and base arrays
		InicializaNodosEnNiveles (&tablaNodosEnNiveles, tablaNodosEnNiveles.maxNivel+1);

		//reading values for each level.
		{ register unsigned long i;
			for(i=tablaNodosEnNiveles.minNivel;i<=tablaNodosEnNiveles.maxNivel;i++){
				//base[i] value is read.
				codeStart = pbeg;
				 while (*pbeg !='\0') {
					pbeg++;
				}
				{   register unsigned long len;
					register int i;
					len = pbeg-codeStart;
					for (i=0;i<=len;i++) { 
						aux[i]=*(codeStart++);
					}
				}
				tablaNodosEnNiveles.base[i]=strtoul(aux, (char **)NULL, 10);
		        pbeg++;
		        
			    //first[i] value is read.
		        codeStart = pbeg;
				while (*pbeg !='\0') {
					pbeg++;
				}			
				{   register unsigned long len;
					register int i;
					len = pbeg-codeStart;
					for (i=0;i<=len;i++) {  
						aux[i]=*(codeStart++);
					}
				}
				tablaNodosEnNiveles.first[i]=  strtoul(aux, (char **)NULL, 10);
		        pbeg++;
			}/* end for */

			//for (i=tablaNodosEnNiveles.minNivel; i<= tablaNodosEnNiveles.maxNivel; i++) {
			//	fprintf(stderr,"\n Base[%d] -> %d; first[%d] --> %d",i,tablaNodosEnNiveles.base[i],i,tablaNodosEnNiveles.first[i]);
			//}
	
		}
		
		
		/** loading the words of the vocabulary **/
		/*Reads one word at a time (j) and adds it to the vocabulary vector.*/		
		unsigned long j;
        j=0;
		while(1){
			while ( pbeg < pend )   {
				codeStart = pbeg;
				while (*pbeg !='\0') {
					pbeg++;
				}
				{ 
					register unsigned long len;
					register int i;
					unsigned char *w;
					
					len = pbeg-codeStart;
					getMemoryBlock(_memMgr,( char**)&(words[j].word),len+1);					
					w=words[j].word;
				 	for (i=0;i<=len;i++) { *(w++)=*(codeStart++); }  //copying
				 	
					words[j].size=len;
				}
			
				j++;pbeg++;
				//fprintf(stderr,"\n words [%s] was read",words[j].word);
			}

			nextbuf=pbeg;
	
			bufLoad (V, (char*)nextbuf);
			//fprintf(stderr,"\n [%ld] palabras metidas, buffer [%d]",j,bufEmpty(V));
			if (bufEmpty(V)) break;
			//fprintf(stderr,"\n pasa ampliamente del break");
			bufCurrent (V,(char**)&text,(char**)&top);
			pbeg = text; pend = top;
	    } //while (1)
	    

	//Performs the decoding process over all the codewords of the compressed file
	    
	{
		register unsigned int nivelActual,j;
  		while (!bufEmpty(B) ) {

		bufCurrent (B,(char**)&text,(char**)&top);
		pbeg = text; pend = top;

		while ( pbeg < pend )   {
			//Reads and decodes a codeword
			nivelActual=1;
			j=*pbeg;
			while (j < tablaNodosEnNiveles.first[nivelActual]) {
				j *= 256 ;
				pbeg++;
				j += (*pbeg);
			    nivelActual++;
			}
			j += tablaNodosEnNiveles.base[nivelActual] - tablaNodosEnNiveles.first[nivelActual];
			pbeg++;

			writeWord(outWriter,words[j].word, words[j].size);
		}/* inner while*/

		nextbuf=pbeg;
		bufLoad (B, (char*)nextbuf);
	}//while
   }


	fprintf(stderr, "\nDECOMPRESSION FINISHED...");

	//frees buffers and memory.
	bufDestroy(B);
	bufDestroy(V);
	destroyWriter(outWriter);
	free(words);
	destroyMemoryManager(_memMgr);
	free(tablaNodosEnNiveles.first);
	free(tablaNodosEnNiveles.base);	

} //mainDecompressor


/* Shows a message with info of this version of the code */
void showMessage() {
	printf("\n*Plain Huffman Code [semistatic decompressor]");
	printf("\n*CopyRight(c)2006 Nieves R. Brisaboa, Gonzalo Navarro, Antonio Fari�a and Jos� R. Param�\n");
	
    printf("\n  #PH comes with ABSOLUTELY NO WARRANTY; ");
    printf("\n  #This is free software, and you are welcome to redistribute it ");
    printf("\n  #under certain conditions\n\n");
}


/*------------------------------------------------------------------ */
/*
 *  DECOMPRESSOR PROGRAM OF PLAIN HUFFMAN
 */
/*------------------------------------------------------------------ */

int main(int argc, char* argv[])
{


	showMessage();	
	
	if (argc < 3) {
		fprintf(stderr,"\n Wrong call to PH decompressor\n");
		fprintf(stderr," Use:  %s  <in file>  <out file> \n",argv[0]);
		return 0;
	}

	StartRusageTime();
	mainDecompressor(argv[1], argv[2]);
	StopRusageTime("timesDecPH",argv[1]);
	fprintf(stderr,"\n");
}




void InicializaNodosEnNiveles (t_NodosEnNiveles *tablaNiveles, unsigned long int maxNiveles) {
	int i;
	((*tablaNiveles).first)   = (unsigned long int *) calloc(maxNiveles, sizeof (unsigned long int));
	((*tablaNiveles).base) 	  = (unsigned long int *) calloc(maxNiveles, sizeof (unsigned long int));
}
