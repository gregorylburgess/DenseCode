/* DYNAMIC LIGHTWEIGHT END-TAGGED DENSE CODE. -- 
A dynamic word-based byte oriented compressor for text files based on 
dynamic End-Tagged Dense Code.

N. Brisaboa, A. Fariña, G. Navarro, and J. Parama. Eficiently decodable and searchable natural language adaptive compression. In Proceedings of the 28th Annual International ACM SIGIR Conference on Research and Development in Information Retrieval (SIGIR-2005), Bahia-Brazil, August 15-19 2005. ACM Press.

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

/* Gives support to take time measures 
	Using either startTime or startRusageTime we take note of the initial "time"
	Using either stopTime or stopRusageTime we take note of the final "time"
	Difference between final and initial times give the execution time.
*/

#include "timer.h"
#include <sys/resource.h>
   long Microseginic, 
     Microsegfim,
     SegInic,
     SegFim,
     UserTimeBeginning,
     SysTimeBeginning,
     UserTimeEnd,
     SysTimeEnd;



void StartTime()
{
  struct timeval tp;
  struct timezone tzp;

  gettimeofday( &tp, &tzp );
  Microseginic = tp.tv_sec*1000000+tp.tv_usec;
  SegInic = tp.tv_sec;
}

void StopTime()
{
  struct timeval tp;
  struct timezone tzp;

  gettimeofday( &tp, &tzp );
  Microsegfim = tp.tv_sec*1000000+tp.tv_usec;
  SegFim = tp.tv_sec;

  printf( "Time in seconds: %lf\n", ((double) (Microsegfim-Microseginic)) / ((double) 1000000) );
  printf( "Time in seconds 2: %lf\n", (double) (SegFim - SegInic));
}

void StartRusageTime()
{
  struct rusage Rusage;

  getrusage(RUSAGE_SELF, &Rusage);
  UserTimeBeginning = Rusage.ru_utime.tv_sec * 1000000 + Rusage.ru_utime.tv_usec;
  SysTimeBeginning = Rusage.ru_stime.tv_sec * 1000000 + Rusage.ru_stime.tv_usec;
}

void StopRusageTime()
{
  struct rusage Rusage;

  getrusage(RUSAGE_SELF, &Rusage);
  UserTimeEnd = Rusage.ru_utime.tv_sec * 1000000 + Rusage.ru_utime.tv_usec;
  SysTimeEnd = Rusage.ru_stime.tv_sec * 1000000 + Rusage.ru_stime.tv_usec;

  printf( "User Time in seconds: %lf\n", ((double) (UserTimeEnd - UserTimeBeginning)) / ((double) 1000000) );
  printf( "Sys  Time in seconds: %lf\n", ((double) (SysTimeEnd - SysTimeBeginning)) / ((double) 1000000) );
}

void StopRusageTimeFile(char *fileName, char *nota)
{
  struct rusage Rusage;

  FILE *fout;  //para sacar la lista de frecuencias ...

  getrusage(RUSAGE_SELF, &Rusage);
  UserTimeEnd = Rusage.ru_utime.tv_sec * 1000000 + Rusage.ru_utime.tv_usec;
  SysTimeEnd = Rusage.ru_stime.tv_sec * 1000000 + Rusage.ru_stime.tv_usec;

  printf( "User Time in seconds: %lf\n", ((double) (UserTimeEnd - UserTimeBeginning)) / ((double) 1000000) );
  printf( "Sys  Time in seconds: %lf\n", ((double) (SysTimeEnd - SysTimeBeginning)) / ((double) 1000000) );


  if ((fout = fopen(fileName,"a")) == NULL)
	{
	  fprintf(stderr,"Error en la apertura del fichero %s\n", fileName);
	  //exit(0);
	}


	fprintf(fout,"\n\n=========================================\n");
	fprintf(fout,"** Corpus: %s **\n",nota);
	fprintf(fout,"Tiempo (USER)--> %lf\n",((double) (UserTimeEnd - UserTimeBeginning)) / ((double) 1000000));
	fprintf(fout,"Tiempo (SYS )--> %lf\n",((double) (SysTimeEnd - SysTimeBeginning)) / ((double) 1000000));
	fprintf(fout,"\n=========================================\n");


}
