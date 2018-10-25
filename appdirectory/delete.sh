#!/bin/bash
rm -rf autom4te.cache
rm -rf m4
make clean
files=("Makefile.in" "configure" "compile" "ltmain.sh" "install-sh" "depcomp" "missing" "aclocal.m4" "config.*" "libtool" "Makefile" "stamp-h1" "ogre.cfg" "Ogre.log", "CEGUI.log")
for f in ${files[*]}
do
  if [ -f "$f" ]; then
    echo "rm $f"
    rm $f
  fi 
done