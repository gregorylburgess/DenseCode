
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

/*
  BufferedWriter includes buffering support for writting.

  Writes  will not work properly if writing more elements (N) than buffer size ( WRITER_BUFFER_SIZE)

*/


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <string.h>
#include <fcntl.h>

//#define WRITER_BUFFER_SIZE 65536
#define WRITER_BUFFER_SIZE 1048576
#define MAX_FILENAME_LENGTH 1000
#define byte char


struct BWriter {
	int outFile;
	char *_fileName;
	int size;  //size of buffer
	byte *data;
	unsigned long posInBuffer;
} ;

typedef struct BWriter *BufferWrite;


	//constructor & destructor
	BufferWrite bufCreateWriter (char *fileName);
	void destroyBW(BufferWrite bW);

	//another methods.

	void openBW(BufferWrite bW);
	void closeBW(BufferWrite bW);
	long writeBW(BufferWrite bW, unsigned char *dest, long n);

	//This method could be private, because it is only called when closing
	//outFile. I decided to make it public, for allowing flushing Buffer.
	void flushBW(BufferWrite bW);




