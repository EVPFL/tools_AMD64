#include "LHHValue.h"

LHHVal::LHHVal(){
}

LHHVal::LHHVal(int val_len){
	val.resize(val_len);
	for (int i = 0; i < val_len; ++i)
		val[i] = BN_new();
}

LHHVal::~LHHVal(){
	// cout << "~LHHVal" << endl;
	for (int i = 0; i < val.size(); ++i)
		BN_free(val[i]);
}

void LHHVal::copy(const LHHVal& lhhval){
	val.resize(lhhval.val.size());
	for (int i = 0; i < lhhval.val.size(); ++i){
		val[i] = BN_new();
		BN_copy(val[i], lhhval.val[i]);
	}
}

void LHHVal::reset(int val_len){
	val.resize(val_len);
	for (int i = 0; i < val_len; ++i)
		val[i] = BN_new();
}

void LHHVal::setVal(string bin_val, int i){
	BN_dec2bn( &val[i], bin_val.c_str());
}

string LHHVal::getVal(int i){
	return BN_bn2dec(val[i]);
}

string LHHVal::getVal(){
	string val_hex = "";
	for (int i = 0; i < val.size(); i++){
		val_hex += BN_bn2dec(val[i]);
		val_hex += "-" ;
	}
	val_hex.pop_back();
	return val_hex;
}

void LHHVal::printVal(int i){
	cout << getVal(i) << endl;
}

void LHHVal::printVal(){
	cout << getVal() << endl;
}




LHHMsgs::LHHMsgs() {
	msgs_num = 0;
	val_len = 0;
}

LHHMsgs::LHHMsgs(int m_num, int v_len) {
	msgs_num = m_num;
	val_len = v_len;
	msgs = new LHHVal[msgs_num]; 
	for (int i = 0; i < msgs_num; i++){
		msgs[i].reset(val_len);
	}
}

LHHMsgs::~LHHMsgs() {
	delete[] msgs;
}

void LHHMsgs::reset(int m_num, int v_len){
	msgs_num = m_num;
	val_len = v_len;
	msgs = new LHHVal[msgs_num]; 
	for (int i = 0; i < msgs_num; i++){
		msgs[i].reset(val_len);
	}
}

void LHHMsgs::setMsgs(const LHHVal& msg, int i) {
	if(msgs_num == 0){
		msgs_num = 1;
	}
	if(i >= msgs_num){ 
		i = 0;
	}
	msgs[i].copy(msg);
}


