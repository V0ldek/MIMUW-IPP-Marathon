#!/bin/bash
# Runs the given executable on all files *.in in the given directory and 
# compares stdout and stderr with correct outputs in *.out and *.err
# Uses valgrind to catch memory leaks and reports them as errors with exitcode
# $LEAKEXITCODE
#
# Arg1 -- Name of the executable to be tested
# Arg2 -- Directory with tests and outputs
#
# Author: Mateusz Gienieczko
# Copyright (C) 2018

if [ $# != 2 ];
then 
	echo "You need to pass exactly two arguments: program name \
and tests directory.";
	exit;
fi

PROG=$1
TESTDIR=$2

LEAKEXITCODE=42
VALGRINDFLAGS="--error-exitcode=$LEAKEXITCODE --leak-check=full \
--show-leak-kinds=all --errors-for-leak-kinds=all -q"

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
	if [ $OUT_DIFF -eq 0 -a $ERR_DIFF -eq 0 -a $EXIT_CODE -eq 0 ];
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
