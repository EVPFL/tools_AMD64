#include "LHHHash.h"

LHHHash::LHHHash(){
}

LHHHash::LHHHash(int val_len, const EC_GROUP *curve){
	val.resize(val_len);
	for (int i = 0; i < val_len; ++i)
		val[i] = EC_POINT_new(curve);
}

LHHHash::~LHHHash(){
	for (int i = 0; i < val.size(); ++i)
		EC_POINT_free(val[i]);
}

void LHHHash::reset(int val_len, const EC_GROUP *curve){
	val.resize(val_len);
	for (int i = 0; i < val_len; ++i)
		val[i] = EC_POINT_new(curve);
}

void LHHHash::resize(int val_len, const EC_GROUP *curve){
	int org_size = val.size();
	if(val_len > org_size){
		val.resize(val_len);
		for (int i = org_size; i < val_len; ++i)
			val[i] = EC_POINT_new(curve);
	}
	else{
		val.resize(val_len);
	}
}


void LHHHash::setVal(string hex_val, int i, const EC_GROUP *curve, BN_CTX *ctx){
	EC_POINT_hex2point(curve, hex_val.c_str(), val[i], ctx);
}


string LHHHash::getVal(int i, const EC_GROUP *curve, BN_CTX *ctx) const{
	return EC_POINT_point2hex(curve, val[i], POINT_CONVERSION_UNCOMPRESSED, ctx);
}

string LHHHash::getVal(const EC_GROUP *curve, BN_CTX *ctx) const{
	string val_hex = "";
	for (int i = 0; i < val.size(); i++){
		val_hex += EC_POINT_point2hex(curve, val[i], POINT_CONVERSION_UNCOMPRESSED, ctx);
		val_hex += "-" ;
	}
	val_hex.pop_back();
	return val_hex;
}


void LHHHash::printVal(int i, const EC_GROUP *curve, BN_CTX *ctx) const{
	cout << getVal(i, curve, ctx) << endl;
}

void LHHHash::printVal(const EC_GROUP *curve, BN_CTX *ctx) const{
	cout << getVal(curve, ctx) << endl;
}