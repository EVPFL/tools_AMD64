#ifndef MKLHS_PUBLICKEY_H_
#define MKLHS_PUBLICKEY_H_

#include "relic.h"
#include <vector>
#include <iostream>
using namespace std;

class PK {
public:
	g2_t pk;
	string id;

	PK();
	PK(const PK& o);

	void setID(string id_o);
	void setKey(g2_t pk_o);
	void setKey(string pk_str);

	string getKeyStr();
	void printKey();

	virtual ~PK();
};

class MKPK {
public:
	int signers_num;
	// vector<PK*> mkpks;
	PK* mkpks;

	MKPK();
	MKPK(int signers_num);

	void setPKs(PK& pk, int i);

	virtual ~MKPK();
};
#endif