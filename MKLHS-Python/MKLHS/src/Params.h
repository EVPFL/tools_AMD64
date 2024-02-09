#ifndef MKLHS_PARAMS_H_
#define MKLHS_PARAMS_H_

#include "relic.h"
#include <iostream>
using namespace std;

class Params{
public:
	bn_t n;
	string dataset;
	string label;

	Params();

	void setN(string n_str);
	void setDataset(string dataset_o);
	void setLabel(string label_o);

	string getNStr();
	void printParams();

	void free();
	// virtual ~Params();
};
#endif
