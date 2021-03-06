
/* DYNAMIC END-TAGGED DENSE CODE. -- 
A dynamic word-based byte oriented compressor for text files based on 
dynamic End-Tagged Dense Code.

Programmed by Antonio Fari�a
Copyright (C) 2006  Nieves R. Brisaboa, Gonzalo Navarro, Antonio Fari�a and Jos� R. Param�
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
 File      : compressor
 Function  : Main file of the compressor
 ------------------------------------------------------------------------*/


#include "compressor.h"
#include "version.h"

/*------------------------------------------------------------------
 Initilizes the structures used.
 ---------------------------------------------------------------- */
void initialize (unsigned long sizeVoc, unsigned long tamTop){
	unsigned long i;
	unsigned long N;
	unsigned long N1;

	N=sizeVoc;
	N1=tamTop;

	_memMgr = createMemoryManager(); //_memMgr.initialize();

	initialize_hash(N);
	positionInTH = (unsigned long*) malloc(N * sizeof(unsigned long));

	zeroNode = 0;

	//initializes top and last values for each block
	topBlock = (unsigned long*) malloc(N1 * sizeof(unsigned long));
	MAXFREQ = 1;
	topBlock[0] = 0;
	topBlock[1] = 0;

}





/*------------------------------------------------------------------
 * GetCode
 * gets the codeword of a word ranked "position"
 * returns an unsigned long value (up to 4 bytes codewords) and its size
 *  in two global scope variables: CODEWORD and K
------------------------------------------------------------------ */
void getCode(register unsigned long position) {

	register unsigned long kk=1; //counts the number of bytes of the codeword to be generated

	if (position >= 128) {
	   register unsigned long seguir;
		register unsigned  long i,r;
		register unsigned  long codeword;
		i=position;
		(codeword) = (i % 128);
		seguir 	  = (i / 128) -1;

			while (seguir >= 128) {
				r = seguir % 128;
				seguir = seguir/128 -1;
				//seguir -=1;
				(codeword) = (codeword) + ( (r + 128)<<(8*kk) ) ;
				kk ++;
			}
		(codeword) = (codeword) + ( (seguir + 128  )<<(8*kk) ) ;
		kk ++;

		K  = kk;
		CODEWORD= codeword;

	}
	else {
		K  = kk;
		CODEWORD= position;
	}
}


/*------------------------------------------------------------------
 * GetCode
 * Obtains the codeword of a word ranked in "position"
 * returns the codeword and its size by modifing
 * two global scope variables: CODEWORD and K
------------------------------------------------------------------ */
void  getCodeOLD(register unsigned long position) {

	register unsigned  long codeword;

	register unsigned long kk=1;

	if (position >= 128) {
			unsigned long despl=8;
			codeword = (position& 0x7F);
			position  >>= 7;

			while (position > 0) {
				position --;
				 codeword |= (( (position & 0x7F) | 0x80 )<<despl );
				 despl += 8;
				 position  >>= 7;
				 kk ++;
			}

			K=kk;
			CODEWORD= codeword;
			//return codeword;
	}
	else {
			K  = kk;
			CODEWORD= position;
			//return position;
	}
}

/*------------------------------------------------------------------
 * GetCodeNav  (Gonzalo's algorithm)
 * gets the codeword of a word ranked "position"
 * returns an unsigned long value (up to 4 bytes codewords) in "*codeword"
 *         the number of bytes of the codeword.
------------------------------------------------------------------ */
//Old Gonzalo's algorithm
//while (num > 127) { output (num & 0x7F); num >>= 7; }
//    output (0x80 | num)
void getCodeNav(register unsigned long position){

	register unsigned long despl;
	register unsigned long codeword;
	register unsigned long k;

	k=1;
	codeword = (position& 0x7F);
	position  >>= 7;
	despl=8;

	while (position > 0) {
		position -=1;
		codeword |= (( (position & 0x7F) | 0x80 )<<despl );
		despl += 8;
		position  >>= 7;
		k ++;
	}
	CODEWORD = codeword;
	K=k;
}



#ifdef VERSION_GETCODEANDWRITE

