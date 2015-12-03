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

#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <malloc.h>
#include <stdio.h>

#include "MemoryManager.h"
#include "constants.h"

#include "BufferedReader.h"
#include "writer.h"         //#include "../IO/BufferedWriter.h"
#include "timer.h"


	#if 0
	static void swap(int *x,
			 int *y);
	#endif
	
	#if 0
	static void swap(int *x,
			 int *y)
	{
	  int temp;
	
	  temp = *x;
	  *x = *y;
	  *y = temp;
	  return;
	}
	#endif
	
	#define swap( x, y ) { int temp; temp=*(x); *(x)=*(y); *(y)=temp; }




//const unsigned long TABLABASE[6] = {0,0,128,16512,2113664,270549120};
const unsigned long TABLABASE[5] = {0,128,16512,2113664,270549120};




/*
 * Definition of Class Decompressor
 */



	//information of words
	struct Nword {
		unsigned char *word;
		unsigned int size;

	};

	typedef struct Nword t_word;

   unsigned long N;
   t_word *words;

	MemoryManager _memMgr;
	//	unsigned long TABLABASE[MAX_CODEWORD_SIZE +1];

	Writer outWriter;

	//variables used in the "full process"
	unsigned long j;	 	 //stores the position in the vocabulary of the current parsed word
	unsigned long nj;        //next j

	unsigned long zeroNode;		   /* zeroNode is the place where a new word will be inserted */




	void initialize (unsigned long tamArq);
	void insertElement (const unsigned char *aWord, register unsigned long len , register unsigned long position);

	void mainDecompressor (char* infile, char* outfile, unsigned long N_val);
	void mainDecompressorx (char* infile, char* outfile, unsigned long N_val) ;

