/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/

#include "MPScheme.h"

#include "NTL/BasicThreadPool.h"
#include "StringUtils.h"
#include "SerializationUtils.h"

#include <time.h>
#include <vector>

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>


MPScheme::MPScheme(Ring& ring) : ring(ring) {
};

MPScheme::~MPScheme() {
}

//-----------------------------------------

//------------ key generation -------------
Key MPScheme::getKey(SecretKey& secretKey, long key_flag) {
	Key key;
	//cout << "getting pk" << endl;
	switch(key_flag){
		case ENCRYPTION: //EncKey
			key = getEncKey(secretKey);
			break;
		case MULTIPLICATION: //MultKey
			key = getMultKey(secretKey);
			break;
		case CONJUGATION: //ConjKey
			key = getConjKey(secretKey);
			break;
		default:
			printf("error key_flag\n"); 
			break;
	}
	//cout << "  pk rax[2]: " << key->rax[2] << endl;
	return key;
}

Key MPScheme::getKey(SecretKey& secretKey, long key_flag, long r) {
	Key key;
	switch(key_flag){
		case ROTATION: //LeftRotKey
			key = getLeftRotKey(secretKey, r);
			break;
		default:
			printf("error key_flag\n"); 
			break;
	}
	return key;
}

Key MPScheme::getEncKey(SecretKey& secretKey) {
	ZZ* ax = new ZZ[N];
	ZZ* bx = new ZZ[N];

	long np = ceil((1 + logQQ + logN + 2)/(double)pbnd);
	ring.sampleUniform2(ax, logQQ);
	ring.mult(bx, secretKey.sx, ax, np, QQ);
	ring.subFromGaussAndEqual(bx, QQ);

	Key key;
	ring.CRT(key.rax, ax, nprimes);
	ring.CRT(key.rbx, bx, nprimes);
	delete[] ax; delete[] bx;

	return key;
}

Key MPScheme::getMultKey(SecretKey& secretKey) {
	ZZ* ax = new ZZ[N];
	ZZ* bx = new ZZ[N];
	ZZ* sxsx = new ZZ[N];

	long np = ceil((1 + logQQ + logN + 2)/(double)pbnd);
	ring.sampleUniform2(ax, logQQ);
	ring.mult(bx, secretKey.sx, ax, np, QQ);
	ring.subFromGaussAndEqual(bx, QQ);

	np = ceil((2 + logN + 2)/(double)pbnd);
	ring.mult(sxsx, secretKey.sx, secretKey.sx, np, Q);
	ring.leftShiftAndEqual(sxsx, logQ, QQ);
	ring.addAndEqual(bx, sxsx, QQ);
	delete[] sxsx;

	Key key;
	ring.CRT(key.rax, ax, nprimes);
	ring.CRT(key.rbx, bx, nprimes);
	delete[] ax; delete[] bx;

	return key;
}

Key MPScheme::getConjKey(SecretKey& secretKey) {
	ZZ* ax = new ZZ[N];
	ZZ* bx = new ZZ[N];

	long np = ceil((1 + logQQ + logN + 2)/(double)pbnd);
	ring.sampleUniform2(ax, logQQ);
	ring.mult(bx, secretKey.sx, ax, np, QQ);
	ring.subFromGaussAndEqual(bx, QQ);

	ZZ* sxconj = new ZZ[N];
	ring.conjugate(sxconj, secretKey.sx);
	ring.leftShiftAndEqual(sxconj, logQ, QQ);
	ring.addAndEqual(bx, sxconj, QQ);
	delete[] sxconj;

	Key key;
	ring.CRT(key.rax, ax, nprimes);
	ring.CRT(key.rbx, bx, nprimes);
	delete[] ax; delete[] bx;

	return key;
}

Key MPScheme::getLeftRotKey(SecretKey& secretKey, long r) {
	ZZ* ax = new ZZ[N];
	ZZ* bx = new ZZ[N];

	long np = ceil((1 + logQQ + logN + 2)/(double)pbnd);
	ring.sampleUniform2(ax, logQQ);
	ring.mult(bx, secretKey.sx, ax, np, QQ);
	ring.subFromGaussAndEqual(bx, QQ);

	ZZ* spow = new ZZ[N];
	ring.leftRotate(spow, secretKey.sx, r);
	ring.leftShiftAndEqual(spow, logQ, QQ);
	ring.addAndEqual(bx, spow, QQ);
	delete[] spow;

	Key key;
	ring.CRT(key.rax, ax, nprimes);
	ring.CRT(key.rbx, bx, nprimes);
	delete[] ax; delete[] bx;

	return key;
}

Key MPScheme::getRightRotKey(SecretKey& secretKey, long r) {
	long idx = Nh - r;
	return getLeftRotKey(secretKey, idx);
}

