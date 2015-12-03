Compressor program of End-Tagged Dense Code (ETDC)

Permits to compress a text file with etdc.
The text file cannot contain the character '\0'. Otherwise compression will work in a wrong way.

** Syntax: CETDC <inputFile> <compressedFile> <[VOC SIZE]>

The parameter "VOC SIZE" is optional. If skipped, the compressor uses Heap's law to stimate the size of the vocabulary (number of different words in the inputFile). If "VOC SIZE" is given (it is known by the user), the compressor uses "explicitly" that value instead of the stimated value.


Notes: 
 
 - Two files "compressedFile" and "compressedFile.voc" are output.
 - A file "timesComp" is also created after compression. It logs compression times (useful to perform several runs of the compressor and then collect all the time measures).

 - Additionally, the compressor can be set such that the vocabulary is compressed with char-oriented huffman. To achieve this, you should compile with: "make compressorCOMvoc", and a exe file called CETDCcompVOC will be generated. CETDCcompVOC generates 2 files (instead of just one .voc file) with extensions ".cvoc" and ".voc.huff" respectively.
 
