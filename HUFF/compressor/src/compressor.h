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
 File    : compressor
 Function  : Main file of the compressor
 ------------------------------------------------------------------------*/

#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <malloc.h>
#include <stdio.h>

#include "constants.h"
#include "valstring.h"
#include "BufferedReader.h"
#include "timer.h"
#include "hash.h"
#include "BufferedWriter.h"


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

	//Thanks to Edleno Silva for these macros!!
	#define MAX 256

	#define BASE_NUM 256
	#define DESLOC(c) (c>> 8)
	#define MULT(c) ((c<<3))
	#define LE_LONG32(x)  \
	(((x&0xff000000)>>24)|((x&0x00ff0000)>>8)|((x&0x0000ff00)<<8)|((x&0x000000ff)<<24))	
	#define LINE_UP(x,numBytes) ((x>>(4-numBytes)*8))

	unsigned long *positionInTH;

	int _previousAlfanumerical;

    unsigned long addrInTH;
	unsigned long zeroNode;   /* zeroNode is the place where a new word will be inserted */

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
	
	void mainCompressor (char* infile, char* outfile, unsigned long N_val);
	void GeneraCodigosSCNumero (unsigned long int v, int s);
	void CreateTables(unsigned long int numPalabras,
					 t_NodosEnNiveles *TN);
	void calculate_minimum_redundancy(unsigned long n,
					 		  t_NodosEnNiveles *tablaNodosEnNiveles);
	
	void InicializaNodosEnNiveles (t_NodosEnNiveles *tablaNiveles, unsigned long int maxNiveles);
	int comparaFrecListaAsc(const void *a, const void *b);
