/* Plain Huffman. -- 
A semistatic word-based byte oriented compressor for text files.

Programmed by Antonio Fari�a
Copyright (C) 2006  Nieves R. Brisaboa, Gonzalo Navarro, Antonio Fari�a and Jos� R. Param�
Author's contact: antonio.fari@gmail.com
WEB: http://vios.dc.fi.udc.es/codes

Programmed as a baseline for the paper: 

N. Brisaboa, A. Fari�a, G. Navarro, and J. Param�. Lightweight natural 
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

*/
  

   
/*
  BufferedWriter includes buffering support for writting.
 
  Writes  will not work properly if writing more elements (N) than buffer size ( WRITER_BUFFER_SIZE)
 
*/#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <string.h>
#include <fcntl.h>

#ifndef BUFFERED_WRITER
 	#define BUFFERED_WRITER

	#define WRITER_BUFFER_SIZE 1048576 /*size of the buffer */
	#define MAX_FILENAME_LENGTH 1000
	#define byte char


	struct BWriter {
		FILE *outFile;
		char *_fileName;
		int size;  //size of buffer
		byte *data;
		unsigned long posInBuffer;
	} ;

	typedef struct BWriter *BufferWrite;



	/*************************************************
	 * Construction/Destruction
	 *************************************************/
	/* Creates a new buffer */
	BufferWrite bufCreateWriter (char *fileName);

	/*Flushes all the data in the buffer and frees memory */
	void destroyBW(BufferWrite bW);

	/*************************************************
	 * Other Methods
	 *************************************************/
	 
	/* Opens file for writting */
	void openBW(BufferWrite bW);

	/* Closes the file */
	void closeBW(BufferWrite bW);

	/* Writes all the data of the buffer to disk, leaving the buffer empty  */
	void flushBW(BufferWrite bW);

	/* Adds n bytes to the buffer bW starting from the pointer orig.
		 If the buffer becomes full, the whole buffer is output to disk.
	*/
	long writeBW(BufferWrite bW, unsigned char *orig, long n);
	
 #endif
