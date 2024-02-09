#ifndef MKLHS_SCHEME_H_
#define MKLHS_SCHEME_H_


#include "relic.h"
#include "Params.h"
#include "SecretKey.h"
#include "PublicKey.h"
#include "Message.h"
#include "Signature.h"
#include "Linearity.h"
#include <iostream>
using namespace std;


class LHSScheme {
public:
	Params params;

	LHSScheme();
	LHSScheme(const Params& o);

	void keyGen(SK& sk, PK& pk);
	void keyGen(SK& sk, PK& pk, string id);

	void signMsgs(Signature& signs, Message& msgs, SK& sk);
	void signMsgs(Signature& signs, Message& msgs, SK& sk, string id);

	void evalSign(Signature& sign_res, Signature& sign, Linearity& f);
	void evalSigns(Signature& signs_res, MKSignature& signs, MKLinearity& fs);

	void evalMsg(Message& msg_res, Message& msg, Linearity& f);
	void evalMsgs(Message& msg_res, MKMessage& msgs, MKLinearity& fs);
	void evalMsgs(Message& msg_res, bn_t* msg_ress, MKMessage& msgs, MKLinearity& fs);

	int verMKLHS(Signature& sign_res, MKSignature& signs, MKMessage& msgs, MKLinearity& fs, MKPK& pks);
	int verMKLHS(Signature& sign_res, MKMessage& msgs, MKLinearity& fs, MKPK& pks);
	
	// virtual ~LHSScheme();
};
#endif