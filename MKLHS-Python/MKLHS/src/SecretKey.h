#ifndef MKLHS_SECRETKEY_H_
#define MKLHS_SECRETKEY_H_

#include "relic.h"
#include <iostream>
using namespace std;

class SK {
public:
	bn_t sk;
	string id;

	SK();
	SK(const SK& o);

	void setKey(bn_t sk_o);
	void setKey(string sk_str);
	void setID(string id_o);

	string getKeyStr();
	void printKey();

	virtual ~SK();
};
#endif