Key* MPScheme::getLeftRotKeys(SecretKey& secretKey) {
	Key* keys = new Key[logN-1];
	for (long i = 0; i < logN - 1; ++i) {
		long idx = 1 << i;
		keys[i] = getLeftRotKey(secretKey, idx);
	}
	return keys;
}

Key* MPScheme::getRightRotKeys(SecretKey& secretKey) {
	Key* keys = new Key[logN-1];
	for (long i = 0; i < logN - 1; ++i) {
		long idx = Nh - (1 << i);
		keys[i] = getLeftRotKey(secretKey, idx);
	}
	return keys;
}
//-----------------------------------------
//-------------- fixed ax/rax -------------
Key MPScheme::getEncKey(SecretKey& secretKey, ZZ* ax) {
	ZZ* ax_c = new ZZ[N];
	for (long i = 0; i < N; i++) {
		ax_c[i] = ax[i];
	}
	ZZ* bx = new ZZ[N];

	long np = ceil((1 + logQQ + logN + 2)/(double)pbnd);
	ring.mult(bx, secretKey.sx, ax_c, np, QQ);
	ring.subFromGaussAndEqual(bx, QQ);

	Key key;
	ring.CRT(key.rax, ax_c, nprimes);
	ring.CRT(key.rbx, bx, nprimes);
	delete[] ax_c; delete[] bx; 

	return key;
}

Key MPScheme::getEncKey(SecretKey& secretKey, uint64_t* rax) {
	Key key;
	for (long i = 0; i < Nnprimes; i++) {
		key.rax[i] = rax[i];
	}
	ZZ* bx = new ZZ[N];

	long np = ceil((1 + logQQ + logN + 2)/(double)pbnd);
	ring.multNTT(bx, secretKey.sx, key.rax, np, QQ);
	ring.subFromGaussAndEqual(bx, QQ);
	ring.CRT(key.rbx, bx, nprimes);
	delete[] bx;
	return key;
}

Key MPScheme::getMultKey(SecretKey& secretKey, ZZ* ax) {
	ZZ* ax_c = new ZZ[N];
	for (long i = 0; i < N; i++) {
		ax_c[i] = ax[i];
	}
	ZZ* bx = new ZZ[N];
	ZZ* sxsx = new ZZ[N];
	long np = ceil((1 + logQQ + logN + 2)/(double)pbnd);
	ring.mult(bx, secretKey.sx, ax_c, np, QQ);
	ring.subFromGaussAndEqual(bx, QQ);
	np = ceil((2 + logN + 2)/(double)pbnd);
	ring.mult(sxsx, secretKey.sx, secretKey.sx, np, Q);
	ring.leftShiftAndEqual(sxsx, logQ, QQ);
	ring.addAndEqual(bx, sxsx, QQ);
	delete[] sxsx;
	Key key;
	ring.CRT(key.rax, ax_c, nprimes);
	ring.CRT(key.rbx, bx, nprimes);
	delete[] ax_c; delete[] bx;
	return key;
}

Key MPScheme::getMultKey(SecretKey& secretKey, uint64_t* rax) {
	Key key;
	ZZ* bx = new ZZ[N];
	ZZ* sxsx = new ZZ[N];
	for (long i = 0; i < Nnprimes; i++) {
		key.rax[i] = rax[i];
	}
	long np = ceil((1 + logQQ + logN + 2)/(double)pbnd);
	ring.multNTT(bx, secretKey.sx, key.rax, np, QQ);
	ring.subFromGaussAndEqual(bx, QQ);
	np = ceil((2 + logN + 2)/(double)pbnd);
	ring.mult(sxsx, secretKey.sx, secretKey.sx, np, Q);
	ring.leftShiftAndEqual(sxsx, logQ, QQ);
	ring.addAndEqual(bx, sxsx, QQ);
	delete[] sxsx;
	ring.CRT(key.rbx, bx, nprimes);
	delete[] bx;
	return key;
}

Key MPScheme::getLeftRotKey(SecretKey& secretKey, long r, ZZ* ax) {
	ZZ* ax_c = new ZZ[N];
	for (long i = 0; i < N; i++) {
		ax_c[i] = ax[i];
	}
	ZZ* bx = new ZZ[N];
	long np = ceil((1 + logQQ + logN + 2)/(double)pbnd);
	ring.mult(bx, secretKey.sx, ax_c, np, QQ);
	ring.subFromGaussAndEqual(bx, QQ);
	ZZ* spow = new ZZ[N];
	ring.leftRotate(spow, secretKey.sx, r);
	ring.leftShiftAndEqual(spow, logQ, QQ);
	ring.addAndEqual(bx, spow, QQ);
	delete[] spow;
	Key key;
	ring.CRT(key.rax, ax_c, nprimes);
	ring.CRT(key.rbx, bx, nprimes);
	delete[] ax_c; delete[] bx;
	return key;
}

