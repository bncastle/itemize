Itemize: Creates a searchable blob of binary data from a bunch of input files
========

Itemize takes a bunch of files and crams them together into a single blob with a .item extension. The difference between this and simple concatenation is that the
final output file here is marked with special item headers that allow recovery of the original data from each included file.
This is like resource files for Windows but simpler.  A couple of ways to use it would be to combine a binch of files into one
.items file, then use [objectify](https://github.com/bncastle/objectify) to make a COFF which can then be linked with your C program. Access to the individual data blobs can be achieved by linking to the static itemize library and including itemize_lib.h

###Building

TODO

####Using Itemize

TODO