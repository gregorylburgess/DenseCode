
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



#include  "writer.h"


/*------------------------------------------------------------------
 Initialize method
 ------------------------------------------------------------------ */

Writer createWriter (char *fileName){

   Writer w = (Writer) malloc (sizeof(struct Wrt));

	w->_fileName = (byte *) malloc( strlen(fileName) +1 );
	strcpy(w->_fileName,fileName);

	w->bw = bufCreateWriter(w->_fileName);
   openBW(w->bw);


	w->_previousAlfanumerical = 0;
	strncpy((char *)w->_SPACE," ",1);
	StartValid();
	return w;
}

/*------------------------------------------------------------------
 Destructor method
 ------------------------------------------------------------------ */
void destroyWriter(Writer w) {
	free (w->_fileName);
	closeBW(w->bw);
	destroyBW(w->bw);
}



/*------------------------------------------------------------------
 writeWord method
 write a Word to the output file. It also sets "_previousAlphanumerial".
  ------------------------------------------------------------------ */
void writeWord (Writer w, unsigned char *aWord,unsigned int size){
	////fprintf(stderr,"\n ===== CALLING TO WRITER.WRITEWORD(%s)",aWord);

	if (_Valid[(unsigned char) *aWord] ) {
		if (w->_previousAlfanumerical) {
			writeBW (w->bw,w->_SPACE,1);
		}
		w->_previousAlfanumerical = 1;
	}
	else {w->_previousAlfanumerical = 0;
	}

	writeBW (w->bw,aWord, size);

}


/*------------------------------------------------------------------
 main, to make unit proofs
 ------------------------------------------------------------------ */
/*
int main(int argc, char* argv[])
{
	char fileName[20];
	Writer writer;
	char palabra [50];
	unsigned int size;
	unsigned int type;

	writer = createWriter ("out");

	printf ( "ENTRO EN NEXTWORD \n");

   sprintf(palabra,"exemplo de palabra");
   size=strlen(palabra);

	writeWord(writer,(unsigned char *)palabra, size);
	destroyWriter(writer);
}

*/
