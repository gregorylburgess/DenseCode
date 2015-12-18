/* Plain Huffman. -- 
A semistatic word-based byte oriented compressor for text files.

Programmed by Antonio Fariña
Copyright (C) 2006  Nieves R. Brisaboa, Gonzalo Navarro, Antonio Fariña and José R. Paramá
Author's contact: antonio.fari@gmail.com
WEB: http://vios.dc.fi.udc.es/codes

Programmed as a baseline for the paper: 

N. Brisaboa, A. Fariña, G. Navarro, and J. Paramá. Lightweight natural 
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
/*-----------------------------------------------------------------------
 File       : Compressor of PH. main file.
 ------------------------------------------------------------------------*/
#include "compressor.h"

/*------------------------------------------------------------------
 Initilizes the structures used.
 ---------------------------------------------------------------- */
void initialize (unsigned long tamArq){
	_memMgr = createMemoryManager(); //_memMgr.initialize();
	initialize_hash(tamArq);
	_previousAlfanumerical = 0;
	positionInTH 	= (unsigned long*) malloc(tamArq*sizeof(unsigned long));
	zeroNode = 0;
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
void mainCompressor (char* infile, char* outfile, unsigned long N_val) {

	unsigned long codeword;
	BufferRead B;
	unsigned char *text,*top, *nextbuf;
	register unsigned char *pbeg,*pend,*wordstart;
	register unsigned long size;
	BufferWrite bw,voca;
    unsigned long Resto, numNodosInternos;
	register unsigned long k,i,*comezo,j;
	char aux[MAX_SIZE_OF_WORD];

	int f;

	/**/
	 char *voc;
	unsigned char *aWord;

    voc = (char *) malloc(strlen(infile)+4);

    StartValid();

	// Inicialización del buffer de lectura
	f = open (infile, O_RDONLY);
	if (f == -1)
		printf("Cannot read file %s",infile);

	B = bufCreate(MAX_SIZE_OF_WORD+5);    //número de "bytes" que hay que dejar al final del buffer siempre, por culpa

	bufSetFile (B,f);

	// Inicialización del buffer de escritura

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

				while ( (size<MAX_SIZE_OF_WORD) && (  _Valid[*pbeg] )) {size++;pbeg++;}
		    }
			else {
				if (*pbeg != ' ') { //a separator comes starting in ' ' comes, so it is a new word
					while ( (size<MAX_SIZE_OF_WORD) && (!_Valid[*pbeg])) {size++;pbeg++;}
				}
				else {  //a  SPACE comes, so we have to test if next character is alphanumerical or not
	                pbeg++;
					if (_Valid [*pbeg] ) {
						wordstart = pbeg;
						while ( (size<MAX_SIZE_OF_WORD) && (_Valid[*pbeg] )){size++;pbeg++;}
					}
					else {   // a "separator word" is parsed until an alphanumerical character is found
						size++;
						while ( (size<MAX_SIZE_OF_WORD) && (!_Valid[*pbeg] )) {size++;pbeg++;}
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
			}
			hash[addrInTH].weight +=1;
    	}/* inner while*/
    	nextbuf=pbeg;
      bufLoad (B, (char*)nextbuf);
	}//while(1)

	//processing the last buffer.
	if (!bufEmpty(B)){
	 	bufCurrent (B,(char**)&text,(char**)&top);
	 	pbeg = text; pend = top;

		while ( pbeg < pend )   {
			size=0;
			wordstart = pbeg;
			if (_Valid[*pbeg]) {   //alphanumerical data
		    	while ( (size<MAX_SIZE_OF_WORD) && (  _Valid[*pbeg] ) &&( pbeg < pend )) {size++;pbeg++;}

		    }
			else {
				if (*pbeg != ' ') { //a separator comes starting in ' ' comes, so it is a new word
					while ( (size<MAX_SIZE_OF_WORD) && (!_Valid[*pbeg])&&( pbeg < pend )) {size++;pbeg++;}
				}
				else {  //a  SPACE comes, so we have to test if next character is alphanumerical or not
	                pbeg++;
					if (_Valid [*pbeg] ) {
						wordstart = pbeg;
						while ( (size<MAX_SIZE_OF_WORD) && (_Valid[*pbeg] )&&( pbeg < pend )) {size++;pbeg++;}
					}
					else {   // a "separator word" is parsed until an alphanumerical character is found
						size++;
						while ( (size<MAX_SIZE_OF_WORD) && (!_Valid[*pbeg] )&&( pbeg < pend )) {size++;pbeg++;}
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
		}
         hash[addrInTH].weight +=1;
    	}/* inner while*/
	}//if (!bufEmpty(B){

	/********************** end of first pass **********************/

	InicializaNodosEnNiveles (&tablaNodosEnNiveles,5);

	strcpy(voc,outfile);
    strcat(voc,".voc");
	bw= bufCreateWriter(outfile);
	voca=bufCreateWriter(voc);
	openBW(bw);
	openBW(voca);

	/* sorting by frequency */
	{register long ii;
	 register long kk;
	
	    kk=zeroNode-1;
	    ii=0;
	    while (ii<kk){
			while ((hash[positionInTH[ii]].weight==1) && (ii <kk)) { ii++; }
			while ((hash[positionInTH[kk]].weight!=1) && (ii <kk)) { kk--; }
	
			if (ii<kk){
				swap(&positionInTH[ii], &positionInTH[kk]);
				kk--;
				ii++;
			}
	  	}
	
	    k=kk;
	
		comezo=&positionInTH[k];
		
		//Aplies qsort to the words with frequency > 1.
		qsort(comezo,zeroNode-k,sizeof(unsigned long),comparaFrecListaAsc);
	}

	writeBW(voca,(unsigned char *)&zeroNode,4); //stores the number of words of the vocabulary

	CreateTables (zeroNode,&tablaNodosEnNiveles);
	
#ifdef DDEBUGCODES
	uint z;
	printf("\n\t\t...Codes assigned to each entry (decreasing freq)");
	for (i=0;i<=4;i++) {
	        uint addrInTH = positionInTH[i];
		codeword = LE_LONG32(hash[addrInTH].codeword);
		unsigned char *cod = (unsigned char *)&codeword;
		printf("\n\t\t %7lu --> cod = ", i);
		for (z=0; z<hash[addrInTH].size;z++)
		    printf("[%u]",cod[z]);		
	}
      printf("\n\t\t...\n\t\t...");
	for (i=zeroNode -4 ;i<zeroNode;i++) {
	        uint addrInTH = positionInTH[i];
		codeword = LE_LONG32(hash[addrInTH].codeword);
		unsigned char *cod = (unsigned char *)&codeword;
		printf("\n\t\t %7lu --> cod = ", i);
		for (z=0; z<hash[addrInTH].size;z++)
		    printf("[%u]",cod[z]);		
	}
	printf("\n");
	fflush (stdout);
	fflush(stderr);
#endif		
	
	/* Guardo mínNivel*/
	
	sprintf(aux,"%u",tablaNodosEnNiveles.minNivel);
	writeBW(voca,(unsigned char *)aux,strlen(aux)+1);
	/* Guardo maxNivel*/
	
	sprintf(aux,"%u",tablaNodosEnNiveles.maxNivel);
	writeBW(voca,(unsigned char *)aux,strlen(aux)+1);
	/* Guardo el Base y first*/
	
	for(i=tablaNodosEnNiveles.minNivel;i<=tablaNodosEnNiveles.maxNivel;i++){
	  sprintf(aux,"%lu",tablaNodosEnNiveles.base[i]);
	  writeBW(voca,(unsigned char *)aux,strlen(aux)+1);
	  sprintf(aux,"%lu",tablaNodosEnNiveles.first[i]);
	  writeBW(voca,(unsigned char *)aux,strlen(aux)+1);
	}
	
	//for (i=tablaNodosEnNiveles.minNivel; i<= tablaNodosEnNiveles.maxNivel; i++) {
	//		fprintf(stderr,"\n Base[%d] -> %d; first[%d] --> %d",i,tablaNodosEnNiveles.base[i],i,tablaNodosEnNiveles.first[i]);
	//	}
	
	for (i=0;i<zeroNode;i++){
	  writeBW(voca,(unsigned char *)hash[positionInTH[i]].word,hash[positionInTH[i]].len+1);
	}
	
	close(f);

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
			size=0;
			wordstart = pbeg;
			if (_Valid[*pbeg]) {   //alphanumerical data
				while ( (size<MAX_SIZE_OF_WORD) && (  _Valid[*pbeg] )) {size++;	pbeg++;	}
		    }
			else {
				if (*pbeg != ' ') { //a separator comes starting in ' ' comes, so it is a new word
					while ( (size<MAX_SIZE_OF_WORD) && (!_Valid[*pbeg])) {size++;pbeg++;}
				}
				else {  //a  SPACE comes, so we have to test if next character is alphanumerical or not
	                pbeg++;
					if (_Valid [*pbeg] ) {
						wordstart = pbeg;
						while ( (size<MAX_SIZE_OF_WORD) && (_Valid[*pbeg] )) {size++;pbeg++;}
					}
					else {   // a "separator word" is parsed until an alphanumerical character is found
						size++;
						while ( (size<MAX_SIZE_OF_WORD) && (!_Valid[*pbeg] )) {	size++;	pbeg++;}
					}//else {  // a "separator word"
				}//if (!bufEmpty(B)) {
			}

			aWord=wordstart; //the parsed word
			
			j = search ((unsigned char *)aWord, size, &addrInTH );
			codeword = LE_LONG32(hash[addrInTH].codeword);
			writeBW(bw,(unsigned char *)&codeword,hash[addrInTH].size);
    	}/* inner while */

    	nextbuf=pbeg;
        bufLoad (B, (char*)nextbuf);
	}//while(1)

	//processing the last buffer.
	if (!bufEmpty(B)){
	 	bufCurrent (B,(char**)&text,(char**)&top);
	 	pbeg = text; pend = top;
		while ( pbeg < pend )   {
			size=0;
			wordstart = pbeg;
			if (_Valid[*pbeg]) {   //alphanumerical data
		    	while ( (size<MAX_SIZE_OF_WORD) && (  _Valid[*pbeg] ) &&( pbeg < pend )) {size++;pbeg++;}
		    }
			else {
				if (*pbeg != ' ') { //a separator comes starting in ' ' comes, so it is a new word
					while ( (size<MAX_SIZE_OF_WORD) && (!_Valid[*pbeg])&&( pbeg < pend )) {size++;pbeg++;}
				}
				else {  //a  SPACE comes, so we have to test if next character is alphanumerical or not
	                pbeg++;
					if (_Valid [*pbeg] ) {
						wordstart = pbeg;
						while ( (size<MAX_SIZE_OF_WORD) && (_Valid[*pbeg] )&&( pbeg < pend )) {size++;pbeg++;}
					}
					else {   // a "separator word" is parsed until an alphanumerical character is found
						size++;
						while ( (size<MAX_SIZE_OF_WORD) && (!_Valid[*pbeg] )&&( pbeg < pend )) {size++;pbeg++;}
					}//else {  // a "separator word"
				}//if (!bufEmpty(B)) {
			}
			
			aWord=wordstart;  //the parsed word
			
			j = search ((unsigned char *)aWord, size, &addrInTH );
			codeword = LE_LONG32(hash[addrInTH].codeword);
			writeBW(bw,(unsigned char *)&codeword,hash[addrInTH].size);
    	}/* inner while*/
  	}//if (!bufEmpty(B){

	/********************** end of second pass **********************/

	bufDestroy(B);
	closeBW(bw);
	destroyBW(bw);
	
	closeBW(voca);
	destroyBW(voca);

	free(hash);
	free(voc);
	destroyMemoryManager(_memMgr);		
	
	free(tablaNodosEnNiveles.first);
	free(tablaNodosEnNiveles.base);
	free(positionInTH);
} //mainCompressor