Key MPScheme::getLeftRotKey(SecretKey& secretKey, long r, uint64_t* rax) {
	Key key;
	ZZ* bx = new ZZ[N];
	for (long i = 0; i < Nnprimes; i++) {
		key.rax[i] = rax[i];
	}
	long np = ceil((1 + logQQ + logN + 2)/(double)pbnd);
	ring.multNTT(bx, secretKey.sx, key.rax, np, QQ);
	ring.subFromGaussAndEqual(bx, QQ);
	ZZ* spow = new ZZ[N];
	ring.leftRotate(spow, secretKey.sx, r);
	ring.leftShiftAndEqual(spow, logQ, QQ);
	ring.addAndEqual(bx, spow, QQ);
	delete[] spow;
	ring.CRT(key.rbx, bx, nprimes);
	delete[] bx;
	return key;
}

//------------ encode methods -------------
void MPScheme::encode(Plaintext& plain, double* vals, long n, long logp, long logq) {
	plain.logp = logp;
	plain.logq = logq;
	plain.n = n;
	ring.encode(plain.mx, vals, n, logp + logQ);
}

void MPScheme::encode(Plaintext& plain, complex<double>* vals, long n, long logp, long logq) {
	plain.logp = logp;
	plain.logq = logq;
	plain.n = n;
	ring.encode(plain.mx, vals, n, logp + logQ);
}

complex<double>* MPScheme::decode(Plaintext& plain) {
	complex<double>* res = new complex<double>[plain.n];
	ring.decode(plain.mx, res, plain.n, plain.logp, plain.logq);
	return res;
}

void MPScheme::encodeSingle(Plaintext& plain, double val, long logp, long logq) {
	plain.logp = logp;
	plain.logq = logq;
	plain.n = 1;
	plain.mx[0] = EvaluatorUtils::scaleUpToZZ(val, logp + logQ);
}

void MPScheme::encodeSingle(Plaintext& plain, complex<double> val, long logp, long logq) {
	plain.logp = logp;
	plain.logq = logq;
	plain.n = 1;
	plain.mx[0] = EvaluatorUtils::scaleUpToZZ(val.real(), logp + logQ);
	plain.mx[Nh] = EvaluatorUtils::scaleUpToZZ(val.imag(), logp + logQ);
}

complex<double> MPScheme::decodeSingle(Plaintext& plain) {
	ZZ q = ring.qpows[plain.logq];

	complex<double> res;
	ZZ tmp = plain.mx[0] % q;
	if(NumBits(tmp) == plain.logq) tmp -= q;
	res.real(EvaluatorUtils::scaleDownToReal(tmp, plain.logp));

	tmp = plain.mx[Nh] % q;
	if(NumBits(tmp) == plain.logq) tmp -= q;
	res.imag(EvaluatorUtils::scaleDownToReal(tmp, plain.logp));

	return res;
}
//-----------------------------------------

//---- basic encrypt & decrypt methods ----
void MPScheme::encryptMsg(Ciphertext& cipher, Plaintext& plain, Key& key) {
	cipher.logp = plain.logp;
	cipher.logq = plain.logq;
	cipher.n = plain.n;
	ZZ qQ = ring.qpows[plain.logq + logQ];

	ZZ* vx = new ZZ[N];
	ring.sampleZO(vx);

	long np = ceil((1 + logQQ + logN + 2)/(double)pbnd);
	ring.multNTT(cipher.ax, vx, key.rax, np, qQ);
	ring.addGaussAndEqual(cipher.ax, qQ);

	ring.multNTT(cipher.bx, vx, key.rbx, np, qQ);
	ring.addGaussAndEqual(cipher.bx, qQ);
	delete[] vx;

	ring.addAndEqual(cipher.bx, plain.mx, qQ);

	ring.rightShiftAndEqual(cipher.ax, logQ);
	ring.rightShiftAndEqual(cipher.bx, logQ);
}

void MPScheme::encrypt(Ciphertext& cipher, complex<double>* vals, Key& key, long n, long logp, long logq) {
	Plaintext plain;
	encode(plain, vals, n, logp, logq);
	encryptMsg(cipher, plain, key);
}

void MPScheme::encrypt(Ciphertext& cipher, double* vals, Key& key, long n, long logp, long logq) {
	Plaintext plain;
	encode(plain, vals, n, logp, logq);
	encryptMsg(cipher, plain, key);
}

