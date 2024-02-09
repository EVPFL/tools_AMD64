/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#ifndef HEAAN_MPSCHEME_H_
#define HEAAN_MPSCHEME_H_

#include <NTL/RR.h>
#include <NTL/ZZ.h>
#include <complex>
#include <cstdlib>
#include <math.h>
#include <vector>

#include <NTL/tools.h>
#include <cmath>

#include "BootContext.h"
#include "SecretKey.h"
#include "Ciphertext.h"
#include "Plaintext.h"
#include "Key.h"
#include "EvaluatorUtils.h"
#include "Ring.h"
#include "Scheme.h"

using namespace std;
using namespace NTL;

class MPScheme {
private:

public:
	Ring& ring;

	MPScheme(Ring& ring);

	virtual ~MPScheme();


	//----------------------------------------------------------------------------------
	//   KEYS GENERATION
	//----------------------------------------------------------------------------------

	Key getKey(SecretKey& secretKey, long key_flag);
	Key getKey(SecretKey& secretKey, long key_flag, long r); 

	Key getEncKey(SecretKey& secretKey);
	Key getEncKey(SecretKey& secretKey, ZZ* ax);
	Key getEncKey(SecretKey& secretKey, uint64_t* rax);

	Key getMultKey(SecretKey& secretKey);
	Key getMultKey(SecretKey& secretKey, ZZ* ax);
	Key getMultKey(SecretKey& secretKey, uint64_t* rax);


	Key getConjKey(SecretKey& secretKey);

	Key getLeftRotKey(SecretKey& secretKey, long r);
	Key getLeftRotKey(SecretKey& secretKey, long r, ZZ* ax);
	Key getLeftRotKey(SecretKey& secretKey, long r, uint64_t* rax);

	Key getRightRotKey(SecretKey& secretKey, long r);

	Key* getLeftRotKeys(SecretKey& secretKey);

	Key* getRightRotKeys(SecretKey& secretKey);


	//----------------------------------------------------------------------------------
	//   ENCODING & DECODING
	//----------------------------------------------------------------------------------

	void encode(Plaintext& plain, complex<double>* vals, long n, long logp, long logq);

	void encode(Plaintext& plain, double* vals, long n, long logp, long logq);

	complex<double>* decode(Plaintext& plain);

	void encodeSingle(Plaintext& plain, complex<double> val, long logp, long logq);

	void encodeSingle(Plaintext& plain, double val, long logp, long logq);

	complex<double> decodeSingle(Plaintext& plain);


	//----------------------------------------------------------------------------------
	//   ENCRYPTION & DECRYPTION
	//----------------------------------------------------------------------------------

	//---- basic encrypt & decrypt methods ----
	void encryptMsg(Ciphertext& cipher, Plaintext& plain, Key& key);

	void decryptMsg(Plaintext& plain, SecretKey& secretKey, Ciphertext& cipher);

	void encrypt(Ciphertext& cipher, complex<double>* vals, Key& key, long n, long logp, long logq);

	void encrypt(Ciphertext& cipher, double* vals, Key& key, long n, long logp, long logq);

	void encryptZeros(Ciphertext& cipher, Key& key, long n, long logp, long logq);

	complex<double>* decrypt(SecretKey& secretKey, Ciphertext& cipher);

	void encryptSingle(Ciphertext& cipher, complex<double> val, Key& key, long logp, long logq);

	void encryptSingle(Ciphertext& cipher, double val, Key& key, long logp, long logq);

	complex<double> decryptSingle(SecretKey& secretKey, Ciphertext& cipher);

	// new methods 
	//------ using fixed random vector --------
	void encryptMsgWithRV(Ciphertext& cipher, Plaintext& plain, Key& key, ZZ* vx);
	void encryptWithRV(Ciphertext& cipher, complex<double>* vals, Key& key, long n, long logp, long logq, ZZ* vx);
	void encryptWithRV(Ciphertext& cipher, double* vals, Key& key, long n, long logp, long logq, ZZ* vx) ;

	//----------- mutiple messages ------------
	// using given key (for multiple message, using same key)
	void encrypt(Ciphertexts& ciphers, complex<double>* vals, Key& key, long n, long logp, long logq, long msg_num);
	void encrypt(Ciphertexts& ciphers, double* vals, Key& key, long n, long logp, long logq, long msg_num);
	void encrypt(Ciphertexts& ciphers, complex<double>* vals, Key& key, long n, long logp, long logq, long msg_num, ZZ*vx);
	void encrypt(Ciphertexts& ciphers, double* vals, Key& key, long n, long logp, long logq, long msg_num, ZZ*vx);

	//------------ mutiple threads ------------
	// using given key and vx (for multiple threads, using same key and same vx, and vx is deleted outside of threads to avoid the segmentation fault)
	void encryptMsgsWithAXRVXRBX(ZZ* bxs, complex<double>* vals, long n, long logp, long logq, long msg_num, ZZ* bx);
	static void* encryptThread(void *threadarg);
	void encrypt(Ciphertexts& ciphers, complex<double>* vals, Key& key, long n, long logp, long logq, long msg_num, int thread_num);
	void encryptWithRV(Ciphertexts& ciphers, complex<double>* vals, Key& key, long n, long logp, long logq, long msg_num, int thread_num, ZZ* vx);

	//----------- secret key shares -----------
	// void shamir_shares(uint64_t* shares, uint64_t ss, uint64_t* s, uint64_t mod, long quantity, long threshold);
	void shamir_shares(uint64_t* shares, uint64_t ss, uint64_t* s, uint64_t mod, long quantity, long threshold);
	void shareSecret(SKShares& skshares, ZZ* sx, long logq, long quantity, long threshold);
	void shareSecretKey(SKShares& skshares, SecretKey& sk, long logq, long quantity, long threshold);
	uint64_t shares_interpolate(uint64_t* x, uint64_t* y, uint64_t mod, long threshold);
	void recoverSecret(uint64_t* rsx, SKShares& skshares, long threshold);
	void recoverSecret(uint64_t* rsx, SKShares& skshares);
	void recoverSecret(ZZ* sx, long logq, SKShares& skshares, long threshold);
	void recoverSecret(ZZ* sx, long logq, SKShares& skshares);
};

#endif
