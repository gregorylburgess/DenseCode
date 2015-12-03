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
 File       : MemoryManager
 Function   : Reserves large blocks of memory and gives pointers to small
              portions of that block when requested.
              This improves performance since a unique "LARGE ALLOCATION"
              of memory is needed (a unique call to malloc).
              It is also responsible of freeing memory.
 Purpose    : Improve hash performance.
 ------------------------------------------------------------------------*/

 #include "MemoryManager.h"


/*------------------------------------------------------------------
 Creates a Memory Manager
 ------------------------------------------------------------------ */
MemoryManager createMemoryManager(void) {
	MemoryManager mm;
	mm = (MemoryManager) malloc (sizeof(struct sMem));
	mm->currentBlock=0;
	createNewMemoryBlock(mm);
	return mm;
}

/*------------------------------------------------------------------
 Frees the allocated memory
 ------------------------------------------------------------------ */
void destroyMemoryManager (MemoryManager mm){
	register int i;
	for (i=0; i<=mm->currentBlock;i++) free(mm->BLOCKS[i]);
	//fprintf(stderr,"\n...Freed %ld MegaBytes... RAM\n", mm->currentBlock+1);
	free(mm);
}

/*------------------------------------------------------------------
 Allocates a new memory block of size "LARGE_BLOCK_SIZE" and adds it to
 vector BLOCKS
 ------------------------------------------------------------------ */
void createNewMemoryBlock (MemoryManager mm) {
	mm->BLOCKS[mm->currentBlock] = (byte *) malloc (LARGE_BLOCK_SIZE);

	if (mm->BLOCKS[mm->currentBlock] == NULL) {
		fprintf(stderr, "\n ERROR...\nUnable to allocate enough memory. File too long\n");
		exit(0);
	}

	mm->remainderBytes = LARGE_BLOCK_SIZE;
	mm->availableByte	= mm->BLOCKS[mm->currentBlock];  //points to the begining of the block
}

/*------------------------------------------------------------------
 getBlock method
 returns a pointer to a free block of memory of size "size"
 ------------------------------------------------------------------ */
void getMemoryBlock (MemoryManager mm, byte **dst, const unsigned int size) {
	if (mm->remainderBytes < size)	{
		mm->currentBlock++;
		createNewMemoryBlock(mm);
	}

	*dst = mm->availableByte;			//points to a free size-block
	mm->remainderBytes -= (size);
	mm->availableByte  += (size);
}


/*------------------------------------------------------------------
 main, to make unit proofs
 ------------------------------------------------------------------ */
/*
int main(int argc, char* argv[])
{  byte *word;
	unsigned int size;
int i;
	MemoryManager memMgr;
	memMgr=createMemoryManager();

	size = 100;
	getMemoryBlock(memMgr,&word,size);

	fprintf(stderr,"getBlock called\n");
	
	strcpy((char *)(word), "01234567890123456789012345678901234567890123456789");
	word[50]='\0';

	fprintf(stderr,"\n%s",word);


	destroyMemoryManager(memMgr);

}
*/
