/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#include "Ciphertext.h"

#include <NTL/tools.h>

Ciphertext::Ciphertext(long logp, long logq, long n) : logp(logp), logq(logq), n(n) {
}

Ciphertext::Ciphertext(const Ciphertext& o) : logp(o.logp), logq(o.logq), n(o.n) {
	for (long i = 0; i < N; ++i) {
		ax[i] = o.ax[i];
		bx[i] = o.bx[i];
	}
}

Ciphertext::Ciphertext(long logp_o, long logq_o, long n_o, ZZ* ax_o, ZZ* bx_o) {
	logp = logp_o;
	logq = logq_o;
	n = n_o; 
	for (long i = 0; i < N; ++i) {
		ax[i] = ax_o[i];
		bx[i] = bx_o[i];
	}
}

void Ciphertext::copyParams(Ciphertext& o) {
	logp = o.logp;
	logq = o.logq;
	n = o.n;
}

void Ciphertext::copy(Ciphertext& o) {
	copyParams(o);
	for (long i = 0; i < N; ++i) {
		ax[i] = o.ax[i];
		bx[i] = o.bx[i];
	}
}

// void Ciphertext::free() {
// 	for (long i = 0; i < N; ++i) {
// 		clear(ax[i]);
// 		clear(bx[i]);
// 	}
// }

Ciphertext::~Ciphertext() {
	delete[] ax;
	delete[] bx;
}



Ciphertexts::Ciphertexts(long size) : size(size) {
	logp = 0;
	logq = 0; 
	n = 0;
	bxs = new ZZ[N*size];
}

Ciphertexts::Ciphertexts(long size, long logp, long logq, long n) : size(size), logp(logp), logq(logq), n(n) {
	bxs = new ZZ[N*size];
}

Ciphertexts::Ciphertexts(long size, long logp, long logq, long n, ZZ* ax_o, ZZ* bxs_o) : size(size), logp(logp), logq(logq), n(n) {
	for (long i = 0; i < N; ++i) {
		ax[i] = ax_o[i];
	}
	bxs = new ZZ[N*size];
	for (long i = 0; i < N*size; ++i) {
		bxs[i] = bxs_o[i];
	}
}

void Ciphertexts::copy(Ciphertexts& o) {
	logp = o.logp;
	logq = o.logq;
	n = o.n;
	size = o.size;
	for (long i = 0; i < N; ++i) {
		ax[i] = o.ax[i];
	}
	for (long i = 0; i < N*size; ++i) {
		bxs[i] = o.bxs[i];
	}
}

Ciphertext Ciphertexts::getItem(long inx){
	if(inx<size){
		return Ciphertext(logp, logq, n, ax, bxs+inx*N);
	}
	else{
		return Ciphertext(logp, logq, n);
	}
}


ZZ* Ciphertexts::getBxsItem(long inx){
	if(inx<size){
		return bxs+inx*N;
	}
	else{
		return new ZZ[N];
	}
}


Ciphertexts::~Ciphertexts() {
	delete[] ax;
	delete[] bxs;
}



