
/* DYNAMIC END-TAGGED DENSE CODE. -- 
A dynamic word-based byte oriented compressor for text files based on 
dynamic End-Tagged Dense Code.

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


#include "decompressor.h"
#include "version.h"


/*------------------------------------------------------------------
 reads the vocabulary size and the size of the vector "top" from 
 the aulixiar file (if it exists)
 ---------------------------------------------------------------- */
void readSizeVocabularyandTop(char *filename,  long *vocSize,  long *topSize) {
	char fileNametmp[24];
	int file;
	unsigned long value;
	
	strcpy (fileNametmp,filename);
	strcat(fileNametmp,".voc");	
	
	if ( (file = open (fileNametmp, O_RDONLY)) ==-1) {
		fprintf(stderr,"\nError while opening file: %s\n\tUsing default vocabulary size instead\n",fileNametmp);
		return ;
	}
	read(file,  vocSize,  4);
	read(file,  topSize,  4);
	
	close (file);
//	*vocSize = 885630;
//	*topSize= 9485287;
}


/*------------------------------------------------------------------
 Initializes data structures
 ---------------------------------------------------------------- */
void initialize (unsigned long sizeVoc, unsigned long tamTop){
	unsigned long i;

	_memMgr = createMemoryManager(); //_memMgr.initialize();

	N=sizeVoc;
	words  = (t_word *) malloc(N * sizeof(t_word));
	zeroNode = 0;

	topBlock = (unsigned long *) malloc(tamTop * sizeof(unsigned long));

	//initializes top and last values for each block
	MAXFREQ = 1;
	topBlock[0] =zeroNode;
	topBlock[1] =zeroNode;

}



/*-----------------------------------------------------------------------
 Inserts a new element into vocabulary (in the array of words)
 NOT USED HERE
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
		for (i=0;i<=len;i++) {  //copia tambien el '\0'
		 *(w++)=*(src++);
		}

	}
//	strncpy ((char *) words[position].word, (char *)aWord, len);
//	words[position].word[len]='\0';
	words[position].size=len;

}


/*------------------------------------------------------------------
 * Performs the full decoding process over the compressed file.
------------------------------------------------------------------ */
void mainDecompressor (char* infile, char* outfile, int needToReadFile, unsigned long N_val, unsigned long N_val2) {
  
  unsigned char *palabra;
  unsigned long codeSize1;
  unsigned char *tmpW;
  register unsigned char abyte;
  BufferRead B;

	int f;
	/**/unsigned char *text,*top, *nextbuf;

	if (needToReadFile)
		readSizeVocabularyandTop(infile,(long *)&N_val,(long *)&N_val2);
	printf("\n VOC SIZE [%ld],     TOP SIZE [%ld] \n",N_val,N_val2);

// Inicialización del buffer de lectura
	f = open (infile, O_RDONLY);
	if (f == -1)
		printf("Cannot read file %s",*infile);
	B = bufCreate(MAX_SIZE_OF_WORD+5);    //número de "bytes" que hay que dejar al final del buffer siempre, por culpa
													  //de que pudiese venir una nueva palabra"que fuese cortada".
	bufSetFile (B,f);

// Inicialización del buffer de escritura
	outWriter = createWriter (outfile);

    initialize(N_val,N_val2); //initializes decompressor for up to N_value words

	fprintf(stderr, "\n\n\nSTARTING DECOMPRESSION...\n");

	{
    /**/unsigned char *pbeg,*pend,*codeStart;

	while (!bufEmpty(B) ) {

		//fprintf(stderr, "\nEntra en while principal");

		bufCurrent (B,(char**)&text,(char**)&top);
		pbeg = text; pend = top;
	    //fprintf(stderr,"\n top = [%ld]",((unsigned long) top) - ((unsigned long)B->data));


		while ( pbeg < pend )   {
				codeStart = pbeg;
				j=0;
				while (*pbeg >=128) {
					j = j * 128 + (*pbeg - 128);
					pbeg++;
				}
				j = j * 128 + *pbeg;
				j += TABLABASE[pbeg - codeStart];
	
				//fprintf(stderr,"\n decodificado=[%ld], bytes [%ld], con zeronodoe = [%ld]",j,pbeg - codeStart +1,zeroNode);
	
				pbeg++;
	
//fprintf(stderr, "\n paso 50 ");
	
	         if (j<zeroNode) {
					writeWord(outWriter,words[j].word, words[j].size);
					
					
					/**/	{    // HACE EL UPDATE
							register unsigned long f,p,i,h;
							register unsigned long temp;
							unsigned char *tmpW;
																						 
								f = words[j].weight;
											
								i = topBlock[f];
								words[i].weight +=1;																	
								
								tmpW = words[i].word;
								words[i].word = words[j].word;
								words[j].word = tmpW;
			
								temp = words[i].size;
								words[i].size = words[j].size;
								words[j].size = temp;
								
								topBlock[f]=i+1;
								
//								if (MAXFREQ == f+1) {  //The most frequent words need to be increased.
//									topBlock[f+1] = 0;
//									MAXFREQ++;
//								}
			
							}
					/**/					
					
				}
				else { // j== zeronode
					
					codeStart = pbeg;
					while (*pbeg !='\0') {
						pbeg++;
					}
	            
	
					{  register unsigned long len;
					   unsigned char *w;
	
	 				   len = pbeg-codeStart;
	
					   ////fprintf(stderr,"\nzeroNode= true wordLeng=[%ld]",pbeg-codeStart);
					   writeWord(outWriter,codeStart, len);
	
					   getMemoryBlock(_memMgr,( char**)&(words[j].word),len+1);
	
						w=words[j].word;
						register int i;
						for (i=0;i<len;i++) {  //copia tambien el '\0'
							*(w++)=*(codeStart++);
						}
						*w='\0';
						words[j].weight = 1;
						words[j].size=len;
					}
	
					pbeg++;
					zeroNode++;
	             // fprintf(stderr,"\n Chega ao final");
	
				}/*else == zeronode*/

								

			}/* while interno*/


		nextbuf=pbeg;

		bufLoad (B, (char*)nextbuf);


	}//while
   }

	//fprintf(stderr, "\n... DECOMPRESSION FINISHED after %d words\n\n");

	bufDestroy(B);
	destroyWriter(outWriter);
	free(words);


} //mainDecompressor





