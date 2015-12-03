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
   
#define MAX_CODEWORD_SIZE 4
//#define MAX_SIZE_OF_WORD 8192
#define C_NEW 16777086
#define C_SWAP 16777087
#define MAX_SIZE_OF_WORD 32768

//#define N_value 1000000
#define N_value   10000000


//#define UNCOMPRESSED_VOC
//#define COMPRESSED_VOC
