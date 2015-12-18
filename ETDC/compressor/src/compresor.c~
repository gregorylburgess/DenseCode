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
 File       : Compressor of ETDC. main file.
 ------------------------------------------------------------------------*/

#include "compressor.h"


/*------------------------------------------------------------------
 Function used by qsort to compare two elements a and b
------------------------------------------------------------------*/
int comparaFrecListaDesc(const void *a, const void *b)
{
	unsigned int *left,*right;
	left =  (unsigned int *) a;
	right = (unsigned int *) b;
    if ( hash[*left].weight < hash[*right].weight)
                return 1;
    if ( hash[*left].weight > hash[*right].weight)
                return -1;

    return 0;
}


/*------------------------------------------------------------------
 Initilizes the structures used.
 ---------------------------------------------------------------- */
void initialize (unsigned int sizeVoc){

	_memMgr = createMemoryManager();
    initialize_hash(sizeVoc);

    positionInTH = (unsigned int*) malloc (sizeVoc * sizeof(unsigned int));
	zeroNode = 0;
}

/*------------------------------------------------------------------
 Generates sequentially all the codes of the sorted vocabulary.
 ---------------------------------------------------------------- */
void GeneraCodigosETDC (unsigned int v)
{
	register unsigned int i;

	register unsigned int x,y,z,t;	// codeword bytes -> [x][y][z][t]
	unsigned int p0,p1,p2,p3;
	unsigned int base[4];

	unsigned int n;    		//codeword as a number
	register int k;				//codeword lenght.

	p0=256*256*256;
	p1=256*256;
	p2=256;
	p3=-1;   //no se usa


	//const unsigned int TABLABASE[5] = {0,128,16512,2113664,270549120};

	base[3]=128;
	base[2]=128*128 + base[3];
	base[1]= 128*128*128 + base[2];
	base[0]=-1;

	i=0;
	k=1;

 	x=0; y=0; z=0;t=0;

	//x=s;
	while ( (i<v) && (x<MAX) ) {
		//y=s;
		while ( (i<v) && (y<MAX) ) {
			//z=s;
			while ( (i<v) && (z<MAX) ) {
				//t=0;
				n=x*p0 + y*p1 + z*p2 + t;
				while ( (i<v) && (t<128) ) {
					hash[positionInTH[i]].codeword=n;
					hash[positionInTH[i]].size=k;
                    //DEBUG
					//fprintf(stderr,"\n word [%s], codeword [%d]",hash[positionInTH[i]].word,hash[positionInTH[i]].codeword);

					n++;
					i++;
					t++;
				}
				if (i!=base[3]) {z++;} else{z=128;k++;}
				t=0;
				//z++;
			}
			if ( i!= base[2] ) {y++;} else {y=128;z=128;k++;}
			z=128;
			//y++;
		}
		if (i != base[1]) {x++;} else {x=128;y=128;z=128;k++;}
		y=128;
		//x++;
	}
}


