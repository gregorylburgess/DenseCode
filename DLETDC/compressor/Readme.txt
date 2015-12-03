Compressor of Dynamic Lightweight End-Tagged Dense Code (dletdc)

Permits to compress a text file with dletdc
The text file cannot contain the character '\0'. Otherwise compression will work wrong.


** Syntax: CDLETDC <inputFile> <compressedFile> <[VOC SIZE]> <[MAX FREQ]>


The parameterS "VOC SIZE" and "MAX FREQ" are optional.  If skipped, the compressor uses Heap's law to stimate the size of the vocabulary "VOC SIZE" (number of different words in the inputFile) and a stimation of the maximum frequency value "MAX FREQ" of all words in the compression process. If "VOC SIZE" and "MAX FREQ" are given (they are known by the user), the compressor uses "explicitly" those values instead of the stimated values.

The compressor creates 2 files: 
	- compressedFile: the file containing the compressed data
	- compressedFile.voc: A file containing a long value that indicates the size of the vocabulary at the end of the compression process. This value can be used by the decompressor later to allocate only the amount of memory that the vocabulary needs (in other case, a higher value will be used.


Notes: 
  
 - A file "timesCDLETDC" is also created after compression. It logs compression times (useful to perform several runs of the compressor and then collect all the time measures).
 
 
