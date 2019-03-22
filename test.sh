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
  echo "Memory test:"
  valgrind --error-exitcode=15 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all ./${prog} <$f >vgtemp.out 2>vgtemp.err
  if [ "15" == "$?" ]
    then echo "FAIL"
    else echo "PASS"
  fi
  echo "Valgrind ended with exitcode $?."
done
rm temp.out
rm temp.err
rm vgtemp.out
rm vgtemp.err