/*------------------------------------------------------------------ */
/*
 *  DECOMPRESSOR PROGRAM OF DYNAMIC (S,C)-DC.
 */
/*------------------------------------------------------------------ */

int main(int argc, char* argv[])
{
	char fileName[20];

	long sizeNValue;
	long sizeNValue2;
	int needToLoadSizes=0;
	

	showMessage();

	if (argc < 3) {
		fprintf(stderr,"\n Wrong call to DETDC dynamic decompressor\n");
		fprintf(stderr," Use:  %s  <in file>  <out file> [<VOC SIZE]><TOP SIZE]>\n",argv[0]);
		return 0;
	}


	sizeNValue = MAX_LEN_VOCABULARY;
	sizeNValue2 = INITIAL_TOP_FREQ;
	
    if ((argc ==3) || (argc = 4)) 
    	needToLoadSizes = 1;


	//	if ((argc ==3) || (argc = 4))
	//		readSizeVocabularyandTop(argv[1],&sizeNValue,&sizeNValue2);
		
	//	if (argc ==4)
	//		sizeNValue = atol(argv[3]);
	
	if (argc ==5) {
		sizeNValue = atol(argv[3]);
		sizeNValue2 = atol(argv[4]);
		needToLoadSizes = 0;
	}

	//printf("\n VOC SIZE [%ld],     TOP SIZE [%ld] \n",sizeNValue,sizeNValue2);

	StartRusageTime();
	mainDecompressor(argv[1], argv[2],needToLoadSizes,sizeNValue,sizeNValue2);
	StopRusageTimeFile("timesDecDETDC.txt",argv[1]);
	fprintf(stderr,"\n ZERONODE VALE AL FINAL   %ld \n",zeroNode);
}



