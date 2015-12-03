
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
 File       : Compatibility
 Function   : Permits to represent an array with up to 4 characters (a code)
                    inside a unique long number, by using a pointer to a long as a pointer to
	      a character (which needs some operations with the number.
 Purpose    : Improve performance. A long is manipulated faster than an array.
 ------------------------------------------------------------------------*/
 

#include <stdio.h>
#include "BufferedWriter.h"

// Uncomment the architecture used.

 #ifndef LITTLE_ENDIAN
 #define LITTLE_ENDIAN
 #endif

//	#ifndef  BIG_ENDIAN
//	#define BIG_ENDIAN
//	#endif

//	#ifndef  SOLARIS
//	#define SOLARIS
//	#endif

//	#ifndef  LINUX
//	#define LINUX
//	#endif


#define LINE_UP(x,numBytes) ((x>>(4-numBytes)*8))    //move "4-bytes" bytes to the right
						   						     //so we can write "bytes" bytes from &x

#ifdef LITTLE_ENDIAN

	#define LE_LONG32(x)  (((x&0xff000000)>>24)|((x&0x00ff0000)>>8)|((x&0x0000ff00)<<8)|((x&0x000000ff)<<24))

	 void writeCodeWord(BufferWrite bw, unsigned long *number, unsigned long *numBytes) {
		unsigned long tmp;
		tmp = LE_LONG32(*number);
		tmp = LINE_UP (tmp,*numBytes);
		//fprintf(stderr,"number x [%0x], number LE_LONG(x) [%0x]\n",(*number), tmp);
		writeBW(bw,(unsigned char *)&tmp,*numBytes);
		//	fprintf(stderr," \n***COMPATIBILITY******* COD [%0x]COD [%0x], K [%0x]", tmp,*number,*numBytes);
		//	fprintf(stderr,"little endian\n\n\n\n\n");
	}

#else

	#define LE_LONG32(x) x

	 void writeCodeWord(BufferWrite bw, unsigned long *number, unsigned long *numBytes) {
		unsigned long tmp;
		tmp = LE_LONG32(*number);
		tmp = LINE_UP (tmp,*numBytes);
		//fprintf(stderr,"number x [%0x], number LE_LONG(x) [%0x]\n",(*number), tmp);
		writeBW(bw,(unsigned char *) &tmp,*numBytes);
		//	fprintf(stderr," \n***COMPATIBILITY******* COD [%0x]COD [%0x], K [%0x]", tmp,*number,*numBytes);
		//	fprintf(stderr,"not little endian\n\n\n\n\n");
	}

#endif
