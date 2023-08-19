/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#include "Key.h"
#include<cstring>

Key::Key() {
}

Key::Key(const Key& o){
	for (long i = 0; i < Nnprimes; ++i) {
		rax[i] = o.rax[i];
		rbx[i] = o.rbx[i];
	}
}

Key::Key(uint64_t* rax_o, uint64_t* rbx_o) {
	for (long i = 0; i < Nnprimes; i++) {
		rax[i] = rax_o[i];
	}
	for (long i = 0; i < Nnprimes; i++) {
		rbx[i] = rbx_o[i];
	}
}

// void Key::setKey(uint64_t* rax_o, uint64_t* rbx_o) {
// 	for (long i = 0; i < sizeof(rax_o)/sizeof(rax_o[0]); i++) {
// 		rax[i] = rax_o[i];
// 	}

// 	for (long i = 0; i < sizeof(rbx_o)/sizeof(rbx_o[0]); i++) {
// 		rbx[i] = rbx_o[i];
// 	}
// }

bool Key::isEmpty() {
	for (int i = 0; i < sizeof(rax)/sizeof(rax[0]); i++){
		if (rax[i] != 0){
			return false;
		}
	}
	for (int i = 0; i < sizeof(rbx)/sizeof(rbx[0]); i++){
		if (rbx[i] != 0){
			return false;
		}
	}
	return true;
}

Key::~Key() {
	delete[] rax;
	delete[] rbx;
}
