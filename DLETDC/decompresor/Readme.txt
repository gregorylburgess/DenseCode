Decompressor of Dynamic Lightweight End-Tagged Dense Code (dletdc)

Permits to decompress a file compressed with dletdc.

** Syntax: DDLETDC <inputFile> <OutputFile> <[VOC SIZE]>

The parameter "VOC SIZE" is optional.  When set, the decompressor will use "explicitly" that value as the size of the vocabulary.

Notes: 
 
 - Two files "compressedFile" and "compressedFile.voc" should be in the same folder. That it, the file containing the compressed data, and other file that stores (as a long value) the size of the vobulary (in words).
   If the file "compressedFile.voc does not exist -> the decompressor will use a default value as the size of the vocabylary in order to allocate memory.
 
 - The decompressor creates 1 file "OutputFile" in which the plain text is writen. 
 
 - A file "timesDec" is also created after decompression. It logs decompression times (useful to perform several runs of the decompressor and then collect all the time measures).
  