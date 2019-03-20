#!/bin/bash
dir=$1
prog=$2

for f in ${dir}/*.in
do 
  ./${prog} <$f >temp.out 2>temp.err
  echo "Dla pliku $f program zakonczyl sie kodem $?."
  if diff ${f%in}out temp.out >/dev/null 2>&1
	then echo "dla pliku $f zgadza sie std wyjscie"
	else echo "dla pliku $f wyjscie std sie nie zgadza"
  fi
  if diff ${f%in}err temp.err >/dev/null 2>&1
	then echo "dla pliku $f zgadza sie err wyjscie"
	else echo "dla pliku $f wyjscie err sie nie zgadza"
  fi
done
