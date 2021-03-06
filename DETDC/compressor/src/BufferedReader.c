
/* DYNAMIC END-TAGGED DENSE CODE. -- 
A dynamic word-based byte oriented compressor for text files based on 
dynamic End-Tagged Dense Code.

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

   

#include "BufferedReader.h"

/*------------------------------------------------------------------
 Creates a new buffer 
 ------------------------------------------------------------------*/
BufferRead bufCreate (unsigned int minDataInBuffer)
{
	BufferRead B = (BufferRead) malloc (sizeof(struct sBuffer));
	B->size = BUFFER_SIZE;
	B->data = (byte *) malloc (1+B->size);
	B->data++;
	B->dsize = 0;
	B->minDataInBuffer=minDataInBuffer;
	return B;
}

  
/*------------------------------------------------------------------
 Terminates use of buffer B 
 ------------------------------------------------------------------*/
void bufDestroy (BufferRead B)
{
	free (B->data-1);
	free (B);
} 
  
/*------------------------------------------------------------------
 Sets the file for buffer B 
------------------------------------------------------------------*/
void bufSetFile (BufferRead B, int file)
{
	B->file = file;
	B->fpos = 0;
	B->dsize = read (B->file,B->data,B->size);
}
  
/*------------------------------------------------------------------
 Gets buffer size of B 
------------------------------------------------------------------*/
int bufSize (BufferRead B)
{
	return B->size;
}

/*------------------------------------------------------------------
 Reads a new buffer starting at position next 
 ------------------------------------------------------------------*/
void bufLoad (BufferRead B, byte *pnext)
{ 
	register byte *src = pnext;
	register byte *dst = B->data;
	register int mov = B->dsize - (src - dst);

	/* move unused part to the beginning (memcpy is not portable) */
	B->dsize = mov;

	while (mov--) *dst++ = *src++;
	mov = B->size - B->dsize;
	B->dsize += read (B->file, dst, mov);
	B->fpos += mov;

	////fprintf(stderr,"\n  %ld bytes remain in  buffer ",B->dsize);
}
  
/*------------------------------------------------------------------
 Tells whether there is no more data in B + its file 
 ------------------------------------------------------------------*/
int bufEmpty (BufferRead B)
{
	return B->dsize == 0;
}

/*------------------------------------------------------------------
 Tells whether there is no more data to read in B's file 
------------------------------------------------------------------*/
int bufEof (BufferRead B)
{ 
	return B->dsize < B->size;
}

/*------------------------------------------------------------------
 Tells the file position of the current buffer 
------------------------------------------------------------------*/
int bufTextPos (BufferRead B)
{ 
	return B->fpos;
}

/*------------------------------------------------------------------
 Gives the area of memory of the current buffer 
 One text position before the given buffer can be touched 
------------------------------------------------------------------*/
void bufCurrent (BufferRead B, byte **base, byte **top)
{ 
	*base = B->data;
    if (!bufEof(B)){
    	*top = B->data + B->dsize - B->minDataInBuffer;
    }
	 else {
		*top = B->data + B->dsize;
	}

}



/*------------------------------------------------------------------
 ------------------------------------------------------------------*/
/*
EXAMPLE ....
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
	printf("Read bytes %d\n", B->fpos-1);
	close (fIn);
	bufDestroy (B);

}
*/

