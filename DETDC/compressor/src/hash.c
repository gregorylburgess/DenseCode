
/* DYNAMIC END-TAGGED DENSE CODE. -- 
A dynamic word-based byte oriented compressor for text files based on 
dynamic End-Tagged Dense Code.

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
 
#include "hash.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*-----------------------------------------------------------------
 Initilization of data structures used by the hashTable
 ---------------------------------------------------------------- */
unsigned long initialize_hash (unsigned long sizeVoc) {

	unsigned long i;

	NumElem = 0;

	//	PosInTH = new unsigned long[N];
	//printf("PosInTH initilized to support: %lu elements\n",N);

	TAM_HASH = OCUP_HASH * sizeVoc;
	TAM_HASH = NearestPrime(TAM_HASH);
	hash = (t_word *) malloc(TAM_HASH * sizeof(t_word));

	for(i = 0;i < TAM_HASH; i++)    {
	 	hash[i].word = NULL;
	 	hash[i].len=0;
	 	hash[i].weight = 0;
		//hash[i].size = 0;
		//hash[i].codeword = 0;
	}
	printf("\nHash Table initilized with: %lu elements\n",TAM_HASH);

	return TAM_HASH;
}


/*------------------------------------------------------------------
 Find the nearest prime number over n.
 ---------------------------------------------------------------- */
unsigned long NearestPrime(unsigned long n)
{
    long position;  /* the prime number being sought */
    long index;

    for (position = n; ; position++)
    {
        // checks if those values from 2 to $\sqrt{m}$ can be factors of $m$ */
        for (index = 2; index <= (long) sqrt((double) position) && position % index != 0; index++) ;

        if (position % index != 0)  /* No factors in that range, therefore a prime number was found */
        {
            break;
        }
    }
    return position;
}



/*-----------------------------------------------------------------------
 Computes a hash function over a string "aWord", it returns the position
 where "aWord" should go in the hash table if no collision ocurrs.
 ---------------------------------------------------------------------*/
/*unsigned long HashTable::hashFunction (const unsigned char *aWord, unsigned int len) {
  unsigned int  d;  		// a small prime number
  unsigned long Indice2 = 0;
  unsigned int  i;

  d=SMALL_PRIME;

  if (len==0) {return 0;}

  Indice2 += (aWord[0] * d) ;
  i=1;
  while (i< len){
	  Indice2 = ( (Indice2 * d) + (aWord[i]) ) % TAM_HASH ;
	  i++;
  }

  Indice2 = Indice2 % TAM_HASH;

//  if (Indice2 < 0) {       //possible overflow ?
//  		Indice2 +=TAM_HASH;
//  }

  return( (unsigned long) Indice2 );

}
*/

/* Author J. Zobel, April 2001.
   Permission to use this code is freely granted, provided that this
   statement is retained. */

/* Bitwise hash function.  Note that tsize does not have to be prime. */

//unsigned long hashFunction (const unsigned char *aWord, unsigned int len)
//{
//    char	c;
//    unsigned int h;
//
//
//    h = SEED;
//    //last=aWord+len;
//
//    for( ; ( c=*aWord )!='\0' ; aWord++ )
//    //for(c=*aWord; (aWord++)<last ;  )
//    {
//	//c=*aWord;
//	h ^= ( (h << 5) + c + (h >> 2) );
//    }
//    return((unsigned int)((h&0x7fffffff) % TAM_HASH));
//}

unsigned long hashFunction (const unsigned char *aWord, unsigned int len)
{
    char	c;
    register unsigned int h;
    register unsigned long last;
    last=((unsigned long) aWord) +len -1;


    h = SEED;
    //last=aWord+len;

    for( ; ((unsigned long) aWord <=last ) ; )
    //for(c=*aWord; (aWord++)<last ;  )
    {
    	c=*(aWord++);
		//c=*aWord;
		h ^= ( (h << 5) + c + (h >> 2) );
    }
    return((unsigned int)((h&0x7fffffff) % TAM_HASH));
}


