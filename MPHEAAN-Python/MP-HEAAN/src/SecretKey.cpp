/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#include "SecretKey.h"

SecretKey::SecretKey(Ring& ring) {
	ring.sampleHWT(sx);
}

SecretKey::SecretKey(const SecretKey& o) {
	for (long i = 0; i < N; ++i) {
		sx[i] = o.sx[i];
	}
}

SecretKey::SecretKey(ZZ* sx_o) {
	for (long i = 0; i < N; i++) {
		sx[i] = sx_o[i];
	}
}

void SecretKey::setSK(ZZ* sx_o) {
	for (long i = 0; i < N; i++) {
		sx[i] = sx_o[i];
	}
}

// void SecretKey::setSK(long sx_num) {
// 	for (long i = 0; i < N; ++i) {
// 		sx[N-1-i] = ZZ( (sx_num>>i)& 1 );
// 	}
// }



SKShare::SKShare(uint64_t index) : index(index) {
}

SKShare::SKShare(uint64_t index, long np) : index(index), np(np) {
	rsx_share = new uint64_t[N*np];
}

SKShare::SKShare(uint64_t index, long np, uint64_t* rsx_share_o) : index(index), np(np) {
	rsx_share = new uint64_t[N*np];
	for (long i = 0; i < N*np; ++i) {
		rsx_share[i] = rsx_share_o[i];
	}
}

SKShare::~SKShare() {
	delete[] rsx_share;
}



// SKShares::SKShares(long size) : size(size) {
// 	indexs = new uint64_t[size];
// }

SKShares::SKShares(long size, long np) : size(size), np(np) {
	indexs = new uint64_t[size];
	rsx_shares = new uint64_t[N*np*size];
}

SKShares::SKShares(long size, long np, uint64_t* indexs_o, uint64_t* rsx_shares_o) : size(size), np(np) {
	indexs = new uint64_t[size];
	for (long i = 0; i < size; ++i) {
		indexs[i] = indexs_o[i];
	}
	rsx_shares = new uint64_t[N*np*size];
	for (long i = 0; i < N*np*size; ++i) {
		rsx_shares[i] = rsx_shares_o[i];
	}
}

uint64_t SKShares::getInxsItem(long i){
	if(i<size){
		return indexs[i];
	}
	else{
		return -1;
	}
}

uint64_t* SKShares::getRsxsItem(long i){
	uint64_t* rsx_ss = new uint64_t[N*np];
	if(i<size){
		for (int j=0; j<N*np; j++){
			rsx_ss[j] = rsx_shares[N*np*i+j];
		}
	}
	return rsx_ss;
}

SKShare SKShares::getItem(long i){
	if(i<size){
		return SKShare(indexs[i], np, rsx_shares+N*np*i);
	}
	else{
		return SKShare(-1);
	}
}


void SKShares::setInxsItem(long i, uint64_t index){
	indexs[i] = index;
}

void SKShares::setInxsItem(long i, long index){
	setInxsItem(i, (uint64_t)index);
}

void SKShares::setRsxsItem(long i, uint64_t* share){
	for(int j=0; j<N*np; j++){
		rsx_shares[i*N*np+j] = share[j];
	}
}

void SKShares::setItem(long i, uint64_t index, uint64_t* share){
	// indexs[i] = index;
	// for(int j=0; j<N*np; j++){
	// 	rsx_shares[i*N*np+j] = share[j];
	// }
	setInxsItem(i, index);
	setRsxsItem(i, share);
}

void SKShares::setItem(long i, long index, uint64_t* share){
	setInxsItem(i, index);
	setRsxsItem(i, share);
}

SKShares::~SKShares() {
	delete[] rsx_shares;
}








