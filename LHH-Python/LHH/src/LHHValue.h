#ifndef _LHHVALUE_H
#define _LHHVALUE_H

#include "HomHash.h"
#include <iostream>
using namespace std;

class LHHVal{
public:
	vector<BIGNUM *> val;
	
	LHHVal();
	LHHVal(int val_len);
	~LHHVal();

	// Set
	void copy(const LHHVal& lhhval);
	void reset(int val_len);
	void setVal(string bin_val, int i);

	// Get
	string getVal(int i);
	string getVal();

	// Print
	void printVal(int i);
	void printVal();
};


class LHHMsgs{
public:
	int msgs_num;
	int val_len;
	LHHVal* msgs;
	
	LHHMsgs();
	LHHMsgs(int msgs_num, int val_len);
	~LHHMsgs();

	// Set
	void reset(int msgs_num, int val_len);
	void setMsgs(const LHHVal& msg, int i);
};

#endif