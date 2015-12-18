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
 
*/

#include <string.h>
#include <stdio.h>
#include "BufferedWriter.h"


/*************************************************
 * Construction/Destruction
 *************************************************/

 /*------------------------------------------------------------------
 Creates a new buffer 
 ------------------------------------------------------------------*/

  BufferWrite bufCreateWriter (char *fileName) {

	BufferWrite B = (BufferWrite) malloc (sizeof(struct BWriter));

 	B->_fileName = (char * )malloc (strlen(fileName)+1);
	strcpy(B->_fileName,fileName);
	//fprintf(stderr,"\n*** [%s] [%d]\n",B->_fileName, strlen(B->_fileName));

	B->size = WRITER_BUFFER_SIZE;
	B->posInBuffer=0;
	B->data = (byte *) malloc (B->size);
	return B;
  }



/*------------------------------------------------------------------
 Flushes all the data in the buffer and frees memory 
------------------------------------------------------------------*/
void destroyBW(BufferWrite bW) {
	
	flushBW(bW);
	free (bW->data);
	free(bW->_fileName);
	free(bW);
 }

/*************************************************
 * Other Methods
 *************************************************/

/*------------------------------------------------------------------
 Opens file for writting 
 ------------------------------------------------------------------*/

void openBW(BufferWrite bW) {

//	if ( (bW->outFile = open (bW->_fileName, O_WRONLY|O_CREAT, 0666)) ==-1)
	if ( (bW->outFile = fopen (bW->_fileName,"w")) == NULL)
	{
		fprintf(stderr,"\nError while opening file: %s\n",bW->_fileName);
		return;
	}
	fprintf(stderr,"File [%s] successfuly opened\n",bW->_fileName);
	
}


/*------------------------------------------------------------------
 Closes the file 
------------------------------------------------------------------*/
void closeBW(BufferWrite bW) {
	
	flushBW(bW);
	//close(bW->outFile);
	fclose(bW->outFile);
}


/*------------------------------------------------------------------
 Writes all the data of the buffer to disk, leaving the buffer empty
 ------------------------------------------------------------------*/

void flushBW(BufferWrite bW) {

	if (bW->posInBuffer >0) {
		size_t bread = fwrite(bW->data, sizeof(byte), bW->posInBuffer, bW->outFile);
		//int bread = write(bW->outFile,  bW->data,  bW->posInBuffer);

		bW->posInBuffer=0;
	}
}


/*------------------------------------------------------------------
 Adds n bytes to the buffer bW starting from the pointer orig.
 If the buffer becomes full, the whole buffer is output to disk.
------------------------------------------------------------------*/
long writeBW(BufferWrite bW, unsigned char *orig, long n) {

	byte *dst;
	register long i;

	if (bW->posInBuffer < WRITER_BUFFER_SIZE - n) {					
			dst = (byte *) bW->data + bW->posInBuffer; //bcopy(orig, &(bW->data[bW->posInBuffer]) , n);
			for (i=1;i<=n;i++) {
				*(dst++) = *(orig++); }
			bW->posInBuffer += n;
	}
	else {  //Outputs the data in the buffer to disk  (up to posInBuffer).
			//int bwriten= write(bW->outFile,  bW->data,  bW->posInBuffer);
			size_t bwriten= fwrite(bW->data, sizeof(byte), bW->posInBuffer, bW->outFile);
			//Copies the remaining byte to the begging of the "new buffer"
			dst = (byte *) bW->data;
			for (i=1;i<=n;i++) {*(dst++) = *(orig++); }
			bW->posInBuffer = n;
	}
}
