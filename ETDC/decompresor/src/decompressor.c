/* End-Tagged Dense Code. -- 
A semistatic word-based byte oriented compressor for text files.

Programmed by Antonio Fariña
Copyright (C) 2006  Nieves R. Brisaboa, Gonzalo Navarro, Antonio Fariña and José R. Paramá
Author's contact: antonio.fari@gmail.com

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
*/

/*-----------------------------------------------------------------------
 File       : decompressor.c
 Function   : Main file of the decompressor of ETDC
 ------------------------------------------------------------------------*/

#include "decompressor.h"


/*------------------------------------------------------------------
 Initialization of structures used.
 ---------------------------------------------------------------- */
void initialize (unsigned long sizeVoc){
	_memMgr = createMemoryManager(); //_memMgr.initialize();

	//const unsigned long TABLABASE[5] = {0,128,16512,2113664,270549120};
	/*	TABLABASE[1] = 0;
		TABLABASE[2] = S_value;
		TABLABASE[3] = TABLABASE[2] + S_value * C_value;
		TABLABASE[4] = TABLABASE[3] + S_value * C_value*C_value;
		TABLABASE[5] = TABLABASE[4] + S_value * C_value*C_value*C_value;
	*/

	N=sizeVoc;
	words  = (t_word *) malloc(N * sizeof(t_word));
	zeroNode = 0;
}


/*-----------------------------------------------------------------------
 Inserts a new element into the vocabulary (in the array of words) (function not used now)
 ---------------------------------------------------------------------*/
 void insertElement (const unsigned char *aWord, register unsigned long len , register unsigned long position) {

	// if (position == N){
		// printf("Not enough memory, vocabulary exceeds maximun size !\n");
		// exit(1);
	// }

	getMemoryBlock(_memMgr,( char **)&(words[position].word),len+1);
	{
		unsigned char *w,*src;
		register int i;
		src=(unsigned char*) aWord;

		w=words[position].word;
		for (i=0;i<=len;i++) {  //the final '\0' is also copied
			*(w++)=*(src++);
		}
	}

	words[position].size=len;
}

