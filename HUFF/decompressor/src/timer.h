/* Plain Huffman. -- 
A semistatic word-based byte oriented compressor for text files.

Programmed by Antonio Fariña
Copyright (C) 2006  Nieves R. Brisaboa, Gonzalo Navarro, Antonio Fariña and José R. Paramá
Author's contact: antonio.fari@gmail.com
WEB: http://vios.dc.fi.udc.es/codes

Programmed as a baseline for the paper: 

N. Brisaboa, A. Fariña, G. Navarro, and J. Paramá. Lightweight natural 
language text compression. Information Retrieval. 
Information Retrieval(10), pp. 1-33, 2007. (online) 
(doi:10.1007/s10791-006-9001-9)

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


/* Gives support to perform  measures of time*/


#ifndef _TIMER_H
#define TIMER_H

#include <stdio.h>
#include <malloc.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

	/* Takes note of the initial time */	
	void StartTime();
	/* Takes note of the final time and outputs the execution time */
	void StopTime();
	/* Takes note of the initial time */
	void StartRusageTime();
	/* Takes note of the final time and outputs the execution time to
	   standard output and to a file. */
	void StopRusageTime(char *fileName, char *note);

#endif