/*------------------------------------------------------------------
 * Performs the full encoding process.
   Two passes through the text:
		1- first pass: gather words and frequencies.
		  * Encoding process performed in the vocabulary
		2- second pass:
		  * Replaces words by codewords.
	Outputs:
		- compressed file
		- file with the vocabulary.
------------------------------------------------------------------ */
void mainCompressor (char* infile, char* outfile, unsigned int N_val) {

	unsigned int totalVocSize=0;
	unsigned int codeword,tam;
	BufferRead B;
	unsigned char *text,*top, *nextbuf;
	register unsigned char *pbeg,*pend,*wordstart;
	register unsigned int size;
	BufferWrite bw,voca;

	register unsigned int k,i,j;

	int f;
	char *voc;
	unsigned char *aWord;

#ifdef UNCOMPRESSED_VOC
    voc = (char *) malloc(strlen(outfile)+256); //name of the vocabulary file
#endif

    StartValid();

	// Initialization of read buffer
	f = open (infile, O_RDONLY);
	if (f == -1) {
		printf("Cannot read file %s",infile);
		exit(0);
	}

	B = bufCreate(MAX_SIZE_OF_WORD+5);//buffer will have at least 5 bytes after a word

	bufSetFile (B,f);

	initialize(N_val); //initializes compressor for up to N_value words

	fprintf(stderr, "\nSTARTING COMPRESSION...");

	/********************** Beginning of first pass **********************/

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
					//fprintf(stderr,"\n Entra 3 pbeg [%d] pend [%d] valid [%d]",pbeg,pend, _Valid[*pbeg]);
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
				}//if (!bufEmpty(B)) {
			}

			aWord=wordstart;  //the word parsed.

	        j = search ((unsigned char *)aWord, size, &addrInTH );

	        if (j==zeroNode) {
				insertElement ((unsigned char *) aWord, size, &addrInTH);
				hash[addrInTH].weight = 0;
				hash[addrInTH].size = 0;
				hash[addrInTH].len=size;
				positionInTH[zeroNode] = addrInTH;
				zeroNode++;
				totalVocSize+=size+1;
			}

			hash[addrInTH].weight +=1;

    	}/* internal while*/
    	//fprintf(stderr,"\n Procesa la palabra1 [%s]",hash[addrInTH].word);

		nextbuf=pbeg;
		bufLoad (B, (char*)nextbuf);

	}

	//processing the last buffer.


	if (!bufEmpty(B)){
	 	bufCurrent (B,(char**)&text,(char**)&top);
	 	pbeg = text; pend = top;

		while ( pbeg < pend )   {

			size=0;
			wordstart = pbeg;
			if (_Valid[*pbeg]) {   //alphanumerical data
		    	while ( (size<MAX_SIZE_OF_WORD) && (  _Valid[*pbeg] ) &&( pbeg < pend )) {

						size++;
						pbeg++;
				}
		    }
			else {
				if (*pbeg != ' ') { //a separator comes starting in ' ' comes, so it is a new word

					while ( (size<MAX_SIZE_OF_WORD) && (!_Valid[*pbeg])&&( pbeg < pend )) {

						size++;
						pbeg++;
					}
				}
				else {  //a  SPACE comes, so we have to test if next character is alphanumerical or not

				//if (!bufEof(B)) { //not end of file
	                pbeg++;
					if (_Valid [*pbeg] ) {
						wordstart = pbeg;
						while ( (size<MAX_SIZE_OF_WORD) && (_Valid[*pbeg] )&&( pbeg < pend )) {
							size++;
							pbeg++;
						}

					}
					else {   // a "separator word" is parsed until an alphanumerical character is found

						size++;
						while ( (size<MAX_SIZE_OF_WORD) && (!_Valid[*pbeg] )&&( pbeg < pend )) {
							size++;
							pbeg++;
						}
					}//else {  // a "separator word"
				}//if (!bufEmpty(B)) {

			}
	  		
	  		if ( pbeg >= pend ) {  //need in case of the last word been parsed further away of cend. 
	  			size -= (pbeg-pend);	
	  		}
			aWord=wordstart;

			j = search ((unsigned char *)aWord, size, &addrInTH );

			if (j==zeroNode) {
				insertElement ((unsigned char *) aWord, size, &addrInTH);
				hash[addrInTH].weight = 0;
				hash[addrInTH].size = 0;
				hash[addrInTH].len=size;
				positionInTH[zeroNode] = addrInTH;
				zeroNode++;
				totalVocSize+=size+1;
		   	}

			hash[addrInTH].weight +=1;

    	}/* while interno*/

	}//if (!bufEmpty(B){

	fprintf(stderr,"\n Number of words in the vocabulary:  %ld :sizeofVoc = %d\n",(long)zeroNode,totalVocSize);

	/********************** end of first pass **********************/

	bw= bufCreateWriter(outfile);
	openBW(bw);

