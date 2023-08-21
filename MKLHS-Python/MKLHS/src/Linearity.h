#ifndef MKLHS_LINEARITY_H_
#define MKLHS_LINEARITY_H_

#include "relic.h"
#include <vector>
#include <iostream>
using namespace std;


class Linearity {
public:
	int size;
	dig_t* f;

	Linearity();
	Linearity(int size);
	Linearity(const Linearity& o);

	void setCoeff(int coeff_dig, int i);
	void copyCoeff(const Linearity& o);

	int getCoeff(int i);
	void printLinearity();
	void printLinearity(int fs_num);
	void printLinearity(int start_inx, int end_inx);

	virtual ~Linearity();
};


class MKLinearity {
public:
	int signers_num;
	int msgs_num;
	// vector<Linearity> fs;
	Linearity* fs;

	MKLinearity();
	MKLinearity(int signers_num, int msgs_num);

	void setCoeffs(Linearity& coeffs, int i);

	virtual ~MKLinearity();
};

#endif

