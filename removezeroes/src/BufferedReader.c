/* Removing zero-characters from a file. -- 

Programmed by Antonio Fari�a
Copyright (C) 2006  Nieves R. Brisaboa, Gonzalo Navarro, Antonio Fari�a and Jos� R. Param�
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
  
   
/*
BufferedReader is a modification of the buffering routines found in
"Nrgrep -- a fast and flexible pattern matching tool.
Copyright (C) 2000 Gonzalo Navarro"
http://www.dcc.uchile.cl/~gnavarro/software/sindex.html
*/

/* Buffer management */

#include "BufferedReader.h"


BufferRead bufCreate (unsigned int minDataInBuffer)

	/* Creates a new buffer */

  { BufferRead B = (BufferRead) malloc (sizeof(struct sBuffer));
    B->size = BUFFER_SIZE;
    B->data = (byte *) malloc (1+B->size);
    B->data++;
    B->dsize = 0;
    B->minDataInBuffer=minDataInBuffer;
    return B;
  }

void bufDestroy (BufferRead B)

	/* Terminates use of buffer B */

  { free (B->data-1);
    free (B);
  }

void bufSetFile (BufferRead B, int file)

	/* Sets the file for buffer B */

  { B->file = file;
    B->fpos = 0;
    B->dsize = read (B->file,B->data,B->size);
  }

int bufSize (BufferRead B)

	/* Gets buffer size of B */

  { return B->size;
  }

void bufLoad (BufferRead B, byte *pnext)

	/* Reads a new buffer starting at position next */

  { register byte *src = pnext;
    register byte *dst = B->data;
    register int mov = B->dsize - (src - dst);

	/* move unused part to the beginning (memcpy is not portable) */
    B->dsize = mov;
    while (mov--) *dst++ = *src++;
    mov = B->size - B->dsize;
    B->dsize += read (B->file, dst, mov);
    B->fpos += mov;
  }

int bufEmpty (BufferRead B)

  	/* Tells whether there is no more data in B + its file */

  { return B->dsize == 0;
  }

int bufEof (BufferRead B)

  	/* Tells whether there is no more data to read in B's file */

  { return B->dsize < B->size;
  }

int bufTextPos (BufferRead B)

	/* Tells the file position of the current buffer */

  { return B->fpos;
  }

void bufCurrent (BufferRead B, byte **base, byte **top)

	/* Gives the memory area of the current buffer */

  { *base = B->data;
    if (!bufEof(B)){
   		*top = B->data + B->dsize - B->minDataInBuffer;
    }
	 else {
		*top = B->data + B->dsize;
	}

  }

