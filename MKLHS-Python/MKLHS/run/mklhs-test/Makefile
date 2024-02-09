CFLAGS=-O3 -mtune=native -mcpu=apple-a14 -ggdb
RELIC_SRC = ../../relic/src
RELIC_TARGET = ../../relic/target
MKLHS_ROOT = ../..

all:
	g++ ${CFLAGS} -c mklhs-test.c -o mklhs.o -I ${RELIC_SRC}/include -I ${RELIC_TARGET}/include -I ${MKLHS_ROOT}/src
	g++ ${CFLAGS} -o mklhs mklhs.o ${RELIC_TARGET}/lib/librelic_s.a  ${MKLHS_ROOT}/lib/libMKLHS.a -lgmp -lm
