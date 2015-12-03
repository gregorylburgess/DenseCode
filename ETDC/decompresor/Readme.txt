Decompressor of End-Tagged Dense Code (ETDC)

Permits to decompress a file compressed with etdc.


Syntax: DETDC <compressedFile> <OutputFile>


Notes: 
 
 - Two files "compressedFile" and "compressedFile.voc" should be in the same folder, otherwise decompression will fail.
 
 - A file "timesDec" is also created after decompression. It logs decompression times (useful to perform several runs of the decompressor and then collect all the time measures).
 
 - Additionally, the decompressor can be set such that it permits to manage a vocabulary compressed with char-oriented huffman. To achieve this, you should compile with: "make decompressorCOMvoc", and an exe file called DETDC_compVOC will be generated. DETDC_compVOC proceses the 2 files for the vocabulary (with extensions ".cvoc" and ".voc.huff" respectively) and recovers the model needed for decompression.
 
 