/*------------------------------------------------------------------
 Obtains the size of the file to compress.
 ------------------------------------------------------------------ */
unsigned long fileSize (char *filename) {
	FILE *fpText;
	unsigned long fsize;
	fpText = fopen(filename,"rb");
	fsize=0;
	if (fpText) {
		fseek(fpText,0,2);
		fsize= ftell(fpText);
		fclose(fpText);
		//fprintf(stderr,"fileSize = %ld",fsize);
	}
	return fsize;
}

/* Shows a message with info of this version of the code */
void showMessage() {
	printf("\n*Plain Huffman Code [semistatic compressor]");
	printf("\n*CopyRight(c)2006  Nieves R. Brisaboa, Gonzalo Navarro, Antonio Fariña and José R. Paramá\n");
    printf("\n  #PH comes with ABSOLUTELY NO WARRANTY; ");
    printf("\n  #This is free software, and you are welcome to redistribute it ");
    printf("\n  #under certain conditions\n\n");
}

/* Shows some statistics */
void showRatio(char *inf, char *outf) {
	unsigned long bytesIn, bytesOut;
	char *filename = (char *) malloc ( (strlen (outf) + 5)  * sizeof(char));
		
	bytesIn = fileSize(inf);	
	bytesOut = fileSize(outf);
	sprintf(filename, "%s.voc",outf);
	bytesOut += fileSize(filename);
    
    fprintf(stderr,"\n ::stats: infile = %ld bytes, outfiles = %ld bytes, ratio = %2.3f%%\n",bytesIn,bytesOut,bytesOut*1.0/bytesIn *100.0);
    fprintf(stderr,  " :: !! vocabulary is kept uncompressed, see .voc file \n");
    free(filename);
}