/*-----------------------------------------------------------------------
  compares two strings
 ---------------------------------------------------------------------*/

/* Author J. Zobel, April 2001.
	http://www.seg.rmit.edu.au/code/zwh-ipl/
   Permission to use this code is freely granted, provided that this
   statement is retained. */

inline int strcomp1(const unsigned char *s1, const unsigned char *s2, unsigned long ws) {
    while( *s1 != '\0' && *s1 == *s2 )
/*    register unsigned long i;
    i=0;
    while( i < ws-1 && *s1 == *s2 )*/
    {
		//fprintf(stderr,"\nCaracteres [%c][%c] i[%ld] ws [%ld]",*s1,*s2,i,ws);
		s1++;
		s2++;
	//	i++;

    }
    //fprintf(stderr,"\nDevuelve [%d]",*s1-*s2);
    return( *s1-*s2 );
}

inline int strcomp(const unsigned char *s1, const unsigned char *s2, register unsigned long ws1, unsigned long ws2) {
	 
	 if (ws1 !=ws2) {
	 		return -1;
	 }
	 
	 {  register unsigned long iters;	    
	 	 iters=1;
	    while( iters<ws1 && *s1 == *s2 )
	    {
			s1++;
			s2++;
			iters++;
	    }
	    //fprintf(stderr,"\nDevuelve [%d]",*s1-*s2);
	    return( *s1-*s2 );
	 }
}

inline int strcomp3(const unsigned char *s1, const unsigned char *s2, unsigned long ws1, unsigned long ws2) {
	 
	 if (ws1 !=ws2) {
	 		return -1;
	 }
	 
	 {  register unsigned long iters;
	    register unsigned long end;
	    end = MIN(ws1,ws2);
	 	 iters=1;
	    while( iters<end && *s1 == *s2 )
	    {
			s1++;
			s2++;
			iters++;
	    }
	    //fprintf(stderr,"\nDevuelve [%d]",*s1-*s2);
	    return( *s1-*s2 );
	 }
}


/*-----------------------------------------------------------------------
 Insert a new word in a position of the hashTable (position previously computed)
 ---------------------------------------------------------------------*/
 unsigned long insertElement (const unsigned char *aWord, register unsigned long len, register unsigned long *addr) {
    //fprintf(stderr,"\n Entra en la función [%s], [%ld]",aWord, len);
	if(*addr == TAM_HASH)
	{
		printf("Not enough memory, vocabulary exceeds maximun size !\n");
		exit(1);
	}

    getMemoryBlock(_memMgr,( char **)&(hash[*addr].word),len+1);
	strncpy ((char *) hash[*addr].word, (char *)aWord, len);

	hash[*addr].word[len]='\0';
	hash[*addr].weight=0;
	NumElem++;

	//fprintf(stderr,"insertada palabra [%s] \n",hash[*addr].word);
	return *addr;
}



/*-----------------------------------------------------------------------
 Search for a word in the hash table and returns its position in the
 vocabulary. It returns the next "available" position in the vocabulary,
 if the word is not in the hash table. That is: a "0-node" position.
 It also returns -using attribute returnedAddr- the position where word
 was found (or where it should go if it was inserted in next "insert call".
 -----------------------------------------------------------------------*/
unsigned long search (const unsigned char *aWord, register unsigned len,
								 unsigned long *returnedAddr){

	register unsigned long addr, Saddr;
 
	addr = hashFunction(aWord,len);

	Saddr = addr;

	while((hash[addr].word  != NULL)&&((strcomp(hash[addr].word, aWord,  hash[addr].len, len)) != 0))  {
		
		//fprintf(stderr,"\nComprueba [%s], [%d]",hash[addr].word,strcomp(hash[addr].word, aWord, len));
		addr = ((addr + JUMP) %TAM_HASH);
	}

	*returnedAddr = addr;

	if(hash[addr].word  == NULL) {
		return NumElem;				//Word was not found
	}
	else {
		return hash[addr].posInVocab; //Word was found in this position in the vocabulary
	}

}

