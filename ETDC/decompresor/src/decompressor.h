/* End-Tagged Dense Code. -- 
A semistatic word-based byte oriented compressor for text files.

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

/*-----------------------------------------------------------------------
 File       : decompressor.h
 Function   : Main file of the decompressor of ETDC
 ------------------------------------------------------------------------*/

#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <malloc.h>
#include <stdio.h>

#include "MemoryManager.h"
#include "constants.h"

#include "BufferedReader.h"
#include "writer.h"
#include "timer.h"
#include "huff.h"

	const unsigned long TABLABASE[5] = {0,128,16512,2113664,270549120};

	//Structure of the vocabulary
	struct Nword {
		unsigned char *word;
		unsigned int size;
	};
	typedef struct Nword t_word;

	//Variables used.
	unsigned long N;
	t_word *words;

	MemoryManager _memMgr;
	Writer outWriter;

	unsigned long zeroNode;		/* zeroNode is the place where a new word will be inserted */

#ifdef COMPRESSED_VOC
	unsigned char *zoneWords;
#endif

	void initialize (unsigned long sizeVoc);
	void insertElement (const unsigned char *aWord, register unsigned long len , register unsigned long position);
	void mainDecompressorOLD (char* infile, char* outfile, unsigned long N_val);
	void mainDecompressor (char* infile, char* outfile);
	int  main(int argc, char* argv[]);
