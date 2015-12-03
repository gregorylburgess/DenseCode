
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
    ////fprintf(stderr,"\n");

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
    ////fprintf(stderr,"\n BuffLoad: moviendo %ld bytes en buffer de lectura",mov);
    while (mov--) *dst++ = *src++;
    mov = B->size - B->dsize;
    B->dsize += read (B->file, dst, mov);
    B->fpos += mov;
    ////fprintf(stderr,"\n quedan %ld bytes en buffer de lectura",B->dsize);
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
		    ////fprintf(stderr,"\n dsize = [%ld], top = [%ld]",B->dsize, B->dsize - B->minDataInBuffer);
    }
	 else {*top = B->data + B->dsize;
		    ////fprintf(stderr,"\n dsize = [%ld], top = [%ld]",B->dsize, B->dsize);
	}

  }

/*
int main(int argc, char* argv[])
{
int fIn; //file to read
BufferRead B;

		B = bufCreate(10);
		fIn = open ("in", O_RDONLY);
		if (fIn == -1)
			////fprintf(stderr,"Cannot read file %s","in");

		bufSetFile (B,fIn);

		 { byte *text,*top, *nextbuf,*pbeg,*pend;
			while ( !bufEmpty(B) ) {
				bufCurrent (B,&text,&top);
				pbeg = text; pend = top;

					while (pbeg <pend) {
						////fprintf(stderr,"[%c]",*(pbeg++));
					}

					if (bufEof(B)) nextbuf = top;
					else nextbuf = pbeg;

					bufLoad (B, nextbuf);
			}
		}
	printf("Leidos %d\n", B->fpos-1);
	close (fIn);
	bufDestroy (B);

}
*/

