/* Removing zero-characters from a file. -- 

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
  
   
/*
BufferedReader is a modification of the buffering routines found in
"Nrgrep -- a fast and flexible pattern matching tool.
Copyright (C) 2000 Gonzalo Navarro"
http://www.dcc.uchile.cl/~gnavarro/software/sindex.html
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <string.h>
#include <fcntl.h>


#ifndef BUFFERINCLUDED
#define BUFFERINCLUDED

#define BUFFER_SIZE 1048576

#define byte char
	/* Buffer management */

typedef struct sBuffer
  { int size;		/* size of this buffer */
    int file;		/* associated file */
    int fpos;		/* pos of current buffer in file */
    byte *data;	/* current text */
    int dsize;		/* size of current text */
    int minDataInBuffer;
  } *BufferRead;

	/* Creates a new buffer */

BufferRead bufCreate (unsigned int minDataInBuffer);

	/* Terminates use of buffer B */

void bufDestroy (BufferRead B);

	/* Assigns a file to B */

void bufSetFile (BufferRead B, int file);

        /* Gets buffer size of B */

int bufSizer (BufferRead B);

	/* Reads a new buffer starting at position pnext */

void bufLoad (BufferRead B, byte *pnext);

  	/* Tells whether there is no more data in B + its file */

int bufEmpty (BufferRead B);

  	/* Tells whether there is no more data to read in B's file */

int bufEof (BufferRead B);

	/* Tells the file position of the current buffer */

int bufTextPos (BufferRead B);

	/* Gives the memory area of the current buffer */
	/* One text position before the given buffer can be touched */

void bufCurrent (BufferRead B, byte **base, byte **top);

#endif