#ifdef UNCOMPRESSED_VOC
	strcpy(voc,outfile);
    strcat(voc,".voc");
	voca=bufCreateWriter(voc);
	openBW(voca);
#endif



	// Sorting the vocabulary by frequency.

	{	//Moves all the words with frequency = 1 to the end of the vocabulary.
		register long ii;
		register long kk;

	    kk=zeroNode-1;
	    ii=0;
	    while (ii<kk){
			while ((hash[positionInTH[ii]].weight!=1) && (ii <kk)) { ii++; }
			while ((hash[positionInTH[kk]].weight==1) && (ii <kk)) { kk--; }

			if (ii<kk){
				swap(&positionInTH[ii], &positionInTH[kk]);
				kk--;
				ii++;
			}
	  	}

		//k=ii; 
		k=ii+1; //the lenght of the vector to be sorted with qsort. So v[0 .. k-1]
	}

	//Aplies qsort to the words with frequency > 1.
	qsort(positionInTH,k,sizeof(unsigned int),comparaFrecListaDesc);

	//Generates codes sequentially

	GeneraCodigosETDC (zeroNode);
//DEBUG
    char buffer[32] = {0};
	for (i=0;i<zeroNode;i++)
        //itoa(hash[positionInTH[i]].codeword,buffer,2);
        fprintf(stderr,"\n word [%s], weight [%u], code[%x]",hash[positionInTH[i]].word, hash[positionInTH[i]].weight, hash[positionInTH[i]].codeword);
        fprintf(stderr,"]");
#ifdef UNCOMPRESSED_VOC
	{ //UNCOMPRESSED VOCABULARY
	writeBW(voca,(unsigned char *)&zeroNode,4); //stores the number of words of the vocabulary

	//Writes the vocabulary to disk.
	for (i=0;i<zeroNode;i++)
		writeBW(voca,(unsigned char *)hash[positionInTH[i]].word,hash[positionInTH[i]].len+1);

	close(f);
	
	}
#endif

