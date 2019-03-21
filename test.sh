#!/bin/bash
dir=$1
prog=$2

for f in ${dir}/*.in
do 
  ./${prog} <$f >temp.out 2>temp.err
  echo "Test $f. The program has ended with exitcode $?."
  if diff ${f%in}out temp.out >/dev/null 2>&1
	then echo "Pass! No differences in out files for ${f%in}out."
	else echo "FAILURE! Differences found for out files with ${f%in}out."
  fi
  if diff ${f%in}err temp.err >/dev/null 2>&1
	then echo "Pass! No differences in err files for ${f%in}err."
	else echo "FAILURE! Differences found for err files with ${f%in}err."
  fi
done
