#!/bin/sh

../examples/test36 -h -a 5 > tmp.out 2>&1

if cmp -s tmp.out $srcdir/test103.out; then
	exit 0
else
	exit 1
fi
