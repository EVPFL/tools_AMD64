CFLAGS=-O3 -mtune=native -ggdb
RELIC_ROOT = ./src

all: lib

lib:
	mkdir -p target
	cd target; ../${RELIC_ROOT}/preset/x64-pbc-bls12-381.sh ../${RELIC_ROOT}/; make -DEP_METHD='JACOB;LWNAF;COMBS;INTER' .; make

clean:
	rm -rf target *.o main