void MPScheme::decryptMsg(Plaintext& plain, SecretKey& secretKey, Ciphertext& cipher) {
	ZZ q = ring.qpows[cipher.logq];
	plain.logp = cipher.logp;
	plain.logq = cipher.logq;
	plain.n = cipher.n;
	long np = ceil((1 + cipher.logq + logN + 2)/(double)pbnd);
	ring.mult(plain.mx, cipher.ax, secretKey.sx, np, q);
	ring.addAndEqual(plain.mx, cipher.bx, q);
}

complex<double>* MPScheme::decrypt(SecretKey& secretKey, Ciphertext& cipher) {
	Plaintext plain;
	decryptMsg(plain, secretKey, cipher);
	return decode(plain);
}
//-----------------------------------------
//-----------------------------------------
void MPScheme::encryptZeros(Ciphertext& cipher, Key& key, long n, long logp, long logq) {
	encryptSingle(cipher, 0.0, key, logp, logq);
	cipher.n = n;
}

void MPScheme::encryptSingle(Ciphertext& cipher, complex<double> val, Key& key, long logp, long logq) {
	Plaintext plain;
	encodeSingle(plain, val, logp, logq);
	encryptMsg(cipher, plain, key);
}

void MPScheme::encryptSingle(Ciphertext& cipher, double val, Key& key, long logp, long logq) {
	Plaintext plain;
	encodeSingle(plain, val, logp, logq);
	encryptMsg(cipher, plain, key);
}

complex<double> MPScheme::decryptSingle(SecretKey& secretKey, Ciphertext& cipher) {
	Plaintext plain;
	decryptMsg(plain, secretKey, cipher);
	return decodeSingle(plain);
}
//-----------------------------------------

//------ using fixed random vector --------
void MPScheme::encryptMsgWithRV(Ciphertext& cipher, Plaintext& plain, Key& key, ZZ* vx) {
	cipher.logp = plain.logp;
	cipher.logq = plain.logq;
	cipher.n = plain.n;
	ZZ qQ = ring.qpows[plain.logq + logQ];
	long np = ceil((1 + logQQ + logN + 2)/(double)pbnd);
	
	ring.multNTT(cipher.ax, vx, key.rax, np, qQ);
	ring.addGaussAndEqual(cipher.ax, qQ);

	ring.multNTT(cipher.bx, vx, key.rbx, np, qQ);
	ring.addGaussAndEqual(cipher.bx, qQ);
	ring.addAndEqual(cipher.bx, plain.mx, qQ);

	ring.rightShiftAndEqual(cipher.ax, logQ);
	ring.rightShiftAndEqual(cipher.bx, logQ);
}

void MPScheme::encryptWithRV(Ciphertext& cipher, complex<double>* vals, Key& key, long n, long logp, long logq, ZZ* vx) {
	Plaintext plain;
	encode(plain, vals, n, logp, logq);
	encryptMsgWithRV(cipher, plain, key, vx);
}

void MPScheme::encryptWithRV(Ciphertext& cipher, double* vals, Key& key, long n, long logp, long logq, ZZ* vx) {
	Plaintext plain;
	encode(plain, vals, n, logp, logq);
	encryptMsgWithRV(cipher, plain, key, vx);
}
//-----------------------------------------

//----------- mutiple messages ------------
// void MPScheme::encrypt(Ciphertext* ciphers, complex<double>* vals, Key& key, long n, long logp, long logq, long msg_num) {
// 	Plaintext plain;
// 	for (long i=0; i < msg_num; i++) {
// 		encode(plain, (vals+i*n), n, logp, logq);
// 		encryptMsg(ciphers[i], plain, key);
// 	}
// }

// void MPScheme::encrypt(Ciphertext* ciphers, double* vals, Key& key, long n, long logp, long logq, long msg_num) {
// 	Plaintext plain;
// 	for (long i=0; i < msg_num; i++) {
// 		encode(plain, (vals+i*n), n, logp, logq);
// 		encryptMsg(ciphers[i], plain, key);
// 	}
// }

void MPScheme::encrypt(Ciphertexts& ciphers, complex<double>* vals, Key& key, long n, long logp, long logq, long msg_num) {
	ciphers.logp = logp;
	ciphers.logq = logq;
	ciphers.n = n;

	ZZ qQ = ring.qpows[logq + logQ];
	long np = ceil((1 + logQQ + logN + 2)/(double)pbnd);
	ZZ* vx = new ZZ[N];
	ring.sampleZO(vx);
	ring.multNTT(ciphers.ax, vx, key.rax, np, qQ);
	ring.addGaussAndEqual(ciphers.ax, qQ);
	ring.rightShiftAndEqual(ciphers.ax, logQ);

	Plaintext plain;
	for (long i=0; i < msg_num; i++) {
		encode(plain, (vals+i*n), n, logp, logq);
		ring.multNTT((ciphers.bxs+i*N), vx, key.rbx, np, qQ);
		ring.addGaussAndEqual((ciphers.bxs+i*N), qQ);
		ring.addAndEqual((ciphers.bxs+i*N), plain.mx, qQ);
		ring.rightShiftAndEqual((ciphers.bxs+i*N), logQ);
	}
	delete[] vx;
}

