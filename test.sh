#!/bin/bash

if [ $# != 2 ];
then 
	echo "You need to pass exactly two arguments: program name \
and tests directory.";
	exit;
fi

PROG=$1
TESTDIR=$2

VALGRINDFLAGS="--error-exitcode=15 --leak-check=full --show-leak-kinds=all \
--errors-for-leak-kinds=all -q"

CORRECT=0
WRONG=0

for f in $TESTDIR/*.in; do

	OUTF="$(mktemp)";
	
	valgrind $VALGRINDFLAGS ./$PROG < $f 1>$OUTF.out 2>$OUTF.err
	EXIT_CODE=$?
	
	diff ${f%in}out $OUTF.out &>/dev/null
	OUT_DIFF=$?
	
	diff ${f%in}err $OUTF.err &>/dev/null
	ERR_DIFF=$?
	
	echo -n "${f#$TESTDIR/}: exit code $EXIT_CODE";
	if [ $OUT_DIFF -eq 0 -a $ERR_DIFF -eq 0 -a $EXIT_CODE -eq 0];
	then 
		echo -n " OK"; 
		((CORRECT+=1));
	else
		if [ $OUT_DIFF -ne 0 ];
		then echo -n " wrong output";
		fi
		if [ $ERR_DIFF -ne 0 ];
		then echo -n " wrong diagnostic output";
		fi
		
		((WRONG+=1));
	fi
	echo "."
	
	rm -f $OUTF.out $OUTF.err
	
done;

echo "Passed $CORRECT tests, failed $WRONG tests."
