#!/bin/bash
s="1"
sigma="abcdefghijklmnop"
file="exp"
./mksary ${file}.x ${file}.sa
./bwt -f ${file} -s ${s} -b dumy -a ${sigma}