void MPScheme::encrypt(Ciphertexts& ciphers, double* vals, Key& key, long n, long logp, long logq, long msg_num) {
	ciphers.logp = logp;
	ciphers.logq = logq;
	ciphers.n = n;

	ZZ qQ = ring.qpows[logq + logQ];
	long np = ceil((1 + logQQ + logN + 2)/(double)pbnd);
	ZZ* vx = new ZZ[N];
	ring.sampleZO(vx);
	ring.multNTT(ciphers.ax, vx, key.rax, np, qQ);
	ring.addGaussAndEqual(ciphers.ax, qQ);
	ring.rightShiftAndEqual(ciphers.ax, logQ);

	Plaintext plain;
	for (long i=0; i < msg_num; i++) {
		encode(plain, (vals+i*n), n, logp, logq);
		ring.multNTT((ciphers.bxs+i*N), vx, key.rbx, np, qQ);
		ring.addGaussAndEqual((ciphers.bxs+i*N), qQ);
		ring.addAndEqual((ciphers.bxs+i*N), plain.mx, qQ);
		ring.rightShiftAndEqual((ciphers.bxs+i*N), logQ);
	}
	delete[] vx;
}

// void MPScheme::encrypt(Ciphertext* ciphers, complex<double>* vals, Key& key, long n, long logp, long logq, long msg_num, ZZ*vx) {
// 	Plaintext plain;
// 	for (long i=0; i < msg_num; i++) {
// 		encode(plain, (vals+i*n), n, logp, logq);
// 		encryptMsgWithRV(ciphers[i], plain, key, vx);
// 	}
// }

// void MPScheme::encrypt(Ciphertext* ciphers, double* vals, Key& key, long n, long logp, long logq, long msg_num, ZZ*vx) {
// 	Plaintext plain;
// 	for (long i=0; i < msg_num; i++) {
// 		encode(plain, (vals+i*n), n, logp, logq);
// 		encryptMsgWithRV(ciphers[i], plain, key, vx);
// 	}
// }

void MPScheme::encrypt(Ciphertexts& ciphers, complex<double>* vals, Key& key, long n, long logp, long logq, long msg_num, ZZ*vx) {
	ciphers.logp = logp;
	ciphers.logq = logq;
	ciphers.n = n;

	ZZ qQ = ring.qpows[logq + logQ];
	long np = ceil((1 + logQQ + logN + 2)/(double)pbnd);
	ring.multNTT(ciphers.ax, vx, key.rax, np, qQ);
	ring.addGaussAndEqual(ciphers.ax, qQ);
	ring.rightShiftAndEqual(ciphers.ax, logQ);

	Plaintext plain;
	for (long i=0; i < msg_num; i++) {
		encode(plain, (vals+i*n), n, logp, logq);
		ring.multNTT((ciphers.bxs+i*N), vx, key.rbx, np, qQ);
		ring.addGaussAndEqual((ciphers.bxs+i*N), qQ);
		ring.addAndEqual((ciphers.bxs+i*N), plain.mx, qQ);
		ring.rightShiftAndEqual((ciphers.bxs+i*N), logQ);
	}
}

void MPScheme::encrypt(Ciphertexts& ciphers, double* vals, Key& key, long n, long logp, long logq, long msg_num, ZZ*vx) {
	ciphers.logp = logp;
	ciphers.logq = logq;
	ciphers.n = n;

	ZZ qQ = ring.qpows[logq + logQ];
	long np = ceil((1 + logQQ + logN + 2)/(double)pbnd);
	ring.multNTT(ciphers.ax, vx, key.rax, np, qQ);
	ring.addGaussAndEqual(ciphers.ax, qQ);
	ring.rightShiftAndEqual(ciphers.ax, logQ);

	Plaintext plain;
	for (long i=0; i < msg_num; i++) {
		encode(plain, (vals+i*n), n, logp, logq);
		ring.multNTT((ciphers.bxs+i*N), vx, key.rbx, np, qQ);
		ring.addGaussAndEqual((ciphers.bxs+i*N), qQ);
		ring.addAndEqual((ciphers.bxs+i*N), plain.mx, qQ);
		ring.rightShiftAndEqual((ciphers.bxs+i*N), logQ);
	}
}

//-----------------------------------------

