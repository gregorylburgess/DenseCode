/* DYNAMIC LIGHTWEIGHT END-TAGGED DENSE CODE. -- 
A dynamic word-based byte oriented compressor for text files based on 
dynamic End-Tagged Dense Code.

N. Brisaboa, A. Fariña, G. Navarro, and J. Parama. Eficiently decodable and searchable natural language adaptive compression. In Proceedings of the 28th Annual International ACM SIGIR Conference on Research and Development in Information Retrieval (SIGIR-2005), Bahia-Brazil, August 15-19 2005. ACM Press.

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
 reads the vocabulary size from the aulixiar file (if it exists)
 ---------------------------------------------------------------- */
long readSizeVocabulary(char *filename) {
	char fileNametmp[24];
	int file;
	unsigned long value;
	
	strcpy (fileNametmp,filename);
	strcat(fileNametmp,".voc");	
	
	if ( (file = open (fileNametmp, O_RDONLY)) ==-1) {
		fprintf(stderr,"\nError while opening file: %s\n\tUsing default vocabulary size instead\n",fileNametmp);
		return -1;
	}
	read(file,  &value,  4);
	close (file);
	return (value);
}

/*------------------------------------------------------------------
 Initializes data structures
 ---------------------------------------------------------------- */
void initialize (unsigned long tamArq){
	unsigned long i;

	_memMgr = createMemoryManager(); 

	N=tamArq;
	words  = (t_word *) malloc(N * sizeof(t_word));
	zeroNode = 0;

}



/*-----------------------------------------------------------------------
 Insert a new element into vocabulary (in the array of words)
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
	 register int i;
	 src=(unsigned char*) aWord;

	 w=words[position].word;
	 for (i=0;i<=len;i++) {  //copies also the last '\0'
		 *(w++)=*(src++);
	 }

	}
	words[position].size=len;
}


/*------------------------------------------------------------------
 * Performs the full decoding process.
------------------------------------------------------------------ */
void mainDecompressor (char* infile, char* outfile, unsigned long N_val) {

  
  unsigned char *palabra;
  unsigned long codeSize1;
  unsigned char *tmpW;
  register unsigned char abyte;
  BufferRead B;

	int f;
	unsigned char *text,*top, *nextbuf;

// Initializes the input buffer
	f = open (infile, O_RDONLY);
	if (f == -1)
		printf("Cannot read file %s",*infile);
	B = bufCreate(MAX_SIZE_OF_WORD+5);  
	bufSetFile (B,f);

// Initializes the output buffer
	outWriter = createWriter (outfile);

   initialize(N_val); //initializes decompressor for up to N_value words

	fprintf(stderr, "\n\n\nSTARTING DECOMPRESSION...\n");

	{
    /**/unsigned char *pbeg,*pend,*codeStart;

	while (!bufEmpty(B) ) {

		//fprintf(stderr, "\nEntra en while principal");

		bufCurrent (B,(char**)&text,(char**)&top);
		pbeg = text; pend = top;
		
	  //fprintf(stderr, "\n main while");
		while ( pbeg < pend )   {
			codeStart = pbeg;
			j=0;
			while (*pbeg >=128) {
				j = j * 128 + (*pbeg - 128);
				pbeg++;
			}
			j = j * 128 + *pbeg;
			j += TABLABASE[pbeg - codeStart];

			pbeg++;

      if (j<2113662) { //a regular code
				//	writes a word
				writeWord(outWriter,words[j].word, words[j].size);
				}
				else {

				if (j == 2113662) {  //C_NEW
					//fprintf(stderr,"\n a new word comes");
					{ register unsigned long len,c1;

					  unsigned char *w;
						codeStart = pbeg;
						//reading the ascii word
						while (*pbeg !='\0') {
							pbeg++;
						}
            j=zeroNode;
						len = pbeg-codeStart;
						pbeg=codeStart;

						//fprintf(stderr,"\nzeroNode= true wordLeng=[%ld][%ld]",len,pbeg-codeStart);
						writeWord(outWriter,pbeg, len);

								getMemoryBlock(_memMgr,( char**)&(words[j].word),len+1);

								 w=words[j].word;
								 {
								 register int i;
								 for (i=0;i<len;i++) {  //copies also the '\0'
									 *(w++)=*(pbeg++);
								 }
								}
                *w=0; //adds the final '\0'
								words[j].size=len;
								//fprintf(stderr,"\n Read word [%s]",words[j].word);
					}

					pbeg++;
					zeroNode++;
          
				}
				else //if (j==zeroNode+1)
				{	////fprintf(stderr,"\nA SWAP code comes\n");

						//reads the code of the promoting word
						codeStart = pbeg;
						nj=0;
						while (*pbeg >=128) {
							nj = nj * 128 + (*pbeg - 128);
							pbeg++;
						}
						nj  = nj * 128 + *pbeg;
						nj += TABLABASE[pbeg - codeStart];
						pbeg++;

						//reads the code of the word that goes down

						codeStart = pbeg;
						j=0;
						while (*pbeg >=128) {
							j = j * 128 + (*pbeg - 128);
							pbeg++;
						}
						j = j * 128 + *pbeg;
						j += TABLABASE[pbeg - codeStart];
						pbeg++;

						//swap
						{int tmpS;
						tmpW = words[nj].word;
						tmpS = words[nj].size;
						words[nj].word = words[j].word;
						words[nj].size = words[j].size;
						words[j].word = tmpW;
						words[j].size = tmpS;
					    //swap(words[nj].word,words[j].word);
						}
					writeWord(outWriter,words[j].word, words[j].size);

				}// end (j==zeroNode+1)

			}/*else != zeronode*/

		}/* inner while */

		nextbuf=pbeg;

		bufLoad (B, (char*)nextbuf);


	}// main while
 }

	//fprintf(stderr, "\n... DECOMPRESSION FINISHED after %d words\n\n");

	bufDestroy(B);
	destroyWriter(outWriter);
	free(words);


} //mainDecompressor






/*------------------------------------------------------------------ */
/*
 *  DECOMPRESSOR PROGRAM OF DYNAMIC DLETDC.
 */
/*------------------------------------------------------------------ */

int main(int argc, char* argv[])
{
	char fileName[20];

	long sizeNValue;

	showMessage();

	if (argc < 3) {
		fprintf(stderr,"\n Wrong call to DLETDC dynamic decompressor\n");
		fprintf(stderr," Use:  %s  <in file>  <out file> <[VOC SIZE]>\n",argv[0]);
		return 0;
	}

	sizeNValue = readSizeVocabulary(argv[1]);
	
	if (argc ==4)
		  sizeNValue = atol(argv[3]);

	if (sizeNValue == -1) {sizeNValue = MAX_LEN_VOCABULARY; }

	fprintf(stderr,"\nVocabulary size set to :%ld\n",sizeNValue);

	StartRusageTime();
	mainDecompressor(argv[1], argv[2],sizeNValue);
	StopRusageTimeFile("timesDecDLETDC.TXT",argv[1]);
	//StopRusageTime();
}
