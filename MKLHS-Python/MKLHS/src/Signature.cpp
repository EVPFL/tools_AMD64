#include "Signature.h"

Signature::Signature() {
	size = 1;
	signs = (g1_t*)malloc(sizeof(g1_t)*1);
	g1_null(signs[0]);
	g1_new(signs[0]);
}

Signature::Signature(int sign_size) {
	size = sign_size;
	signs = (g1_t*)malloc(sizeof(g1_t)*size);
	for (int i = 0; i < size; i++){
		g1_null(signs[i]);
		g1_new(signs[i]);
	}
}

Signature::Signature(const Signature& o) {
	size = o.size;
	signs = (g1_t*)malloc(sizeof(g1_t)*size);
	for (int i = 0; i < size; i++){
		g1_null(signs[i]);
		g1_new(signs[i]);
		g1_copy(signs[i], o.signs[i]);
	}
}

string Signature::getSignStr(int inx) {
	string sign_str = "";
	uint8_t* bin = (uint8_t*)malloc(sizeof(uint8_t)*(8*RLC_PC_BYTES+1));
	int l = g1_size_bin(signs[inx], 1);
	g1_write_bin(bin, l, signs[inx], 1);
	for(int j = 0; j < l; j++){
		sign_str += to_string(bin[j]) + "-";
	}
	sign_str.pop_back();
	return sign_str;
}

string Signature::getSignStr() {
	string signs_str = "";
	uint8_t* bin = (uint8_t*)malloc(sizeof(uint8_t)*(8*RLC_PC_BYTES+1));
	for(int inx = 0; inx < size; inx++){
		int l = g1_size_bin(signs[inx], 1);
		g1_write_bin(bin, l, signs[inx], 1);
		for(int j = 0; j < l; j++){
			signs_str += to_string(bin[j]) + "-";
		}
		signs_str.pop_back();
		signs_str += ';';
	}
	signs_str.pop_back();
	return signs_str;
}

void Signature::setSign(string sign_str, int inx) {
	uint8_t bin[8 * RLC_PC_BYTES + 1];
	int k = 0;
	int i = 0; int j = 1;
	int str_size = strlen(sign_str.c_str());
	while (i < str_size){
		j = i+1;
		while (j < str_size && sign_str[j]!='-'){ j++; }
		string tmp = sign_str.substr(i, j);
		bin[k] = atoi(tmp.c_str());
		k++; 
		i = j+1;
	}
	g1_read_bin(signs[inx], bin, k);
}


void split(const string& s, vector<string>& tokens, char delim = ' ') {
    tokens.clear();
    size_t lastPos = s.find_first_not_of(delim, 0);
    size_t pos = s.find(delim, lastPos);
    while (lastPos != std::string::npos) {
        tokens.emplace_back(s.substr(lastPos, pos - lastPos));
        lastPos = s.find_first_not_of(delim, pos);
        pos = s.find(delim, lastPos);
    }
}

void Signature::setSign(string signs_str) {
	uint8_t bin[8 * RLC_PC_BYTES + 1];

	vector<string> sign_strs;
	split(signs_str, sign_strs, ';');

	for(int inx = 0; inx < size; inx++){
		int k = 0;
		int i = 0; int j = 1;
		int str_size = strlen(sign_strs[inx].c_str());
		while (i < str_size){
			j = i+1;
			while (j < str_size && sign_strs[inx][j]!='-'){ j++; }
			string tmp = sign_strs[inx].substr(i, j);
			bin[k] = atoi(tmp.c_str());
			k++; 
			i = j+1;
		}
		g1_read_bin(signs[inx], bin, k);
	}
}


void Signature::printSigns() {
	for (int i = 0; i < size; i++){
		cout << "sign[" << i << "]: ";
		g1_print(signs[i]);
	}
}

void Signature::printSigns(int signs_num) {
	if (signs_num > size){
		signs_num = size;
		cout << "number of signs: " << size << "." << endl;
	}
	for (int i = 0; i < signs_num; i++){
		cout << "sign[" << i << "]: ";
		g1_print(signs[i]);
	}
}

void Signature::printSigns(int start_inx, int end_inx) {
	while (start_inx < 0 and size!=0 ){
		start_inx += size;
	}
	while (end_inx < 0 and size!=0 ){
		end_inx += size;
	}
	if(end_inx > size){
		end_inx = size;
	}
	if(end_inx < start_inx){
		int tmp = end_inx;
		end_inx = start_inx;
		start_inx = tmp;
	}

	cout << "number of signs: " << size << "." << endl;
	for (int i = start_inx; i < end_inx; i++){
		cout << "sign[" << i << "]: ";
		g1_print(signs[i]);
	}
}

Signature::~Signature() {
	for (int i = 0; i < size; i++){
		g1_free(signs[i]);
	}
}



MKSignature::MKSignature() {
	signers_num = 0;
	msgs_num = 0;
}


MKSignature::MKSignature(int s_num) {
	signers_num = s_num;
	msgs_num = 1;

	mksigns = new Signature[signers_num]; 
	for (int i = 0; i < signers_num; i++){
		mksigns[i] = Signature();
	}
}

MKSignature::MKSignature(int s_num, int m_num) {
	signers_num = s_num;
	msgs_num = m_num;

	mksigns = new Signature[signers_num]; 
	for (int i = 0; i < signers_num; i++){
		mksigns[i] = Signature(msgs_num);
	}
}

void MKSignature::setSigns(Signature& sign, int i) {
	if(signers_num == 0){
		signers_num = 1;
	}
	if(i >= signers_num){ 
		i = 0;
	}
	mksigns[i] = sign;
}

MKSignature::~MKSignature() {
	delete[] mksigns;
}