//------------ mutiple threads ------------
struct EncryptThreadArgs{
	int id;
	long msg_num;
	MPScheme* scheme;
	ZZ* bxs;
	complex<double>* vals;
	long n; 
	long logp; 
	long logq;
	ZZ* bx;
};

void MPScheme::encryptMsgsWithAXRVXRBX(ZZ* bxs, complex<double>* vals, long n, long logp, long logq, long msg_num, ZZ* bx) {
	ZZ qQ = ring.qpows[logq + logQ];
	long np = ceil((1 + logQQ + logN + 2)/(double)pbnd);

	for (long i=0; i < msg_num; i++) {
		Plaintext plain;
		ZZ* bx_tmp = new ZZ[N];
		encode(plain, (vals+i*n), n, logp, logq);
		ring.addAndEqual(bx_tmp, bx, qQ);
		ring.addAndEqual(bx_tmp, plain.mx, qQ);
		ring.addGaussAndEqual(bx_tmp, qQ);
		ring.rightShiftAndEqual(bx_tmp, logQ);
		for (int j=0; j<N; j++){
			bxs[i*N+j] = bx_tmp[j];
		}
		delete[] bx_tmp;
	}
}

void* MPScheme::encryptThread(void *threadarg) {
	EncryptThreadArgs *args = (EncryptThreadArgs*) threadarg;
	int id = args->id;
	long msg_num = args->msg_num;
	long n = args->n;
	long logp = args->logp;
	long logq = args->logq;

	args->scheme->encryptMsgsWithAXRVXRBX(args->bxs, args->vals, n, logp, logq, msg_num, args->bx);
}

void MPScheme::encrypt(Ciphertexts& ciphers, complex<double>* vals, Key& key, long n, long logp, long logq, long msg_num, int thread_num) {
	pthread_t threads[thread_num];
	int error;
	EncryptThreadArgs *tds[thread_num];
	int msg_num_td = msg_num/thread_num;
	ZZ qQ = ring.qpows[logq + logQ];
	long np = ceil((1 + logQQ + logN + 2)/(double)pbnd);

	ciphers.logp = logp;
	ciphers.logq = logq;
	ciphers.n = n;
	delete[] ciphers.bxs;
	ciphers.bxs = new ZZ[N*msg_num];

	ZZ* vx = new ZZ[N];
	ring.sampleZO(vx);
	ring.multNTT(ciphers.ax, vx, key.rax, np, qQ);
	ring.addGaussAndEqual(ciphers.ax, qQ);
	ring.rightShiftAndEqual(ciphers.ax, logQ);

	ZZ* bx = new ZZ[N];
	ring.multNTT(bx, vx, key.rbx, np, qQ);

	ZZ* bxs = new ZZ[N*msg_num];
	for ( int i=0; i < thread_num; i++ ) {
		EncryptThreadArgs* td = new EncryptThreadArgs();
		td->id = i;	
		td->msg_num = msg_num_td;	
		td->n = n; 
		td->logp = logp; 
		td->logq = logq;
		td->scheme = this;
		td->vals = vals+i*msg_num_td*n;
		td->bx = bx;
		td->bxs = ciphers.bxs+i*msg_num_td*N;
		tds[i] = td;
	}
	for ( int i=0; i < thread_num; i++ ) {
		error = pthread_create(&threads[i], NULL, encryptThread, (void *)tds[i]);
		if (error) { cout << "thread [" << i << "] error!" << endl; return; }
		//else{ cout << "thread [" << i << "] done!" << endl; }
	}
  	for ( int i=0; i < thread_num; i++ ) {
 		pthread_join(threads[i], NULL);
	}
	// cout << "all threads done!!!" << endl;
	// pthread_exit(NULL);
	delete[] vx; 
	delete[] bx;

}

void MPScheme::encryptWithRV(Ciphertexts& ciphers, complex<double>* vals, Key& key, long n, long logp, long logq, long msg_num, int thread_num, ZZ* vx) {
	pthread_t threads[thread_num];
	int error;
	EncryptThreadArgs *tds[thread_num];
	int msg_num_td = msg_num/thread_num;
	ZZ qQ = ring.qpows[logq + logQ];
	long np = ceil((1 + logQQ + logN + 2)/(double)pbnd);

	ciphers.logp = logp;
	ciphers.logq = logq;
	ciphers.n = n;
	delete[] ciphers.bxs;
	ciphers.bxs = new ZZ[N*msg_num];

	ring.multNTT(ciphers.ax, vx, key.rax, np, qQ);
	ring.addGaussAndEqual(ciphers.ax, qQ);
	ring.rightShiftAndEqual(ciphers.ax, logQ);

	ZZ* bx = new ZZ[N];
	ring.multNTT(bx, vx, key.rbx, np, qQ);

	ZZ* bxs = new ZZ[N*msg_num];
	for ( int i=0; i < thread_num; i++ ) {
		EncryptThreadArgs* td = new EncryptThreadArgs();
		td->id = i;	
		td->msg_num = msg_num_td;	
		td->n = n; 
		td->logp = logp; 
		td->logq = logq;
		td->scheme = this;
		td->vals = vals+i*msg_num_td*n;
		td->bx = bx;
		td->bxs = ciphers.bxs+i*msg_num_td*N;
		tds[i] = td;
	}
	for ( int i=0; i < thread_num; i++ ) {
		error = pthread_create(&threads[i], NULL, encryptThread, (void *)tds[i]);
		if (error) { cout << "thread [" << i << "] error!" << endl; return; }
		//else{ cout << "thread [" << i << "] done!" << endl; }
	}
  	for ( int i=0; i < thread_num; i++ ) {
 		pthread_join(threads[i], NULL);
	}
	delete[] bx;
}
//-----------------------------------------

