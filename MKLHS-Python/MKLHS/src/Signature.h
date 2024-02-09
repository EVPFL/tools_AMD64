#ifndef MKLHS_SIGNATURE_H_
#define MKLHS_SIGNATURE_H_

#include "relic.h"
#include <vector>
#include <iostream>
using namespace std;

class Signature {
public:
	int size;
	g1_t* signs;

	Signature();
	Signature(int size);
	Signature(const Signature& o);

	string getSignStr(int inx);
	string getSignStr();
	void setSign(string sign_str, int inx);
	void setSign(string signs_str);

	void printSigns();
	void printSigns(int signs_num);
	void printSigns(int start_inx, int end_inx);


	virtual ~Signature();
};


class MKSignature {
public:
	int signers_num;
	int msgs_num;
	// vector<Signature*> mksigns;
	Signature* mksigns;

	MKSignature();
	MKSignature(int signers_num);
	MKSignature(int signers_num, int msgs_num);

	void setSigns(Signature& sign, int i);

	virtual ~MKSignature();
};

#endif