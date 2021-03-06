
/* Removes '\0' characters from a input file --

Parses a text file and computes the different words in it.
Then, it applies a filter by frequency of word and/or the lenght
of the words, in such a way that only the "filtered words" are
output to the target file.

This target file is useful since it contains a set of patterns that
can be later searched inside the input text file.

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
  

#include <stdio.h>

#include "BufferedReader.h"
#include "BufferedWriter.h"
#include <stdlib.h>

int main(int argc, char* argv[])
{

	unsigned char *letras;
	char letra;
	long num;

	BufferRead B;
	BufferWrite bw;
	int fIn; //file to read

	fprintf(stderr,"\n ***************************************************************");
	fprintf(stderr,"\n * Removes '\\0' characters from the source file                *");
	fprintf(stderr,"\n ***************************************************************\n");


	if (argc < 3) {
		fprintf(stderr," \n Wrong call to ClearZeroes\n");
		fprintf(stderr," Use:  %s  <in file>  <out file> \n",argv[0]);
		return 0;
	}

	bw=bufCreateWriter(argv[2]);
	openBW(bw);

	B = bufCreate(0);
	fIn = open (argv[1], O_RDONLY);
	if (fIn == -1) {
		fprintf(stderr,"Cannot read file %s","in");
		return;
	}

	bufSetFile (B,fIn);
	unsigned char z = 'z';
	{ byte *text,*top, *nextbuf,*pbeg,*pend;

		while ( !bufEmpty(B) ) {
			bufCurrent (B,&text,&top);
			pbeg = text; pend = top;

			while (pbeg <pend) {
				if (*pbeg) {
					writeBW(bw,(unsigned char*)(pbeg),1);
				}
				else {
					writeBW(bw,&z,1);
					writeBW(bw,&z,1);
					writeBW(bw,&z,1);
					writeBW(bw,&z,1);
				}
				pbeg++;
			}

			if (bufEof(B)) nextbuf = top;
			else nextbuf = pbeg;

			bufLoad (B, nextbuf);
		}
	}

	close (fIn);
	bufDestroy (B);

	closeBW(bw);

	return(0);
}

