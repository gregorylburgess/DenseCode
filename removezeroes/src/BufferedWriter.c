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
  BufferedWriter includes buffering support for writting.
 
  Writes  will not work properly if writing more elements (N) than buffer size ( WRITER_BUFFER_SIZE)
 
*/

#include <string.h>
#include <stdio.h>
#include "BufferedWriter.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/* Creates a new buffer */
	BufferWrite bufCreateWriter (char *fileName){
		BufferWrite B = (BufferWrite) malloc (sizeof(struct BWriter));

		B->_fileName = (char * )malloc (strlen(fileName));
		strcpy(B->_fileName,fileName);

		B->size = WRITER_BUFFER_SIZE;
		B->data = (byte *) malloc (B->size);
		return B;
	}

/* Destroys the buffer */
	void destroyBW(BufferWrite bW)
	{
		flushBW(bW);
		free (bW->data);
	 }


//////////////////////////////////////////////////////////////////////
// PUBLIC METHODS
//////////////////////////////////////////////////////////////////////

/*
 * Opens file for reading
 */
	void openBW(BufferWrite bW) {

		printf("Trying to open file: [%s] for writting \n",bW->_fileName);

		if ( (bW->outFile = open (bW->_fileName, O_WRONLY|O_CREAT, 0666)) ==-1)
		{
			fprintf(stderr,"\nError while opening file: %s\n",bW->_fileName);
			fprintf(stderr,"Program will exit now\n");
			return;
		}
		printf("File [%s] successfuly opened\n",bW->_fileName);

	}

/*
 * Closes File
 */
	void closeBW(BufferWrite bW) {
		flushBW(bW);
		close(bW->outFile);
	}

/*
 *  Copies all the data in the Buffer to disk.
 */
	void flushBW(BufferWrite bW) {
		if (bW->posInBuffer >0) {
			write(bW->outFile,  bW->data,  bW->posInBuffer);

			bW->posInBuffer=0;
		}
	}


/*
 * Write n elements to File.
 * Data is copied into dest.
 */
	long writeBW(BufferWrite bW, unsigned char *orig, long n) {
		byte *dst;
		register int i;
		if (bW->posInBuffer < WRITER_BUFFER_SIZE - n) {
				dst = (byte *) bW->data + bW->posInBuffer;
				for (i=1;i<=n;i++) {*(dst++) = *(orig++); }
				bW->posInBuffer += n;
		}
		else {  //writes the buffer to disk (up to posInBuffer)
				write(bW->outFile,  bW->data,  bW->posInBuffer);
				//and moves the next piece of data to the beginning
				//of the next buffer;
				dst = (byte *) bW->data;
				for (i=1;i<=n;i++) {*(dst++) = *(orig++); }
				bW->posInBuffer = n;
		}

	}


