Compressor program of Plain Huffman (PH)
http://vios.dc.fi.udc.es/codes

Permits to compress a text file with PH.
The text file cannot contain the character '\0'. Otherwise compression will work in a wrong way.

** Syntax: CPH <inputFile> <compressedFile> <[VOC SIZE]>

The parameter "VOC SIZE" is optional. If skipped, the compressor uses Heap's law to stimate the size of the vocabulary (number of different words in the inputFile). If "VOC SIZE" is given (it is known by the user), the compressor uses "explicitly" that value instead of the stimated value.


Notes: 
 
 - Two files "compressedFile" and "compressedFile.voc" are output.
  - A file "timesCompPH" is also created after compression. It logs compression times (useful to perform several runs of the compressor and then collect all the time measures).
 
 
Notes 2:
  - Compile with -DDEBUGCODES to see the codes assigned.

==========================================================
Canonical codes are generated  based on::
	http://www.cs.mu.oz.au/~alistair/abstracts/inplace.html
	http://www.cs.mu.oz.au/~alistair/inplace.c

       The method for calculating codelengths in function
        calculate_minimum_redundancy is described in 

        @inproceedings{mk95:wads,
                author = "A. Moffat and J. Katajainen",
                title = "In-place calculation of minimum-redundancy codes",
                booktitle = "Proc. Workshop on Algorithms and Data Structures",
                address = "Queen's University, Kingston, Ontario",
                publisher = "LNCS 955, Springer-Verlag",
                Month = aug,
                year = 1995,
                editor = "S.G. Akl and F. Dehne and J.-R. Sack",
                pages = "393-402",
        }

        The abstract of that paper may be fetched from 
        http://www.cs.mu.oz.au/~alistair/abstracts/wads95.html
============================================================
