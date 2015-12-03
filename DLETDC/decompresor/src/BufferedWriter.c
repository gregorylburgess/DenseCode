/* DYNAMIC LIGHTWEIGHT END-TAGGED DENSE CODE. -- 
A dynamic word-based byte oriented compressor for text files based on 
dynamic End-Tagged Dense Code.

N. Brisaboa, A. Fariña, G. Navarro, and J. Parama. Eficiently decodable and searchable natural language adaptive compression. In Proceedings of the 28th Annual International ACM SIGIR Conference on Research and Development in Information Retrieval (SIGIR-2005), Bahia-Brazil, August 15-19 2005. ACM Press.

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
BufferWrite bufCreateWriter (char *fileName)

  {BufferWrite B = (BufferWrite) malloc (sizeof(struct BWriter));

 	B->_fileName = (char * )malloc (strlen(fileName));
	strcpy(B->_fileName,fileName);

	B->size = WRITER_BUFFER_SIZE;
	B->data = (byte *) malloc (B->size);
	return B;
  }

void destroyBW(BufferWrite bW)
{
	flushBW(bW);
	free (bW->data);
 }


//////////////////////////////////////////////////////////////////////
// PUBLIC METHODS
//////////////////////////////////////////////////////////////////////

	/*
	 * Open file for reading
	 */
	void openBW(BufferWrite bW) {

		printf("Trying to open file: [%s] for writting \n",bW->_fileName);

//		if ((bW->outFile = fopen(bW->_fileName,"w")) == NULL)
		if ( (bW->outFile = open (bW->_fileName, O_WRONLY|O_CREAT, 0666)) ==-1)
		{
			//fprintf(stderr,"\nError while opening file: %s\n",bW->_fileName);
			//fprintf(stderr,"Program will exit now\n");
			return;
		}
		printf("File [%s] successfuly opened\n",bW->_fileName);

	}

	/*
	 * Close File
	 */
	void closeBW(BufferWrite bW) {
		flushBW(bW);
//		fclose(bW->outFile);
		close(bW->outFile);
	}

	/*
	 *  Vacía el Buffer de escritura a disco.
	 */
	void flushBW(BufferWrite bW) {
		if (bW->posInBuffer >0) {
			//			fwrite(bW->data, sizeof(byte), bW->posInBuffer, bW->outFile);
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
					//	bcopy(orig, &(bW->data[bW->posInBuffer]) , n);
				    dst = (byte *) bW->data + bW->posInBuffer;
 			        for (i=1;i<=n;i++) {*(dst++) = *(orig++); }
					bW->posInBuffer += n;
			}
			else {  //hay que volcar el buffer a disco (hasta posInBuffer)
					write(bW->outFile,  bW->data,  bW->posInBuffer);
				    //y copiar la palabra al comienzo del nuevo buffer;
					dst = (byte *) bW->data;
 			        for (i=1;i<=n;i++) {*(dst++) = *(orig++); }
					bW->posInBuffer = n;
			}
		}


//	/*
//	 * Reads a unique character from File.
//	 * Data is copied into dest.
//	 */
//		long writeBW(BufferWrite bW, unsigned char *orig, long n) {
//			long num;
//			long num2;
//
//			if (bW->posInBuffer < WRITER_BUFFER_SIZE) {
//
//				if ((bW->posInBuffer + n) <=WRITER_BUFFER_SIZE ) {
//					bcopy(orig, &(bW->data[bW->posInBuffer]),n);
//					bW->posInBuffer +=n;
//				}
//				else {
//					num = WRITER_BUFFER_SIZE - bW->posInBuffer;
//					bcopy(orig, &(bW->data[bW->posInBuffer]) , num );
//					fwrite (bW->data, 1, WRITER_BUFFER_SIZE, bW->outFile);
//
//					num2 = n - num;
//
//					bW->posInBuffer = 0;
//					bcopy(orig+num, &(bW->data[bW->posInBuffer]) , num2);
//					bW->posInBuffer += num2;
//				}
//			}
//			else {  //posInBuffer = WRITER_BUFFER_SIZE
//					fwrite (bW->data, 1, WRITER_BUFFER_SIZE, bW->outFile);
//					bW->posInBuffer=0;
//					bcopy(orig, &(bW->data[bW->posInBuffer]) , n );
//					bW->posInBuffer +=n;
//			}
//
//
//		}
