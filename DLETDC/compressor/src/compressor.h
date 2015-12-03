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
#include "compatibility.h"



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



	unsigned long *positionInTH;

	unsigned long *topBlock;       /* holds a the first word of each frequency */
	unsigned long zeroNode;		   /* zeroNode is the place where a new word will be inserted */
	unsigned long MAXFREQ;		   /* Maximum frequency of a word currently reached */

	unsigned long K;		   	   /* size of the codeword obtained by getCode */
	unsigned long CODEWORD;		   /* Codeword obtained by getCode */

	unsigned long C_NEW ;
	unsigned long C_SWAP ;
	unsigned long LEN_SCAPES; 	   /* the number of bytes of C_new and C_swap.*/

	//variables used in the "full process"
	
	unsigned long j;	 	 //stores the position in the vocabulary of the current parsed word
	unsigned long addrInTH;  //position in the hash table were a word is found.
	unsigned long zeroNode;	 /* zeroNode is the place where a new word will be inserted */


	//functions used.
	void initialize (unsigned long tamArq, unsigned long tamTop);
	void getCode(register unsigned long position);
	unsigned long fileSize (char *filename); 
	void mainCompressor (char* infile, char* outfile, unsigned long N_val, unsigned long N_val2);