/*------------------------------------------------------------------
 * GetCodeAndWrite
 * Obtains the codeword of a word ranked "position" and writes it to the
 * output buffer "bw"
 * The codeword is created inside a char[8], instead of doing that inside
 * a number, to enable copying it directly to bw.
 * No codeword is returned.
------------------------------------------------------------------ */
void getCodeAndWrite(BufferWrite bw,register unsigned long position)  {

	register unsigned long kk=1; //counts the number of bytes of the codeword to be generated
	unsigned char *codeword;

	codeword =(unsigned char *) (code+7);

	if (position >= 128) {
	    register unsigned long seguir;
		register unsigned  long i,r;
		i=position;
		(*codeword--) = (i % 128);
		seguir 	 	= (i / 128) -1;

			while (seguir >= 128) {
				r = seguir % 128;
				seguir = seguir/128 -1;
				//(codeword) = (codeword) + ( (r + 128)<<(8*kk) ) ;
				kk ++;
				(*(codeword--)) = r + 128;
			}
//		(codeword) = (codeword) + ( (seguir + 128  )<<(8*kk) ) ;
		(*codeword) = (seguir + 128  );
		kk ++;

//		K  = kk;
//		CODEWORD= codeword;

	}
	else {
//		K  = kk;
//		CODEWORD= position;
		*codeword = position;
	}

writeBW(bw,codeword,kk);
}
#endif


/*------------------------------------------------------------------
 * Performs the full encoding process.
   One pass through the text:
   	- Reads a word.
   	- New word is preceded by C_new "ASCII_WORD" \0
   	- Existing word is encoding as C_code
   	- Update of data structures each time a word is parsed.
------------------------------------------------------------------ */

