--
--
--
local LOC = 'release/'
local prog_name = 'itemize'

if(DEBUG) then LOC = 'debug/' end

default {
c.program{LOC..prog_name, src='*',dynamic=false, odir = 'obj'},
c.library{LOC..'itemize', src='itemize_lib', dynamic = false, odir='obj'}
}