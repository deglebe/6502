# should be compiler agnostic across tcc, gcc, and clang
# can be overridden from command line: make CC=gcc
CC	?= tcc

CFLAGS	= -std=c99 -Wall
