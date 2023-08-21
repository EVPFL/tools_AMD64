#include "Linearity.h"

Linearity::Linearity() {
	size = 0;
}

Linearity::Linearity(int lin_size) {
	size = lin_size;
	f = (dig_t*)malloc(sizeof(dig_t)*size);
	for (int i = 0; i < size; i++){
		f[i] = 1 ;
	}
}

Linearity::Linearity(const Linearity& o){
	size = o.size;
	f = (dig_t*)malloc(sizeof(dig_t)*size);
	for (int i = 0; i < size; i++){
		f[i] = o.f[i];
	}
}

void Linearity::setCoeff(int coeff_dig, int i){
	if(size == 0){
		size = 1;
		f = (dig_t*)malloc(sizeof(dig_t)*size);
	}
	if(i >= size){ 
		i = 0;
	}
	f[i] = coeff_dig;
}

void Linearity::copyCoeff(const Linearity& o){
	int copy_size = o.size;
	if (copy_size>size) { copy_size = size; }
	for (int i = 0; i < copy_size; i++){
		f[i] = o.f[i];
	}
}

int Linearity::getCoeff(int i) {
	return f[i];
}

void Linearity::printLinearity() {
	for (int i = 0; i < size; i++){
		cout << "f[" << i << "]: " << f[i] << endl;
	}
}

void Linearity::printLinearity(int fs_num) {
	if (fs_num > size){
		fs_num = size;
		cout << "number of coeffs: " << size << "." << endl;
	}
	for (int i = 0; i < fs_num; i++){
		cout << "f[" << i << "]: " << f[i] << endl;
	}
}

void Linearity::printLinearity(int start_inx, int end_inx) {
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

	for (int i = start_inx; i < end_inx; i++){
		cout << "f[" << i << "]: " << f[i] << endl;
	}
}

Linearity::~Linearity() {
	// free(f);
}




MKLinearity::MKLinearity() {
	signers_num = 0;
	msgs_num = 0;
}

// MKLinearity::MKLinearity(int s_num, int m_num) {
// 	signers_num = s_num;
// 	msgs_num = m_num;
// 	for (int i = 0; i < signers_num; i++){
// 		Linearity f = Linearity(msgs_num);
// 		fs.push_back(&f);
// 	}
// }

// void MKLinearity::setCoeffs(Linearity& coeffs, int i) {
// 	if(signers_num == 0){
// 		signers_num = 1;
// 	}
// 	if(i >= signers_num){ 
// 		i = 0;
// 	}
// 	fs.erase(fs.begin()+i);
// 	fs.insert(fs.begin()+i, &coeffs);
// }

MKLinearity::MKLinearity(int s_num, int m_num) {
	signers_num = s_num;
	msgs_num = m_num;
	fs = new Linearity[signers_num]; 
	for (int i = 0; i < signers_num; i++){
		fs[i] = Linearity(msgs_num);
	}
}

void MKLinearity::setCoeffs(Linearity& coeffs, int i) {
	if(signers_num == 0){
		signers_num = 1;
	}
	if(i >= signers_num){ 
		i = 0;
	}
	fs[i] = coeffs;
}

MKLinearity::~MKLinearity() {
	delete[] fs;
}