//----------- secret key shares -----------
//-- share
// void MPScheme::shamir_shares(uint64_t* shares, uint64_t ss, uint64_t* x, uint64_t mod, long quantity, long threshold, bool print_flag){
// 	uint64_t* coeffs = new uint64_t[threshold-1];
// 	uint64_t min_rand = pow(2, floor(log2(mod)-5));
// 	if(print_flag){
// 		cout << "mod: " << mod << ", log2(mod):" << log2(mod) << ", min_rand:" << min_rand << endl;
// 	}
// 	coeffs[0] = ss;
// 	for(long k=1; k<threshold-1; k++){
// 		coeffs[k] = (uint64_t) ((rand()+min_rand)%mod);
// 		// coeffs[k] = 5;
// 	}
// 	for(long i=0; i<quantity; i++){
// 		uint64_t share = ss;
// 		if(print_flag){
// 			cout << "i:" << i << endl;
// 			cout << "ss:" << ss << endl;
// 		}
// 		for(long k=1; k<threshold-1; k++){
// 			// ZZ tmp;
// 			uint64_t tmp = MulMod(coeffs[k], (uint64_t)pow(x[i],k), mod);
// 			share = AddMod(share, tmp, mod);
// 			if(print_flag){
// 				cout << "coeffs["<<k<<"]: " << coeffs[k] << ", pow(x[i],k):" << pow(x[i],k) 
// 					<< ", share += " << tmp << ", share = " << share << endl;
// 			}
// 		}
// 		shares[i] = (long) share;
// 		if(print_flag){
// 			cout << "shares[" << i << "]: " << shares[i] << endl;
// 		}
// 	}
// 	delete[] coeffs;
// 	// cout<< "coeffs:" << coeffs[0] << ", " << coeffs[1] << endl;
// }

void MPScheme::shamir_shares(uint64_t* shares, uint64_t ss, uint64_t* x, uint64_t mod, long quantity, long threshold){
	uint64_t* coeffs = new uint64_t[threshold];
	uint64_t min_rand = pow(2, floor(log2(mod)-5));
	coeffs[0] = ss;
	for(long k=1; k<threshold; k++){
		coeffs[k] = (uint64_t) ((rand()+min_rand)%mod);
	}
	for(long i=0; i<quantity; i++){
		shares[i] = ss;
		for(long k=1; k<threshold; k++){
			uint64_t tmp = MulMod(coeffs[k], PowerMod(x[i],k,mod), mod);
			shares[i] = AddMod(shares[i], tmp, mod);
		}
	}
	delete[] coeffs;
}

void MPScheme::shareSecret(SKShares& skshares, ZZ* sx, long logq, long quantity, long threshold){
	long np = ceil((1 + logq + logN + 2)/(double)pbnd);
	// SKShares skshares = SKShares(quantity, np);
	skshares.size = quantity;
	skshares.np = np;
	skshares.indexs = new uint64_t[quantity];
	for(long i=0; i<quantity; i++){
		skshares.indexs[i] = i+1;
	}
	skshares.rsx_shares = new uint64_t[N*np*quantity];
	uint64_t* rsx = new uint64_t[N*np];
    ring.CRT(rsx, sx, np);
    uint64_t* shares_tmp = new uint64_t[quantity];
    long Nnp = N*np;
    for(long k=0; k < np; k++){
    	uint64_t mod = ring.multiplier.pVec[k];
    	for(long i=0; i < N; i++){
    		long inx = k*N+i;
    		shamir_shares(shares_tmp, rsx[inx], skshares.indexs, mod, quantity, threshold);
    		for(long j=0; j<quantity; j++){
    			skshares.rsx_shares[j*Nnp+inx] = shares_tmp[j];
    		}
    	}
    }
    delete[] rsx;
    delete[] shares_tmp;
    // cout<< "rsx:" << rsx[0] << ", " << rsx[1] << ", " << rsx[2] << ", " << rsx[3] << ", " << rsx[4] << endl;
    // // cout<< "mod:" << ring.multiplier.pVec[0] << ", " << ring.multiplier.pVec[1] << ", " << ring.multiplier.pVec[2] << endl;
    // cout<< "skshares.rsx_shares[0]:" << skshares.rsx_shares[0] << ", " << skshares.rsx_shares[1] << endl;

}


