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
 Hash: Definition of HashTable class (Linear Hash)
 ------------------------------------------------------------------------*/
 
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <malloc.h>

#include "MemoryManager.h"

#define JUMP 101  		 //jump done when a collision appears
#define OCUP_HASH 2	 	 //index of occupation of the hash table
#define SMALL_PRIME 1009 // a small prime number, used to compute a hash function
#define SEED	1159241
/* Type definitions */

#define MIN(a,b) (a < b) ? a : b

struct Nword {
	  unsigned char *word;
	  unsigned long len;
	  unsigned long weight;
	  unsigned long posInVocab;
	  
	  unsigned long codeword;
	  unsigned long size;
};

typedef struct Nword t_word;

// private:

	MemoryManager _memMgr; 	  /* Holds dynamic memory reserve for words. */

	unsigned long TAM_HASH;   /* # entries in the hash table    */
	unsigned long NumElem;    /* # elements already added to the hash table*/

	unsigned long NearestPrime(unsigned long n);
	unsigned long hashFunction (const unsigned char *aWord, unsigned int len);


// public:
	t_word  *hash;     		  /* holds a hashTable of words  */
	
	unsigned long initialize_hash  (unsigned long tamArq);
	unsigned long insertElement (const unsigned char *aWord, register unsigned long len,
										register unsigned long *addr);
	unsigned long search (const unsigned char *aWord, register unsigned len,
									unsigned long *returnedAddr);