#ifdef COMPRESSED_VOC
    voc = (char *) malloc(strlen(outfile)+10); //name of the vocabulary file
    voc2 = (char *) malloc(strlen(outfile)+10); //name of the vocabulary file
	sprintf(voc,"%s.voc.huff",outfile);
	sprintf(voc2,"%s.cvoc",outfile);

	
	{	//VOCABULARY IS COMPRESSED WITH HUFFMAN.
		unsigned char *memBuf = (byte *) malloc (sizeof(unsigned char) *  totalVocSize);
		unsigned char *dst = memBuf;  //vocabulary zone with all its chars.
		unsigned char *src;
		
		uint freqs[256];
		
		for (i=0;i<256;i++) freqs[i]=0;
		
		//prepares a buffer with the vocabulary words (by freq) and computes freqs[]
		for (i=0;i<zeroNode;i++) {
			src = (unsigned char *)hash[positionInTH[i]].word;
			while (*src) {
				freqs[*src] ++;
				*dst++=*src++;
			}
			*dst++ = '\0';					
		}
		freqs[0]=zeroNode; //a '\0' for each word.		

	
		THuff huffVoc = createHuff (freqs,255,UNSORTED);

		unsigned int *bitVector;  //compressed vocabulary.
		unsigned int totalBits;
		{//encode the vocab into "bitVector"
			totalBits = huffVoc.total;
			printf("\n totalBits for Huffman = %d",totalBits);
			bitVector = (unsigned int *) malloc (sizeof (unsigned int) * (totalBits + 31)/32);
			bitVector[(totalBits + 31)/32 -1]=0000;
			unsigned int ptr=0;
			for (i=0;i<totalVocSize;i++) {
				ptr=encodeHuff(huffVoc, memBuf[i], bitVector, ptr);		
			}	
		}	
		
		prepareToDecode(&(huffVoc));
		
		{FILE *f;
			f = fopen(voc,"w");
			saveHuffAfterDecode(huffVoc,f);
			fclose(f);
		}

		freeHuff(huffVoc);

		{FILE *f;
			f = fopen(voc2,"w");
			fwrite(&zeroNode,sizeof(unsigned int), 1,f);		
			fwrite(&totalVocSize,sizeof(unsigned int), 1,f);		
			fwrite(&totalBits,sizeof(unsigned int), 1,f);
			fwrite(bitVector,sizeof(unsigned int), ((totalBits+31)/32),f);
			fclose(f);
			free(bitVector);
		}
		
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
			bitVector = (unsigned int *) malloc (sizeof (unsigned int) * ((totalBits + 31)/32));
			bitVector[(totalBits + 31)/32 -1]=0000;				
			fread(bitVector,sizeof(unsigned int), ((totalBits+31)/32),f);
			fclose(f);
		}
			
	
		//decodes the compressed buff into memBuf2.
		unsigned char *memBuf2 = (byte *) malloc (sizeof(unsigned char) *  totalVocSize);	
		unsigned char *aChar = memBuf2;

		unsigned int ptr=0;
		unsigned int pos;
		while (ptr < totalBits) {
			ptr=decodeHuff (&huffVoc, &pos, bitVector, ptr);
			//fprintf(stderr,"%c. \n",pos);
			*aChar =(unsigned char) pos;
			aChar++;
		}
		
		free(bitVector);
		
		//chequea que descomprime ok!
		for (i=0;i<totalVocSize;i++) {
			if (memBuf[i] != memBuf2[i]) { 
				printf("\n distintos en pos %d: [%d]!=[%d]",i,memBuf[i],memBuf2[i]);
				printf("\n distintos en pos %d: [%s][%s]!=[%s][%s]",i,&memBuf[i-3],&memBuf[i],&memBuf2[i-3],&memBuf2[i]);
				//exit(0);
			}
		}
		printf("\n SON IGUALES !!!");		
		freeHuff(huffVoc);
	
		free(memBuf2);	
	

	free(memBuf);
	}
	free(voc);free(voc2);
	
#endif



	f = open (infile, O_RDONLY);
	if (f == -1)
		printf("Cannot read file %s",infile);


	/********************** Beginning of the second pass **********************/

	bufSetFile (B,f);

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
				}//if (!bufEmpty(B)) {

			}

			aWord=wordstart; //the parsed word

			j = search ((unsigned char *)aWord, size, &addrInTH );

			codeword = hash[addrInTH].codeword;
			tam=hash[addrInTH].size;
			writeCodeWord(bw,&codeword,&tam);

    	}/* inner while */
    	nextbuf=pbeg;
        bufLoad (B, (char*)nextbuf);
	}//end while


	//processing the last buffer.
	if (!bufEmpty(B)){
	 	bufCurrent (B,(char**)&text,(char**)&top);
	 	pbeg = text; pend = top;

		while ( pbeg < pend )   {

			//parses a word
			size=0;
			wordstart = pbeg;
			if (_Valid[*pbeg]) {   //alphanumerical data
		    	while ( (size<MAX_SIZE_OF_WORD) && (  _Valid[*pbeg] ) &&( pbeg < pend )) {
					size++;
					pbeg++;
				}
		    }
			else {
				if (*pbeg != ' ') { //a separator comes starting in ' ' comes, so it is a new word
					while ( (size<MAX_SIZE_OF_WORD) && (!_Valid[*pbeg])&&( pbeg < pend )) {
						size++;
						pbeg++;
					}
				}
				else {  //a  SPACE comes, so we have to test if next character is alphanumerical or not

	                pbeg++;
					if (_Valid [*pbeg] ) {
						wordstart = pbeg;
						while ( (size<MAX_SIZE_OF_WORD) && (_Valid[*pbeg] )&&( pbeg < pend )) {
							size++;
							pbeg++;
						}
					}
					else {   // a "separator word" is parsed until an alphanumerical character is found

						size++;
						while ( (size<MAX_SIZE_OF_WORD) && (!_Valid[*pbeg] )&&( pbeg < pend )) {
							size++;
							pbeg++;
						}
					}//else {  // a "separator word"
				}//if (!bufEmpty(B)) {
			}

			aWord=wordstart;  //the parsed word

			j = search ((unsigned char *)aWord, size, &addrInTH );
			codeword = hash[addrInTH].codeword;
			tam=hash[addrInTH].size;
			writeCodeWord(bw,&codeword,&tam);

    	}/* inner while */
	}

	/********************** end of second pass **********************/

	bufDestroy(B);  //freeing resources.
	closeBW(bw);
	destroyBW(bw);

