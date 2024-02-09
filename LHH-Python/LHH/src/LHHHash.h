#ifndef _LHHHash_H
#define _LHHHash_H

#include "HomHash.h"
#include <iostream>
using namespace std;

class LHHHash{
public:
	vector<EC_POINT *> val;
	LHHHash();
	LHHHash(int val_len, const EC_GROUP *curve);
	~LHHHash();

	// Set
	void reset(int val_len, const EC_GROUP *curve);
	void resize(int val_len, const EC_GROUP *curve);
	void setVal(string hex_val, int i, const EC_GROUP *curve, BN_CTX *ctx);

	// Get
	string getVal(int i, const EC_GROUP *curve, BN_CTX *ctx) const;
	string getVal(const EC_GROUP *curve, BN_CTX *ctx) const;

	// Print
	void printVal(int i, const EC_GROUP *curve, BN_CTX *ctx) const;
	void printVal(const EC_GROUP *curve, BN_CTX *ctx) const;
};

#endif