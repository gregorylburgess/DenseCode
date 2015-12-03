
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

#define MAX_CODEWORD_SIZE 4

//#define MAX_SIZE_OF_WORD 8192
#define MAX_SIZE_OF_WORD 32768

//#define S_value 128
//#define C_value 128

#define INITIAL_TOP_FREQ 10000000  //freq of the most frequent word
#define MAX_LEN_VOCABULARY 2000000 // up to 2x10^6 words

#define VERSION_GETCODEANDWRITE   //defines how codewords are writen to a "output buffer"
								  //comment this line to use the old "slower way".