void mainCompressor (char* infile, char* outfile, unsigned long N_val, unsigned long N_val2) {

	BufferRead B;
	unsigned char *text,*top, *nextbuf;
	register unsigned char *pbeg,*pend,*wordstart;
	register unsigned long size;
	BufferWrite bw;

	register unsigned long i;

	int f;

	unsigned char *aWord;

   	StartValid();

	// Initialization of read buffer
	f = open (infile, O_RDONLY);
	if (f == -1) {
		printf("Cannot read file %s",infile);
		exit(0);
	}

	B = bufCreate(MAX_SIZE_OF_WORD+5);    //n�mero de "bytes" que hay que dejar al final del buffer siempre, por culpa

	bufSetFile (B,f);

	// Initialization of output buffer

	bw= bufCreateWriter(outfile);
	openBW(bw);

	initialize(N_val,N_val2); //initializes compressor for up to N_value words
	fprintf(stderr, "\nSTARTING COMPRESSION...");


	/*** Beginning of the compression process   ***/
	if (bufEmpty(B) ) {size =0; return;}
	size = 0;

	while ( B->size==B->dsize ) {

		bufCurrent (B,(char**)&text,(char**)&top);
		pbeg = text; pend = top;

        while ( pbeg < pend )   {

			//parsing a word or separator
			size=0;
			wordstart = pbeg;
			if (_Valid[*pbeg]) {   //alphanumerical data

				while ( (size<MAX_SIZE_OF_WORD) && (  _Valid[*pbeg] )) {
						size++;
						pbeg++;
				}

		    }
	      	else {
				if (*pbeg != ' ') { //a separator comes starting in ' ' comes, so it is a new word

					while ( (size<MAX_SIZE_OF_WORD) && (!_Valid[*pbeg])) {

						size++;
						pbeg++;
					}
				}
				else {  //a  SPACE comes, so we have to test if next character is alphanumerical or not

					pbeg++;
					if (_Valid [*pbeg] ) {
						wordstart = pbeg;
						while ( (size<MAX_SIZE_OF_WORD) && (_Valid[*pbeg] )) {
							size++;
							pbeg++;
						}

					}
					else {   // a "separator word" is parsed until an alphanumerical character is found

						size++;
						while ( (size<MAX_SIZE_OF_WORD) && (!_Valid[*pbeg] )) {
							size++;
							pbeg++;
						}
					}//else {  // a "separator word"
				}// end else  (!bufEmpty(B)) {
	  		}
	  		
			//The parsed word ...
			aWord=wordstart;
			j = search ((unsigned char *)aWord, size, &addrInTH );

			//encodes and outputs the code associated to the word.
			#ifdef VERSION_GETCODEANDWRITE			
				getCodeAndWrite(bw,j);
			#else
	   	 		getCode(j); 	    	 
			 	writeCodeWord(bw,&CODEWORD,&K);	
			#endif
				   	 		
			//Dynamic update of the data structures of the compressor//
			{
				register unsigned long p,f,i,h;
				register unsigned long temp;
			
				p=addrInTH;
			
				if (j==zeroNode) {		
					// if currentWord is a new word, then the "currentWord" is also sent
					// notice that the '\0' caracter in "currentWord" is also sent as mark to
					// enable decoder to distinguish where that word ends.
					writeBWPlusZero(bw,(unsigned char *)aWord,size);
				
				  	insertElement ((unsigned char *) aWord, size, &addrInTH);
					hash[p].len=size;
					hash[p].weight = 0;
	
					hash[p].posInVocab = zeroNode;
					
					positionInTH[zeroNode] = p;
					zeroNode++;
					
				}
			
				f = hash[p].weight;
				hash[p].weight +=1;
				
				i = hash[p].posInVocab;
				j = topBlock[f];
				h = positionInTH[j];
	
				temp = positionInTH[i];
				positionInTH[i]=  positionInTH[j];
				positionInTH[j] = temp;
				
				hash[p].posInVocab =j;
				hash[h].posInVocab =i;		
				
				topBlock[f]=j+1;
			
				if (MAXFREQ == f+1) { //The most frequent words need to be increased.
					topBlock[f+1] = 0;
					MAXFREQ++;
				}
							
			}//End of dynamic update
 
    	}/* inner while*/
    	nextbuf=pbeg;
      	bufLoad (B, (char*)nextbuf);
	}//while


	//processing the last buffer.
	if (!bufEmpty(B)){
		bufCurrent (B,(char**)&text,(char**)&top);
		pbeg = text; pend = top;

        while ( pbeg < pend )   {

			//parsing a word or separator
			size=0;
			wordstart = pbeg;
			if (_Valid[*pbeg]) {   //alphanumerical data
	
				while ( (size<MAX_SIZE_OF_WORD) && (  _Valid[*pbeg] )) {
						size++;
						pbeg++;
				}
	
		    }
	      	else {
				if (*pbeg != ' ') { //a separator comes starting in ' ' comes, so it is a new word
	
					while ( (size<MAX_SIZE_OF_WORD) && (!_Valid[*pbeg])) {
	
						size++;
						pbeg++;
					}
				}
				else {  //a  SPACE comes, so we have to test if next character is alphanumerical or not
	
					pbeg++;
					if (_Valid [*pbeg] ) {
						wordstart = pbeg;
						while ( (size<MAX_SIZE_OF_WORD) && (_Valid[*pbeg] )) {
							size++;
							pbeg++;
						}
	
					}
					else {   // a "separator word" is parsed until an alphanumerical character is found
	
						size++;
						while ( (size<MAX_SIZE_OF_WORD) && (!_Valid[*pbeg] )) {
							size++;
							pbeg++;
						}
					}//else {  // a "separator word"
				}// end else  (!bufEmpty(B)) {
	  		}
	  		if ( pbeg >= pend ) {  //need in case of the last word been parsed further away of cend. 
	  			size -= (pbeg-pend);	
	  		}
	  		
			//The parsed word ...
			aWord=wordstart;
			j = search ((unsigned char *)aWord, size, &addrInTH );

			//encodes and outputs the code associated to the word.
   	 		getCode(j); 	    	 
		 	writeCodeWord(bw,&CODEWORD,&K);
   	 		
			//Dynamic update of the data structures of the compressor//
			{
				register unsigned long p,f,i,h;
				register unsigned long temp;
			
				p=addrInTH;
			
				if (j==zeroNode) {		
					// if currentWord is a new word, then the "currentWord" is also sent
					// notice that the '\0' caracter in "currentWord" is also sent as mark to
					// enable decoder to distinguish where that word ends.
					writeBWPlusZero(bw,(unsigned char *)aWord,size);
				
				  	insertElement ((unsigned char *) aWord, size, &addrInTH);
					hash[p].len=size;
					hash[p].weight = 0;
	
					hash[p].posInVocab = zeroNode;
					
					positionInTH[zeroNode] = p;
					zeroNode++;
					
				}
			
				f = hash[p].weight;
				hash[p].weight +=1;
				
				i = hash[p].posInVocab;
				j = topBlock[f];
				h = positionInTH[j];
	
				temp = positionInTH[i];
				positionInTH[i]=  positionInTH[j];
				positionInTH[j] = temp;
				
				hash[p].posInVocab =j;
				hash[h].posInVocab =i;		
				
				topBlock[f]=j+1;
			
//				if (MAXFREQ == f+1) { //The most frequent words need to be increased.
//					topBlock[f+1] = 0;
//					MAXFREQ++;
//				}
							
			}//End of dynamic update
 
    	}/* inner while*/
  	}//if (!bufEmpty(B) ... ends processing last buffer



	/********************** end of compression process **********************/

	MAXFREQ = hash[positionInTH[0]].weight; //Needed to the summary shown at the end.
	bufDestroy(B);  //freeing resources.
	close(f);
	closeBW(bw);

	free(hash);
	free(topBlock);
	free(positionInTH);



} //mainCompressor


