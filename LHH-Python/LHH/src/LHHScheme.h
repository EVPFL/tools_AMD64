#ifndef _LHHSCHEME_H
#define _LHHSCHEME_H

#include "HomHash.h"
#include "LHHValue.h"
#include "LHHHash.h"

#include <iostream>
using namespace std;

class LHHScheme{
private:
	int d;
	BN_CTX *ctx;

public:
	EC_GROUP *curve;
	EC_POINT *g0;
	vector<EC_POINT *> g;
	BIGNUM *q;
	
	LHHScheme(int d);
	~LHHScheme();

	// Get Parmas
	// int getValLen();
	string getModStr();

	// Basic hash function
	void resetHashes(LHHHash& hash, int val_len);
	void setHashes(LHHHash& hash, const LHHHash& hash_single, int i);
	void setHashes(LHHHash& hash, string hash_hex_str, int i);

	string getHashesHex(const LHHHash& hash);
	string getHashesHex(const LHHHash& hash, int i);
	void printHash(const LHHHash& hash);
	void printHash(const LHHHash& hash, int i);
	int cmpHash(const LHHHash& hash1, const LHHHash& hash2); // return 1: hash1 == hash2


	// Compute homomorphic hash
	void Hash(EC_POINT *hash, const vector<BIGNUM *> &vec);
	void getHash(LHHHash& hash, const LHHVal& msg);
	
	// Linear combination of hashes
	void Eval(EC_POINT *res, const vector<EC_POINT *> &hashes, const vector<BIGNUM *> &alphas);
	void evalHashes(LHHHash& hash_agg, const LHHHash& hashes, const LHHVal& alphas);

	// Linear combination of msgs
	void evalMsgs(LHHVal& agg_msg, const LHHMsgs& msgs, const LHHVal& alphas);

	// Verify: hash_agg = Hash(msg*alphas)
	int verHashes(const LHHHash& hash_agg, const LHHMsgs& msgs, const LHHVal &alphas);

};

#endif