void MPScheme::shareSecretKey(SKShares& skshares, SecretKey& sk, long logq, long quantity, long threshold){
	shareSecret(skshares, sk.sx, logq, quantity, threshold);
}

//-- interpolate 
// using ZZ
// uint64_t MPScheme::shares_interpolate(uint64_t* x, uint64_t* y, uint64_t mod_o, long threshold){
//     ZZ sum = ZZ(0);
//     ZZ mod = ZZ(mod_o);
//     for (long i = 0; i < threshold; i++) {
//     	ZZ prod = ZZ(1);
//     	ZZ div = ZZ(1);
//     	bool abs_flag = false;
//         for (long j = 0; j < threshold; j++) {
//             if (x[i] != x[j]) {
//             	if (0>x[j]){
//             		prod = MulMod(prod, (0-x[j]), mod);
//             	}
//             	else{
//             		prod = MulMod(mul, x[j], mod);
//             		prod = mod - prod;
//             	}
//             	if (x[i]>x[j]) {
//             		div = MulMod(div, (x[i]-x[j]), mod);
//             	}
//             	else{
//             		div = MulMod(div, (x[j]-x[i]), mod);
//             		div = mod - div;
//             	}
//             }
//         }
//         prod = MulMod(prod, y[i], mod);
//         prod = MulMod(prod, PowerMod(ZZ(div), mod-2, mod), mod);
//         sum = AddMod(sum, prod, mod);
//     }
//     return (uint64_t) to_long(sum);
// }

uint64_t MPScheme::shares_interpolate(uint64_t* x, uint64_t* y, uint64_t mod, long threshold){
    uint64_t sum = 0;
    for (long i = 0; i < threshold; i++) {
    	uint64_t prod = 1;
    	uint64_t div = 1;
        for (long j = 0; j < threshold; j++) {
            if (x[i] != x[j]) {
            	if (0>x[j]){
            		prod = MulMod(prod, (0-x[j]), mod);
            		
            	}
            	else{
            		prod = MulMod(prod, x[j], mod);
            		prod = mod - prod;
            	}
            	if (x[i]>x[j]) {
            		div = MulMod(div, (x[i]-x[j]), mod);
            	}
            	else{
            		div = MulMod(div, (x[j]-x[i]), mod);
            		div = mod - div;
            	}
            }
        }
        prod = MulMod(prod, y[i], mod);
        prod = MulMod(prod, PowerMod(div, mod-2, mod), mod);
        sum = AddMod(sum, prod, mod);
    }
    return (uint64_t) sum;
}


void MPScheme::recoverSecret(uint64_t* rsx, SKShares& skshares, long threshold){
	long np = skshares.np;
	long Nnp = N*np;
	if (threshold<=0){
		threshold = skshares.size;
	}

	uint64_t* values = new uint64_t[threshold];
	for(long  k=0; k < np; k++){
    	uint64_t mod = ring.multiplier.pVec[k];
    	for(long i=0; i < N; i++){
    		long inx = k*N+i;
    		for(long j=0; j < threshold; j++){
    			values[j] = skshares.rsx_shares[j*Nnp+inx];
    		}
    		// bool print_flag = false;
    		// if (k==0 && i==0){
    		// 	print_flag=true;
    		// }
    		rsx[inx] = shares_interpolate(skshares.indexs, values, mod, threshold);
    	}
    }
    delete[] values;
}

void MPScheme::recoverSecret(uint64_t* rsx, SKShares& skshares){
	recoverSecret(rsx, skshares, skshares.size);
}


void MPScheme::recoverSecret(ZZ* sx, long logq, SKShares& skshares, long threshold){
	long np = skshares.np;
	ZZ q = ring.qpows[logq];
	if (threshold<=0){
		threshold = skshares.size;
	}
	uint64_t* rsx = new uint64_t[N*np];
	recoverSecret(rsx, skshares, threshold);
	ring.multiplier.reconstruct(sx, rsx, np, q);
	delete[] rsx;
}

void MPScheme::recoverSecret(ZZ* sx, long logq, SKShares& skshares){
	recoverSecret(sx, logq, skshares, skshares.size);
}

//-----------------------------------------







