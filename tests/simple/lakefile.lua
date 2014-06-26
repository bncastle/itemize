--
--
--
local prog_name = 'itemized_file_test'
local INCDIR = '../../src/'
local LIBDIR = '../../src/'
default {c.program{'./'..prog_name, src='itemized_file_test ',libs = 'itemizer', libdir = LIBDIR, incdir = INCDIR, dynamic=false, odir = 'obj'} }