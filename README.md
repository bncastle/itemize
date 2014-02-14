Itemize: Creates a searchable blob of binary data from a bunch of input files
========

Itemize takes a bunch of files and crams them together into a single blob with a .item extension. The
final output file is marked with special item headers that allow recovery of the original data from each included file.
A couple of ways to use it would be to "itemize" a bunch of files, then use [objectify](https://github.com/bncastle/objectify) to make a COFF which can then be linked with a C program. Access to the individual data blobs can be achieved by linking to the static itemize library and including itemize_lib.h

###Building

TODO

####Using Itemize

TODO