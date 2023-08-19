#include "PublicKey.h"

PK::PK() {
	g2_null(pk); 
	g2_new(pk);
	id = "JD";
}

PK::PK(const PK& o){
	g2_null(pk); 
	g2_new(pk);
	g2_copy(pk, o.pk);
}

void PK::setKey(g2_t pk_o) {
	g2_copy(pk, pk_o);
}

void PK::setKey(string key) {
	uint8_t bin[8 * RLC_PC_BYTES + 1];
	int k = 0;
	int i = 0; int j = 1;
	int str_size = strlen(key.c_str());
	while (i < str_size){
		j = i+1;
		while (j < str_size && key[j]!='-'){ j++; }
		string tmp = key.substr(i, j);
		bin[k] = atoi(tmp.c_str());
		k++; 
		i = j+1;
	}
	g2_read_bin(pk, bin, k);
}

void PK::setID(string id_o) {
	id = id_o;
}

string PK::getKeyStr() {
	uint8_t* bin = (uint8_t*)malloc(sizeof(uint8_t)*(8*RLC_PC_BYTES+1));
	int l = g2_size_bin(pk, 1);
	g2_write_bin(bin, l, pk, 1);
	// cout << "pk: ";
	// for(int i = 0; i < l; i++){
	// 	cout << unsigned(bin[i]);
	// }
	// cout << endl;

	string key = "";
	for(int i = 0; i < l; i++){
		key += to_string(bin[i]) + "-";
	}
	key.pop_back();
	return key;
}

void PK::printKey() {
	g2_print(pk);
}

PK::~PK() {
	g2_free(pk);
}




MKPK::MKPK() {
	signers_num = 0;
}

MKPK::MKPK(int s_num) {
	signers_num = s_num;
	mkpks = new PK[signers_num]; 
	for (int i = 0; i < signers_num; i++){
		mkpks[i] = PK();
	}
}

void MKPK::setPKs(PK& pk, int i) {
	if(signers_num == 0){
		signers_num = 1;
	}
	if(i >= signers_num){ 
		i = 0;
	}
	mkpks[i] = pk;
}

MKPK::~MKPK() {
	delete[] mkpks;
}
