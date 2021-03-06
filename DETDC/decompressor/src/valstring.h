
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
 ValString: 
	Using an array to rapidly check if a character is an alpha-numeric character.
 ------------------------------------------------------------------------*/
 

#include <ctype.h>

#ifndef ValidCh

 #define ValidCh(ch)  (isalnum(ch)) /* Teste de validacao	  */   
 #define InvalidCh(ch) (!ValidCh(ch))

#endif


extern char  _Valid[256];
extern char  _Invalid[256];

void StartValid();

