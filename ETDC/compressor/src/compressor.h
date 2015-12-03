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
#include "hash.h"
#include "BufferedWriter.h"
#include "compatibility.h"
#include "timer.h"
#include "huff.h"
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

#define MAX 256


	unsigned int *positionInTH;

    unsigned int addrInTH;		//the value return by calls to Search(hashTable)
	unsigned int zeroNode;		//The number of words that have been added to  the vocabulary.

	void initialize (unsigned int sizeVoc);

	void mainCompressor (char* infile, char* outfile, unsigned int N_val);
	void GeneraCodigosETDC (unsigned int v);
    void bin(unsigned int n);

