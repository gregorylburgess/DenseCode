#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <malloc.h>


#define LE_LONG32(x)  \
(((x&0xff000000)>>24)|((x&0x00ff0000)>>8)|((x&0x0000ff00)<<8)|((x&0x000000ff)<<24))



int main (int argc, char *argv[])
{
unsigned long tmp;
int i;
unsigned char *p;

  FILE *fichero;
  int mas;
  unsigned char car;

long long largo;

  fprintf(stderr,"\n TAMAÑO%d", sizeof (largo));

  if ((fichero=fopen(argv[1],"r"))==NULL)
    {
      fprintf(stderr,
              "Error en la apertura del fichero %s\n",
              argv[1]);
      exit(0);
    }


i=0;
  while (mas = fread(&car, sizeof(unsigned char),1,fichero)) {
        printf("[%d]",car);

        if (i++ ==100) break;
  }




}

