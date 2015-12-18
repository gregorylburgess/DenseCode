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


/* Gives support to take time measures 
	Using either startTime or startRusageTime we take note of the initial "time"
	Using either stopTime or stopRusageTime we take note of the final "time"
	Difference between final and initial times give the execution time.
*/

#include "timer.h"
#include <sys/resource.h>
	long Microseginic;
	long Microsegfim;
	long SegInic;
	long SegFim;
	long UserTimeBeginning;
	long SysTimeBeginning;
	long UserTimeEnd;
	long SysTimeEnd;



/* Takes note of the initial time */	
void StartTime()
{
  struct timeval tp;
  struct timezone tzp;

  gettimeofday( &tp, &tzp );
  Microseginic = tp.tv_sec*1000000+tp.tv_usec;
  SegInic = tp.tv_sec;
}

/* Takes note of the final time and outputs the execution time */
void StopTime()
{
  struct timeval tp;
  struct timezone tzp;

  gettimeofday( &tp, &tzp );
  Microsegfim = tp.tv_sec*1000000+tp.tv_usec;
  SegFim = tp.tv_sec;

  printf( "Time in seconds: %lf\n", ((double) (Microsegfim-Microseginic)) / ((double) 1000000) );
  printf( "Time in seconds: %lf\n", (double) (SegFim - SegInic));
}

/* Takes note of the initial time */
void StartRusageTime()
{
  struct rusage Rusage;

  getrusage(RUSAGE_SELF, &Rusage);
  UserTimeBeginning = Rusage.ru_utime.tv_sec * 1000000 + Rusage.ru_utime.tv_usec;
  SysTimeBeginning = Rusage.ru_stime.tv_sec * 1000000 + Rusage.ru_stime.tv_usec;
}

/* Takes note of the final time and outputs the execution time to
   standard output and to a file. */
void StopRusageTime(char *fileName, char *note)
{
  struct rusage Rusage;

  FILE *fout;  //para sacar la lista de frecuencias ...

  getrusage(RUSAGE_SELF, &Rusage);
  UserTimeEnd = Rusage.ru_utime.tv_sec * 1000000 + Rusage.ru_utime.tv_usec;
  SysTimeEnd  = Rusage.ru_stime.tv_sec * 1000000 + Rusage.ru_stime.tv_usec;

  fprintf(stderr, "\n\tUser Time in seconds: %lf", ((double) (UserTimeEnd - UserTimeBeginning)) / ((double) 1000000) );
  fprintf(stderr, "\n\tSys  Time in seconds: %lf", ((double) (SysTimeEnd - SysTimeBeginning)) / ((double) 1000000) );


  if ((fout = fopen(fileName,"a")) == NULL)
	{
	  fprintf(stderr,"Error in opening file %s\n", fileName);
	  //exit(0);
	}

	fprintf(fout,"\nETDC=========================================");
	fprintf(fout,"\n** File compressed: %s  **",note);
	fprintf(fout,"\nTime (USER)--> %lf",((double) (UserTimeEnd - UserTimeBeginning)) / ((double) 1000000));
	fprintf(fout,"\nTime (SYS )--> %lf",((double) (SysTimeEnd - SysTimeBeginning)) / ((double) 1000000));
	fprintf(fout,"\n=========================================\n");

 fclose(fout);
}
