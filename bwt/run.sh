#!/bin/bash
s=1
sigma="abcdefghijklmnop"
file="exp"
./bwt -f ${file} -s ${s} -r du -a ${sigma} -q $1 -d 1