/*------------------------------------------------------------------ */
/*
 *  COMPRESSOR PROGRAM OF PLAIN HUFFMAN.
 */
/*------------------------------------------------------------------ */

int main(int argc, char* argv[])
{
	char fileName[20];
	long sizeNValue;
	unsigned long bytesFile;

	StartRusageTime();

	showMessage();

	//printf("ARGC VALE [%d]\n",argc);
	if (argc < 3) {
		fprintf(stderr,"\n Wrong call to PH compressor\n");
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
	sizeNValue = (unsigned long) floor(3.9* pow(bytesFile,0.69) );

	if (argc ==4) {
		unsigned long sizetmp;
		sizetmp = atol(argv[3]);
		sizeNValue = sizetmp;
	}

	fprintf(stderr,"\nMax size of voc allowed %ld words \n ",sizeNValue);

	mainCompressor(argv[1], argv[2],sizeNValue);
	StopRusageTime("timesCompPH",argv[1]);
	fprintf(stderr,"\n\tExact Number of words in vocabulary: %ld ",zeroNode);
	showRatio(argv[1],argv[2]);	
	fprintf(stderr,"**Compression finished.**\n");

}


/* qsort comparison function */
int comparaFrecListaAsc(const void *a, const void *b)
{
	unsigned long *left,*right;
	left =  (unsigned long *) a;
	right = (unsigned long *) b;
    if ( hash[*left].weight > hash[*right].weight)
                return 1;
    if ( hash[*left].weight < hash[*right].weight)
                return -1;
    return 0;
}


/** creates the codes **/
/** Based on:
		http://www.cs.mu.oz.au/~alistair/abstracts/inplace.html
		http://www.cs.mu.oz.au/~alistair/inplace.c
**/
void calculate_minimum_redundancy(unsigned long n,
						 		  t_NodosEnNiveles *tablaNodosEnNiveles) {
	long root;                  /* next root node to be used */
	long leaf;                  /* next leaf to be used */
	register long next;                  /* next value to be assigned */
	register long avbl;                  /* number of available nodes */
	long used;                  /* number of internal nodes */
	register long dpth;                  /* current depth of leaves */
	register long x   ;                 		/* counter var */
	unsigned long Resto;         /* remainder */
	register unsigned long ndint;//,i;      	 /* Numero de Nodos Internos */
	t_word *A; /* Vetor A  rep. palabras dentro da funcao */
    A=hash;
    /* check for pathological cases */
    if (n==0) { return; }
    if (n==1) { hash[positionInTH[0]].weight = 0; return; }

    /* first pass, left to right, setting parent pointers */


     //Correction in Fariña's phD
/*     
    if (n<BASE_NUM)
      Resto=n;
    else if ( ((n-BASE_NUM) % (BASE_NUM -1)) == 0)
      Resto = BASE_NUM;
    else 
      Resto = 1 + ((n - BASE_NUM)%(BASE_NUM -1));
    
    ndint=1+((n-Resto)/(BASE_NUM-1));
 */  

/*
      //original in PH

      if( n>(BASE_NUM-1))
		Resto=1+((n-BASE_NUM)%(BASE_NUM-1));
	else 	Resto=n-1;

	ndint=1+((n-Resto)/(BASE_NUM-1));
	if(Resto<2) Resto=BASE_NUM;

*/	

	//Correction by Takuya Masaki - 2014
	if( n>(BASE_NUM-1))
		Resto=1+((n-BASE_NUM)%(BASE_NUM-1));
	else 	Resto=n-1;

	if(Resto<2) Resto=BASE_NUM;
	ndint=1+((n-Resto)/(BASE_NUM-1));
	





	A[positionInTH[0]].codeword = A[positionInTH[0]].weight;
	for(x=1; x<Resto; x++) {
		A[positionInTH[0]].codeword +=A[positionInTH[x]].weight;
	}

	root = 0; leaf = Resto;

	for (next=1; next<ndint; next++) {
			/* select first item for a pairing */
		if (leaf>=n || A[positionInTH[root]].codeword<A[positionInTH[leaf]].weight) {
				A[positionInTH[next]].codeword = A[positionInTH[root]].codeword; A[positionInTH[root++]].codeword = next;
		} else
				A[positionInTH[next]].codeword = A[positionInTH[leaf++]].weight;

		/* add on the other itens */
		for(x=1; x<BASE_NUM; x++) {
		  if (leaf>=n || (root<next && A[positionInTH[root]].codeword<A[positionInTH[leaf]].weight)) {
			A[positionInTH[next]].codeword += A[positionInTH[root]].codeword; A[positionInTH[root++]].codeword = next;
		  } else
			A[positionInTH[next]].codeword += A[positionInTH[leaf++]].weight;
		}
	}

	/* second pass, right to left, setting internal depths */

	// O(n/256)
	A[positionInTH[next-1]].codeword = 0; root = next-1;
	for (next=root-1; next>=0; next--)
			A[positionInTH[next]].codeword = A[positionInTH[A[positionInTH[next]].codeword]].codeword+1;

	// O(n + n/256)
	/* third pass, right to left, setting leaf depths */
	avbl = 1; used = dpth = 0; next = n-1;
	while (avbl>0) {
			//miro cuantos son los nodos internos del nivel actual dpth
			while (root>=0 && A[positionInTH[root]].codeword==dpth) {
					used++; root--;
			}
		    x=0;

			while (avbl>used) {   // O(avail - used) = O(hojas en nivel) o lo que es lo mismo
								  // O(total nodos en nivel - nodos internos en nivel)
				   A[positionInTH[next--]].codeword = dpth;
				   avbl--;
				   x++;
		  		   if(next<0) { used=0; break; }
			}
			avbl = BASE_NUM*used;     //guarda el número de nodos disp. en nivel
			dpth++;
			used = 0;
	}
}



/** tables that will be needed for decoding **/
void  CreateTables(unsigned long int numPalabras,
						 t_NodosEnNiveles *TN)
{
	register unsigned long x,nivel_atual,codigo=0;
	unsigned long Max,TotalElementos;
	unsigned  long *Base,*First;
	unsigned long *Niveis;
	
	calculate_minimum_redundancy(numPalabras,&tablaNodosEnNiveles);
	
	Max = hash[positionInTH[0]].codeword+1;
	TN->maxNivel = Max;
	TN->minNivel = 1;
	
	TotalElementos=numPalabras;
	for(x=0;x<Max;x++) {
		TN->first[x]=TotalElementos; // Inicializa valores das tabelas
		TN->base[x]=TotalElementos;
	}

	//GENERATING THE CANONICAL CODES.

	codigo = 0;
	x=0;
	nivel_atual = hash[positionInTH[x]].codeword;
	while ( x < TotalElementos ) {
		TN->base[nivel_atual]  = x;
		TN->first[nivel_atual] = codigo;
		while( hash[positionInTH[x]].codeword == nivel_atual ) {
			hash[positionInTH[x]].size = nivel_atual;
			
			hash[positionInTH[x]].codeword  = (codigo<<(32-MULT(nivel_atual))  );
			
			codigo++;
			x++;
			if ( x == TotalElementos ) break;
	    }
	    codigo --;
		if( x < TotalElementos ) {
			while(nivel_atual > hash[positionInTH[x]].codeword) {
				codigo = DESLOC(codigo);
				nivel_atual --;
			}
			codigo ++;
		}
	}
		
	x=0;
	while ( ( x < Max ) && ( TN->base[x] == TotalElementos ) ) x++;
	
	TN->minNivel = x;
	
	while(x<Max) {
		if (TN->base[x]==TotalElementos) TN->base[x]=TN->base[x-1];
		x++;
	}

}

void InicializaNodosEnNiveles (t_NodosEnNiveles *tablaNiveles, unsigned long int maxNiveles) {
	int i;
	(*tablaNiveles).maxNivel = maxNiveles;
	(*tablaNiveles).minNivel = 1;

	((*tablaNiveles).first)   = (unsigned long int *) calloc(maxNiveles, sizeof (unsigned long int));
	((*tablaNiveles).base) 	  = (unsigned long int *) calloc(maxNiveles, sizeof (unsigned long int));

	//fprintf(stderr,"\n Inicializa base [%ld] y first[%ld]",tablaNiveles->base[0],tablaNiveles->first[0]);
}