#ifdef UNCOMPRESSED_VOC
	closeBW(voca);
	destroyBW(voca);
	free(voc);	
#endif

	free(hash);
	free(positionInTH);
	destroyMemoryManager(_memMgr);

	
} //mainCompressor

/*------------------------------------------------------------------
 Obtains the size of the file to compress.
 ------------------------------------------------------------------ */
unsigned int fileSize (char *filename) {
	FILE *fpText;
	unsigned int fsize;
	fpText = fopen(filename,"rb");
	fsize=0;
	if (fpText) {
		fseek(fpText,0,2);
		fsize= ftell(fpText);
		fclose(fpText);
		fprintf(stderr,"fileSize = %ld", (long) fsize);
	}
	return fsize;
}

/* Shows a message with info of this version of the code */
void showMessage() {
	printf("\n*End-Tagged Dense Code v0.9 [semistatic compressor]");
	printf("\n*CopyRight(c)2006  Nieves R. Brisaboa, Gonzalo Navarro, Antonio Fariña and José R. Paramá\n");
	
    printf("\n  #ETDC comes with ABSOLUTELY NO WARRANTY; ");
    printf("\n  #This is free software, and you are welcome to redistribute it ");
    printf("\n  #under certain conditions\n\n");
}

/*------------------------------------------------------------------ */
/*
 *  COMPRESSOR PROGRAM OF ETDC.
 */
/*------------------------------------------------------------------ */

int main(int argc, char* argv[])
{
	int sizeNValue;
	unsigned int bytesFile;

	StartRusageTime();

	showMessage();

	//printf("ARGC VALE [%d]\n",argc);
	if (argc < 3) {
		fprintf(stderr,"\n Wrong call to ETDC compressor\n");
		fprintf(stderr," Use:  %s  <in file>  <out file> <[VOC SIZE]> \n",argv[0]);
		return 0;
	}

	//Estimation (Using Heap's law) of the size of the vocabulary (in number of distinct words).
	//sizeNValue=N_value;
	bytesFile = fileSize(argv[1]);
	if (!bytesFile) {
		fprintf(stderr, "\nFILE EMPTY OR FILE NOT FOUND !!\nCompression aborted\n");
		exit(0);
	}

	if(bytesFile<5000000) bytesFile = 5000000;
	sizeNValue = (unsigned int) floor(3.9* pow(bytesFile,0.69) );

	if (argc ==4) {
		unsigned int sizetmp;
		sizetmp = atol(argv[3]);
		//if (sizetmp > sizeNValue) {sizeNValue = sizetmp;}
		sizeNValue = sizetmp;
	}

	fprintf(stderr,"\nMax size of voc allowed %ld", (long)sizeNValue);

	mainCompressor(argv[1], argv[2],sizeNValue);
	StopRusageTime("timesComp",argv[1]);
	fprintf(stderr,"\nExact Number of words in vocabulary: %ld ", (long)zeroNode);
	fprintf(stderr,"\n\n*Compression finished.\n");
	return (0);
}



