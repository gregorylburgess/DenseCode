
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

/*-----------------------------------------------------------------------
 File       : MemoryManager
 Function   : Reserves large blocks of memory and gives pointers to small
              portions of that block when requested.
              This improves performance since a unique "LARGE ALLOCATION"
              of memory is needed (a unique call to malloc).
              It is also responsible of freeing memory.
 Purpose    : Improve hash performance.
 ------------------------------------------------------------------------*/

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include <malloc.h>

#define byte char
#define LARGE_BLOCK_SIZE 1048576 // Size of the blocks of memory that will be allocated
#define MAX_BLOCKS 128				// Maximum number of blocks of size LARGE_BLOCK_SIZE that
											// can be allocated 

	/*
	 * Definition of structure MemoryManager
	 */
	struct sMem {
		byte *BLOCKS[MAX_BLOCKS]; 		//array of blocks of size LARGE_BLOCK_SIZE
		unsigned int  currentBlock;	//currentBlock in the array of blocks
		unsigned long remainderBytes; //number of bytes not yet assigned in BLOCKS[currentBlock]
		byte *availableByte;				//pointer to next byte not yet assigned 
	} ;
	
	typedef struct sMem *MemoryManager;
	
		 
	MemoryManager createMemoryManager(void);
	void destroyMemoryManager (MemoryManager mm);
	void getMemoryBlock (MemoryManager mm, byte **dst, const unsigned int size);
	void createNewMemoryBlock (MemoryManager mm);

