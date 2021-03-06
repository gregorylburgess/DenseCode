
/* DYNAMIC END-TAGGED DENSE CODE. Decompressor -- 
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

#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <stdio.h>
#include "valstring.h"
#include "BufferedWriter.h"

#include <malloc.h>

#define byte char

/*
 * Definition of DataType Writer
 */

struct Wrt {
	int _previousAlfanumerical;
	unsigned char _SPACE[1];
	char *_fileName;
	BufferWrite bw;
} ;

typedef struct Wrt *Writer;

	
	Writer createWriter(char * fileName);
	void destroyWriter(Writer w);
	void writeWord (Writer w, unsigned char *aWord,unsigned int size);
