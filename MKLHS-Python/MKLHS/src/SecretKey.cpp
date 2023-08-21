#include "SecretKey.h"

SK::SK() {
	bn_null(sk); 
	bn_new(sk);
	id = "JD";
}

SK::SK(const SK& o){
	bn_null(sk); 
	bn_new(sk);
	bn_copy(sk, o.sk);
}

void SK::setKey(bn_t sk_o) {
	bn_copy(sk, sk_o);
}

void SK::setKey(string key) {
	bn_read_str(sk, key.c_str(), strlen(key.c_str()), 16);
}

void SK::setID(string id_o) {
	id = id_o;
}

string SK::getKeyStr() {
	char key[100];
	bn_write_str(key, 100, sk, 16);
	return key;
}

void SK::printKey() {
	bn_print(sk);
}

SK::~SK() {
	bn_free(sk);
}

