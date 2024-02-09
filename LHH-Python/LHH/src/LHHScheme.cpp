#include "LHHScheme.h"

LHHScheme::LHHScheme(int d){
	this->d = d;
	ctx = BN_CTX_new();

	curve = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
	g0 = EC_POINT_dup(EC_GROUP_get0_generator(curve), curve);
	g.resize(d);
	for (int i = 0; i < d; ++i)
		g[i] = EC_POINT_new(curve);

	q = BN_new();
	EC_GROUP_get_order(curve, q, ctx);

	unsigned char ciphertext[32];
	int clen;

	// Use AES-CTR mode as PRG
	BIGNUM *x = BN_new();
	int lsb = 0;
	EVP_CIPHER_CTX *cipher_ctx = EVP_CIPHER_CTX_new();
	EVP_EncryptInit_ex(cipher_ctx, EVP_aes_256_ctr(), NULL,
		_homhash_seed_key, _homhash_seed_iv);

	for (int i = 0; i < d; ++i)
	{
		do 	// Loop until find d different points (with overwhelming probability)
		{
			EVP_EncryptUpdate(cipher_ctx, ciphertext, &clen, _homhash_seed_plaintext, 32);
			BN_bin2bn(ciphertext, 32, x);
		} while (EC_POINT_set_compressed_coordinates_GFp(curve,
			g[i], x, lsb, ctx) == 0);
	}

	EVP_CIPHER_CTX_free(cipher_ctx);
	BN_free(x);
}

LHHScheme::~LHHScheme(){
	BN_free(q);
	for (int i = 0; i < d; ++i)
		EC_POINT_free(g[i]);
	EC_POINT_free(g0);
	EC_GROUP_free(curve);
	BN_CTX_free(ctx);
}

// // get val_len
// int LHHScheme::getValLen(){
// 	return d;
// }

// get mod
string LHHScheme::getModStr(){
	return BN_bn2dec(q);
}

// get & print hashes
string LHHScheme::getHashesHex(const LHHHash& hash, int i){
	return hash.getVal(i, curve, ctx);
}

string LHHScheme::getHashesHex(const LHHHash& hash){
	return hash.getVal(curve, ctx);
}

void LHHScheme::printHash(const LHHHash& hash, int i){
	hash.printVal(i, curve, ctx);
}

void LHHScheme::printHash(const LHHHash& hash){
	hash.printVal(curve, ctx);
}

// set hashes
void LHHScheme::resetHashes(LHHHash& hash, int val_len){
	hash.reset(val_len, curve);
}

void LHHScheme::setHashes(LHHHash& hash, const LHHHash& hash_single, int i){
	if (hash.val.size() <= i)
		hash.resize(i+1, curve);
	EC_POINT_copy(hash.val[i], hash_single.val[0]);
}

void LHHScheme::setHashes(LHHHash& hash, string hash_hex_str, int i){
	hash.setVal(hash_hex_str, i, curve, ctx);
}

int LHHScheme::cmpHash(const LHHHash& hash1, const LHHHash& hash2){
	if (hash1.val.size() != hash2.val.size())
		return 0;
	for(int i = 0; i<hash1.val.size(); i++){
		if( EC_POINT_cmp(curve, hash1.val[i], hash2.val[i], ctx) != 0 ){
			// cout << "hash1:" << EC_POINT_point2hex(curve, hash1.val[i], POINT_CONVERSION_UNCOMPRESSED, ctx) << endl;
			// cout << "hash2:" << EC_POINT_point2hex(curve, hash2.val[i], POINT_CONVERSION_UNCOMPRESSED, ctx) << endl;
			return 0;
		}
	}
	return 1;
}


void LHHScheme::Hash(EC_POINT *hash,const vector<BIGNUM *> &vec){
	EC_POINT *temp = EC_POINT_new(curve);
	assert( vec.size() <= d );
	for (int i = 0; i < vec.size(); ++i){
		EC_POINT_mul(curve, temp, NULL, g[i], vec[i], ctx);
		EC_POINT_add(curve, hash, hash, temp, ctx);
	}
	EC_POINT_free(temp);
}

void LHHScheme::getHash(LHHHash& hash, const LHHVal& msg){
	hash.reset(1, curve);
	Hash(hash.val[0], msg.val);
	// cout << "hash:" << EC_POINT_point2hex(curve, hash.val[0], POINT_CONVERSION_UNCOMPRESSED, ctx) << endl;
}

void LHHScheme::Eval(EC_POINT *res, const vector<EC_POINT *> &hashes, const vector<BIGNUM *> &alphas){
	EC_POINT *temp = EC_POINT_new(curve);
	size_t l = hashes.size();
	for (int i = 0; i < l; ++i){
		EC_POINT_mul(curve, temp, NULL, hashes[i], alphas[i], ctx);
		EC_POINT_add(curve, res, res, temp, ctx);
	}
	EC_POINT_free(temp);
}

void LHHScheme::evalHashes(LHHHash& hash_agg, const LHHHash& hashes, const LHHVal& alphas){
	hash_agg.reset(1, curve);
	Eval(hash_agg.val[0], hashes.val, alphas.val);
	// cout << "hash_agg in c: "; printHash(hash_agg);
}


void LHHScheme::evalMsgs(LHHVal& agg_msg, const LHHMsgs& msgs, const LHHVal& alphas){
	int msgs_num = msgs.msgs_num;
	int val_len = msgs.val_len;
	assert( alphas.val.size() == msgs_num );

	agg_msg.reset(val_len);
	for (int i = 0; i < msgs_num; ++i){
		for (int j = 0; j < val_len; ++j){
			BIGNUM * tmp = BN_new();
			BN_mod_mul(tmp, msgs.msgs[i].val[j], alphas.val[i], q, ctx);
			BN_mod_add(agg_msg.val[j], agg_msg.val[j], tmp, q, ctx);
		}
	}
}

int LHHScheme::verHashes(const LHHHash& hash_agg, const LHHMsgs& msgs, const LHHVal &alphas){
	LHHVal agg_msg = LHHVal();
	evalMsgs(agg_msg, msgs, alphas);

	LHHHash ver_agg_hash;
	getHash(ver_agg_hash, agg_msg);

	return cmpHash(hash_agg, ver_agg_hash);
}