/*------------------------------------------------------------------
 Obtains the size of the file to compress.
------------------------------------------------------------------ */
unsigned long fileSize (char *filename){
	FILE *fpText;
	unsigned long fsize;
	fpText = fopen(filename,"rb");
	fsize=0;
	if (fpText) {
		fseek(fpText,0,2);
		fsize= ftell(fpText);
		fclose(fpText);
		fprintf(stderr,"fileSize = %ld",fsize);
	}
	return fsize;
}
/*------------------------------------------------------------------
 Obtains the size of the file to compress.
------------------------------------------------------------------ */
void writeSizeVocabulary (char *filename, unsigned long valueSIZEVOC, unsigned long valueMAXFREQ) {
	char fileNametmp[24];
	int file;
	
	strcpy (fileNametmp,filename);
	strcat(fileNametmp,".voc");
	
	if ( (file = open (fileNametmp, O_WRONLY|O_CREAT, 0666)) ==-1)
	{
		fprintf(stderr,"\nError while opening file: %s\n exiting...\n",fileNametmp);
		exit(0);
	}
	else  fprintf(stderr,"\nFILE: %s OPENED OK\n",fileNametmp);

	fprintf(stderr,"   *Saving: VOCSIZE=%ld, MAXFREQ=%ld !\n",valueSIZEVOC, valueMAXFREQ);		
	write(file,  &valueSIZEVOC,  4);
	write(file,  &valueMAXFREQ,  4);
	close(file);
}



/*------------------------------------------------------------------ */
/*
 *  COMPRESSOR PROGRAM OF DYNAMIC ETDC.
 */
/*------------------------------------------------------------------ */

int main(int argc, char* argv[])
{
	char fileName[20];
	long sizeNValue,sizeNValue2;
	unsigned long bytesFile;

	StartRusageTime();

	showMessage();

	if (argc < 3) {
		fprintf(stderr,"\n Wrong call to DLETDC\n");
		fprintf(stderr," Use:  %s  <in file>  <out file> [<VOC SIZE> <TOP SIZE>] \n",argv[0]);
		return 0;
	}

	sizeNValue=MAX_LEN_VOCABULARY;
	sizeNValue2=INITIAL_TOP_FREQ;

	//Estimation (Using Heap's law) of the size of the vocabulary (in number of distinct words).
	//sizeNValue=N_value;
	bytesFile = fileSize(argv[1]);
	if (!bytesFile) {
		fprintf(stderr, "\nFILE EMPTY OR FILE NOT FOUND !!\nCompression aborted\n");
		exit(0);
	}

	if(bytesFile<5000000) bytesFile = 5000000;
	sizeNValue = (unsigned long) floor(3.9* pow(bytesFile,0.69) );
	sizeNValue2 = sizeNValue *4;
	
	if (sizeNValue > MAX_LEN_VOCABULARY) {sizeNValue = MAX_LEN_VOCABULARY;}

	if (argc ==5) {
		  sizeNValue = atol(argv[3]);
		  sizeNValue2 = atol(argv[4]);
	}

	fprintf(stderr,"\nMax size of voc set[%ld] and Max Freq [%ld] allowed",sizeNValue,sizeNValue2);

	mainCompressor(argv[1], argv[2],sizeNValue,sizeNValue2);
	
	//uses an external file to store the number of words in the vocabulary.
	writeSizeVocabulary (argv[2], zeroNode,MAXFREQ);
	
	StopRusageTime("timesCDETDC",argv[1]);
	fprintf(stderr,"\nExact Number of words in vocabulary: %ld ",zeroNode);
	fprintf(stderr,"\nFinal maximum word frequency: %ld ",MAXFREQ);
	fprintf(stderr,"\n\n*Compression finished.\n");
}