/*------------------------------------------------------------------
 * Performs the full decoding process.
------------------------------------------------------------------ */
void mainDecompressor (char* infile, char* outfile) {

	BufferRead B;

	/**/unsigned char *text,*top, *nextbuf;
	char *voc; 
	int f;


fprintf(stderr, "\nSTARTING DECOMPRESSION...");

#ifdef UNCOMPRESSED_VOC
	BufferRead V;
	int v;
	register unsigned long j;	 	 	//stores the position in the vocabulary of the current parsed word
	
	voc = (char *) malloc(strlen(infile)+256);
	strcpy(voc,infile);
	strcat(voc,".voc");


	v = open (voc, O_RDONLY);
	if (v == -1) {
		fprintf(stderr,"Cannot read file %s\nDecompression aborted.\n",voc);
		exit(0);
	}
	
	register unsigned char *pbeg,*pend,*codeStart;

	/* loading the vocabulary into memory********************************************/

	V = bufCreate(MAX_SIZE_OF_WORD+5);	  //a word being "cut"
	bufSetFile (V,v);
	bufCurrent (V,(char**)&text,(char**)&top);
	pbeg = text; pend = top;

	{ //Reads the number of words of the vocabulary (stored in the text file as a long value)
		unsigned int numbertmp;
		int z;
		unsigned char *c;
		c= (unsigned char*) &numbertmp;
		for (z=1; z<=4;z++) {
			*c =*pbeg;
			c++;
			pbeg++;
		}
		fprintf(stderr,"\n\t*Number of words in the vocabulary is: %ld",(long)numbertmp);

	initialize(numbertmp); //initializes decompressor for up to numbertmp words
	}

	/*Reads one word at a time (j) and adds it to the vocabulary vector.*/
	j=0;
	while(1){

		while ( pbeg < pend )   {
			//reads the word
			codeStart = pbeg;
			while (*pbeg !='\0') {
				pbeg++;
			}
			//adds the word to the vocabulary
			{
				register unsigned long len;
				unsigned char *w;
				register int i;

				len = pbeg-codeStart;
				getMemoryBlock(_memMgr,( char**)&(words[j].word),len+1);

				w=words[j].word;
				for (i=0;i<=len;i++) {  //copia tambien el '\0'
					*(w++)=*(codeStart++);
				}
				words[j].size=len;
			}

			j++;
			pbeg++;
		}

		nextbuf=pbeg;

		bufLoad (V, (char*)nextbuf);
		if (bufEmpty(V)) break;
		bufCurrent (V,(char**)&text,(char**)&top);
		pbeg = text; pend = top;
	}
	bufDestroy(V);
	free(voc);
#endif


#ifdef COMPRESSED_VOC
	char *voc2;
	register unsigned char *pbeg,*pend,*codeStart;
	{
    voc = (char *) malloc(strlen(infile)+10); //name of the vocabulary file
    voc2 = (char *) malloc(strlen(infile)+10); //name of the vocabulary file
	sprintf(voc,"%s.voc.huff",infile);
	sprintf(voc2,"%s.cvoc",infile);
	
		register unsigned long i;
		THuff huffVoc;
		unsigned int *bitVector;  //compressed vocabulary.
		unsigned long totalBits;
		unsigned int zeroNode;	
		unsigned long totalVocSize;
		//////////////////////////////////////  DECODING
		
		{FILE *f;
			f = fopen(voc,"r");
			loadHuffAfterDecode2 (&huffVoc,f,0);
			fclose(f);	
		}
		
		
		{FILE *f;		
			f = fopen(voc2,"r");
			fread(&zeroNode,sizeof(unsigned int), 1,f);					
			fread(&totalVocSize,sizeof(unsigned int), 1,f);		
			fread(&totalBits,sizeof(unsigned int), 1, f);
			bitVector = (unsigned int *) malloc (sizeof (unsigned int) * (totalBits + 31)/32);
			bitVector[(totalBits + 31)/32 -1]=0000;				
			fread(bitVector,sizeof(unsigned int), ((totalBits+31)/32),f);
			fclose(f);
		}
		
		initialize(zeroNode); //initializes decompressor for up to zeroNode words
		
		//decodes the compressed buff int memBuf2.
		unsigned char *memBuf2 = (unsigned char *) malloc (sizeof(unsigned char) *  totalVocSize);	
		zoneWords = memBuf2;		
		unsigned char *aChar = memBuf2;

		unsigned int ptr=0;
		unsigned int pos;
		while (ptr < totalBits) {
			ptr=decodeHuff (&huffVoc, &pos, bitVector, ptr);
			//fprintf(stderr,"%c. \n",pos);
			*aChar++=(unsigned char) pos;
		}
		free(bitVector);	
			
		//sets pointers and lens into words[].
		unsigned char *aWord = memBuf2;
		register unsigned long len;
		for (i=0;i<zeroNode;i++) {
			len=0;
			words[i].word = aWord;
			while (*aWord) {aWord++;len++;}
			words[i].size = len;
			aWord++;			
		}
			
		freeHuff(huffVoc);
		
	}	
	
	free(voc);free(voc2);

	
#endif
	/* end loading the vocabulary into memory********************************************/
	
	


	// Inicialization of the buffers used to read the compressed file
	f = open (infile, O_RDONLY);
	if (f == -1) {
		fprintf(stderr,"Cannot read file %s\nDecompression aborted.\n",infile);
		exit(0);
	}	

	B = bufCreate(MAX_SIZE_OF_WORD+5);    //Number of byte left at the end of a memory buffer to avoid

	bufSetFile (B,f);

	// Inicialization of output buffer
	outWriter = createWriter (outfile);

	//Performs the decoding process over all the codewords of the compressed file
	{
		register unsigned long j;
		while (!bufEmpty(B) ) {

			bufCurrent (B,(char**)&text,(char**)&top);
			pbeg = text; pend = top;

			while ( pbeg < pend )   {
				//Reads and decodes a codeword
				codeStart = pbeg;
				j=0;
				while (*pbeg >=128) {
					j = j * 128 + (*pbeg - 128);
					pbeg++;
				}
				j = j * 128 + *pbeg;
				j += TABLABASE[pbeg - codeStart]; //the position of the word in the vocabulary

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
	destroyWriter(outWriter);
	free(words);
	destroyMemoryManager(_memMgr);
	
	#ifdef COMPRESSED_VOC
		free(zoneWords);
	#endif

} //mainDecompressor


/* Shows a message with info of this version of the code */
void showMessage() {
	printf("\n*End-Tagged Dense Code v0.9 [semistatic decompressor]");
	printf("\n*CopyRight(c)2006 Nieves R. Brisaboa, Gonzalo Navarro, Antonio Fariña and José R. Paramá\n");
	
    printf("\n  #ETDC comes with ABSOLUTELY NO WARRANTY; ");
    printf("\n  #This is free software, and you are welcome to redistribute it ");
    printf("\n  #under certain conditions\n\n");
}

/*------------------------------------------------------------------ */
/*
 *  DECOMPRESSOR PROGRAM OF ETDC.
 */
/*------------------------------------------------------------------ */



int main(int argc, char* argv[])
{
	showMessage();	
	
	if (argc < 3) {
		fprintf(stderr,"\n Wrong call to ETDC  decompressor\n");
		fprintf(stderr," Use:  %s  <in file>  <out file> \n",argv[0]);
		return 0;
	}

	StartRusageTime();
	mainDecompressor(argv[1], argv[2]);
	StopRusageTime("timesDec",argv[1]);
	fprintf(stderr,"\n");
	return (0);
}
