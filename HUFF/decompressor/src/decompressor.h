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

#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <malloc.h>
#include <stdio.h>

#include "MemoryManager.h"
#include "constants.h"

#include "BufferedReader.h"
#include "writer.h"        
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


	//information of words
	struct Nword {
		unsigned char *word;
		unsigned int size;

	};

	typedef struct Nword t_word;

   unsigned long N;
   t_word *words;

	MemoryManager _memMgr;

	Writer outWriter;

	//variables used in the "full process"

	struct NodosEnNiveles {
	   unsigned int maxNivel;
	   unsigned int minNivel;
	   unsigned long  *hojas;
	   unsigned long  *nohojas;
	   unsigned long  *first;
	   unsigned long  *base;
	
	} ;
	typedef struct NodosEnNiveles t_NodosEnNiveles;
		
	t_NodosEnNiveles tablaNodosEnNiveles;
	
	
	void initialize (unsigned long tamArq);
	void insertElement (const unsigned char *aWord, register unsigned long len , register unsigned long position);
	
	void mainDecompressor (char* infile, char* outfile);
	void InicializaNodosEnNiveles (t_NodosEnNiveles *tablaNiveles, unsigned long int maxNiveles);
	

