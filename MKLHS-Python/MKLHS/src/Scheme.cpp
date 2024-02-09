#include "Scheme.h"

LHSScheme::LHSScheme() {
}

LHSScheme::LHSScheme(const Params& o) {
	// bn_copy(params.n, o.n); 
	params.setDataset(o.dataset);
	params.setLabel(o.label);
}


void LHSScheme::keyGen(SK& sk, PK& pk) {
	cp_mklhs_gen(sk.sk, pk.pk);
}

void LHSScheme::keyGen(SK& sk, PK& pk, string id) {
	cp_mklhs_gen(sk.sk, pk.pk);
	sk.setID(id);
	pk.setID(id);
}


void LHSScheme::signMsgs(Signature& signs, Message& msgs, SK& sk) {
	signs.size = msgs.size;
	const char* id = sk.id.c_str();
	const char* dataset = params.dataset.c_str();
	for (int i = 0; i < msgs.size; i++){
		string label = to_string(i);
		label.insert(0, params.label);
		cp_mklhs_sig(signs.signs[i], msgs.msgs[i], dataset, id, label.c_str(), sk.sk);
		// cout << "label[" << i <<"]: " << label.c_str() << endl;
		// cout << "msgs[" << i <<"]: "; bn_print(msgs.msgs[i]);
		// cout << "signs[" << i <<"]: "; g1_print(signs.signs[i]);
	}
}

void LHSScheme::signMsgs(Signature& signs, Message& msgs, SK& sk, string id) {
	signs.size = msgs.size;
	const char* dataset = params.dataset.c_str();
	for (int i = 0; i < msgs.size; i++){
		// const char* label = params.label.append(to_string(i)).c_str();
		string label = to_string(i);
		label.insert(0, params.label);
		// cout << "ls[" << i << "]: " << label << endl;
		cp_mklhs_sig(signs.signs[i], msgs.msgs[i], dataset, id.c_str(), label.c_str(), sk.sk);
	}
}


void LHSScheme::evalSign(Signature& sign_res, Signature& sign, Linearity& f) {
	dig_t *coeffs;
	if (f.size < sign.size){
		coeffs = (dig_t*)malloc(sizeof(dig_t)*sign.size);
		for (int i = 0; i < sign.size; i++){
			if (i < f.size) { coeffs[i] = f.f[i]; }
			else {  coeffs[i] = 1; }
		}
	}
	else{
		coeffs = f.f;
	}
	// cp_mklhs_evl(sign_res.signs[0], sign.signs, coeffs, sign.size);
	// int L = 1;
	// int S = 1;
	// g1_t res;
	// g1_null(res); g1_new(res);
	// g1_t a[S][L];
	// g1_null(a[0][0]); g1_new(a[0][0]);
	// dig_t *fo[S];
	// fo[0] = RLC_ALLOCA(dig_t, 1);
	// fo[0][0] = 2;
	// cp_mklhs_evl(res, sign.signs, coeffs, L);
	// cout << "OK!!" << endl;
	cp_mklhs_evl(sign_res.signs[0], sign.signs, coeffs, sign.size);
	// cp_mklhs_evl(sign_res.signs[0], sign.signs, coeffs, sign.size);
	// cout << "OK!!" << endl;
}

void LHSScheme::evalSigns(Signature& sign_res, MKSignature& signs, MKLinearity& fs) {
	g1_set_infty(sign_res.signs[0]);
	for (int j = 0; j < signs.signers_num; j++) {
		Signature sign_res_tmp;
		if (j >= fs.signers_num){
			Linearity f;
			// evalSign(sign_res_tmp, *signs.mksigns.at(j), f);
			evalSign(sign_res_tmp, signs.mksigns[j], f);
		}
		else {
			// evalSign(sign_res_tmp, *signs.mksigns.at(j), *fs.fs.at(j));
			evalSign(sign_res_tmp, signs.mksigns[j], fs.fs[j]);
		}
		g1_add(sign_res.signs[0], sign_res.signs[0], sign_res_tmp.signs[0]);
	}
	g1_norm(sign_res.signs[0], sign_res.signs[0]);
}


void LHSScheme::evalMsg(Message& msg_res, Message& msg, Linearity& f) {
	dig_t *coeffs;
	if (f.size < msg.size){
		coeffs = (dig_t*)malloc(sizeof(dig_t)*msg.size);
		for (int i = 0; i < msg.size; i++){
			if (i < f.size) { coeffs[i] = f.f[i]; }
			else {  coeffs[i] = 1; }
		}
	}
	else{
		coeffs = f.f;
	}
	cp_mklhs_fun(msg_res.msgs[0], msg.msgs, coeffs, msg.size);
}

void LHSScheme::evalMsgs(Message& msg_res, MKMessage& msgs, MKLinearity& fs) {
	bn_zero(msg_res.msgs[0]);
	for (int j = 0; j < msgs.signers_num; j++) {
		Message msg_res_tmp = Message(1);
		if (j >= fs.signers_num){
			Linearity f;
			// evalMsg(msg_res_tmp, *msgs.mkmsgs.at(j), f);
			evalMsg(msg_res_tmp, msgs.mkmsgs[j], f);
		}
		else {
			// evalMsg(msg_res_tmp, *msgs.mkmsgs.at(j), *fs.fs.at(j));
			evalMsg(msg_res_tmp, msgs.mkmsgs[j], fs.fs[j]);
		}
		bn_add(msg_res.msgs[0], msg_res.msgs[0], msg_res_tmp.msgs[0]);
		bn_mod(msg_res.msgs[0], msg_res.msgs[0], params.n);
	}
}


void LHSScheme::evalMsgs(Message& msg_res, bn_t* msg_ress, MKMessage& msgs, MKLinearity& fs) {
	bn_zero(msg_res.msgs[0]);
	for (int j = 0; j < msgs.signers_num; j++) {
		Message msg_res_tmp = Message(1);
		if (j >= fs.signers_num){
			Linearity f;
			// evalMsg(msg_res_tmp, *msgs.mkmsgs.at(j), f);
			evalMsg(msg_res_tmp, msgs.mkmsgs[j], f);
		}
		else {
			// evalMsg(msg_res_tmp, *msgs.mkmsgs.at(j), *fs.fs.at(j));
			evalMsg(msg_res_tmp, msgs.mkmsgs[j], fs.fs[j]);
		}
		bn_copy(msg_ress[j], msg_res_tmp.msgs[0]);
		bn_add(msg_res.msgs[0], msg_res.msgs[0], msg_res_tmp.msgs[0]);
		bn_mod(msg_res.msgs[0], msg_res.msgs[0], params.n);
	}
}


int LHSScheme::verMKLHS(Signature& sign_res, MKSignature& signs, MKMessage& msgs, MKLinearity& fs, MKPK& pks) {
	const char* dataset = params.dataset.c_str();
	int signers_num = signs.signers_num;
	int msg_size = msgs.msgs_num;

	char** ls = (char**)malloc(sizeof(char*)*msg_size);
	for (int i = 0; i < msg_size;i++){
		string label = to_string(i);
		label.insert(0, params.label);
		ls[i] = (char*)malloc(sizeof(char)*strlen(label.c_str()));
		memcpy(ls[i], label.c_str(), strlen(label.c_str()));
		// cout << "ls[" << i << "]: " << ls[i] << endl;
	}

	const char **id = (const char**)malloc(sizeof(char*)*signers_num);
	g2_t* pk = (g2_t*)malloc(sizeof(g2_t)*signers_num);
	const dig_t **f = (const dig_t**)malloc(sizeof(dig_t*)*signers_num);
	size_t* flen = (size_t*)malloc(sizeof(dig_t)*signers_num);
	bn_t* msg_ress = (bn_t*)malloc(sizeof(bn_t)*signers_num);
	
	for (int j = 0; j < signers_num; j++) {
		// strcpy(id[j], pks.mkpks.at(j)->id.c_str());
		// cout << "signer[" << j << "]: "<< endl;
		// id[j] = pks.mkpks.at(j)->id.c_str();
		id[j] = pks.mkpks[j].id.c_str();
		// cout << "  id: " << id[j] << endl;
		g2_null(pk[j]); g2_new(pk[j]);
		g2_copy(pk[j], pks.mkpks[j].pk);
		// g2_copy(pk[j], pks.mkpks.at(j)->pk);
		// cout << "  pk: ";  g2_print(pk[j]);
		// f[j] = fs.fs.at(j)->f;
		// flen[j] = fs.fs.at(j)->size;
		f[j] = fs.fs[j].f;
		flen[j] = fs.fs[j].size;
		// cout << "  flen: " << flen[j] << endl;
		// cout << "  f: " << f[j][0];
		// for (int i = 1; i < msg_size;i++){ cout  << ", " << f[j][i]; }
		// cout << endl;

		bn_null(msg_ress[j]); bn_new(msg_ress[j]);
	}

	Message msg_res = Message(1);
	evalMsgs(msg_res, msg_ress, msgs, fs);
	// cout << "  msg_res: "; msg_res.printMsgs();
	// for (int j = 0; j < signers_num; j++) {
	// 	cout << "  msg_ress[" << j << "]: ";
	// 	bn_print(msg_ress[j]);
	// }

	int ver_res;
	ver_res = cp_mklhs_ver(sign_res.signs[0], msg_res.msgs[0], msg_ress, dataset, id, (const char**)ls, f, flen, pk, signers_num);
	// cout << "  sign_res: "; sign_res.printSigns();
	// cout << "  ver_res: " << ver_res << endl;
	return ver_res;
}


int LHSScheme::verMKLHS(Signature& sign_res, MKMessage& msgs, MKLinearity& fs, MKPK& pks) {
	const char* dataset = params.dataset.c_str();
	int signers_num = msgs.signers_num;
	int msg_size = msgs.msgs_num;
	// cout << "signers_num: " << signers_num;
	// cout << ", msg_size: " << msg_size << endl;

	char** ls = (char**)malloc(sizeof(char*)*msg_size);
	for (int i = 0; i < msg_size;i++){
		string label = to_string(i);
		label.insert(0, params.label);
		ls[i] = (char*)malloc(sizeof(char)*strlen(label.c_str()));
		memcpy(ls[i], label.c_str(), strlen(label.c_str()));
		// cout << "ls[" << i << "]: " << ls[i] << endl;
	}

	const char **id = (const char**)malloc(sizeof(char*)*signers_num);
	g2_t* pk = (g2_t*)malloc(sizeof(g2_t)*signers_num);
	const dig_t **f = (const dig_t**)malloc(sizeof(dig_t*)*signers_num);
	size_t* flen = (size_t*)malloc(sizeof(dig_t)*signers_num);
	bn_t* msg_ress = (bn_t*)malloc(sizeof(bn_t)*signers_num);
	
	for (int j = 0; j < signers_num; j++) {
		// cout << "signer[" << j << "]: "<< endl;
		// id[j] = pks.mkpks.at(j)->id.c_str();
		id[j] = pks.mkpks[j].id.c_str();
		// cout << "  id: " << id[j] << endl;
		g2_null(pk[j]); g2_new(pk[j]);
		// g2_copy(pk[j], pks.mkpks.at(j)->pk);
		g2_copy(pk[j], pks.mkpks[j].pk);
		// cout << "  pk: ";  g2_print(pk[j]);
		// f[j] = fs.fs.at(j)->f;
		// flen[j] = fs.fs.at(j)->size;
		f[j] = fs.fs[j].f;
		flen[j] = fs.fs[j].size;
		// cout << "  flen: " << flen[j] << endl;
		// cout << "  f: " << f[j][0];
		// for (int i = 1; i < msg_size;i++){ cout  << ", " << f[j][i]; }
		// cout << endl;
		bn_null(msg_ress[j]); bn_new(msg_ress[j]);
	}

	Message msg_res = Message(1);
	evalMsgs(msg_res, msg_ress, msgs, fs);
	// cout << "  msg_res: "; msg_res.printMsgs();
	// for (int j = 0; j < signers_num; j++) {
	// 	cout << "  msg_ress[" << j << "]: ";
	// 	bn_print(msg_ress[j]);
	// }

	int ver_res;
	ver_res = cp_mklhs_ver(sign_res.signs[0], msg_res.msgs[0], msg_ress, dataset, id, (const char**)ls, f, flen, pk, signers_num);
	return ver_res;
}



// int LHSScheme::verMKLHS(Signature& sign_res, MKMessage& msgs, MKLinearity& fs, MKPK& pks) {
// 	const char* dataset = params.dataset.c_str();
// 	int signers_num = msgs.signers_num;
// 	int msg_size = msgs.msgs_num;

// 	char** ls = (char**)malloc(sizeof(char*)*msg_size);
// 	for (int i = 0; i < msg_size;i++){
// 		string label = to_string(i);
// 		label.insert(0, params.label);
// 		ls[i] = (char*)malloc(sizeof(char)*strlen(label.c_str()));
// 		memcpy(ls[i], label.c_str(), strlen(label.c_str()));
// 	}

// 	const char **id = (const char**)malloc(sizeof(char*)*signers_num);
// 	g2_t* pk = (g2_t*)malloc(sizeof(g2_t)*signers_num);
// 	const dig_t **f = (const dig_t**)malloc(sizeof(dig_t*)*signers_num);
// 	size_t* flen = (size_t*)malloc(sizeof(dig_t)*signers_num);
// 	bn_t* msg_ress = (bn_t*)malloc(sizeof(bn_t)*signers_num);
	
// 	for (int j = 0; j < signers_num; j++) {
// 		id[j] = pks.mkpks[j].id.c_str();
// 		g2_null(pk[j]); g2_new(pk[j]);
// 		g2_copy(pk[j], pks.mkpks[j].pk);
// 		f[j] = fs.fs[j].f;
// 		flen[j] = fs.fs[j].size;
// 		bn_null(msg_ress[j]); bn_new(msg_ress[j]);
// 	}
// 	Message msg_res = Message(1);
// 	evalMsgs(msg_res, msg_ress, msgs, fs);

// 	int ver_res = 0;
// 	int imax = 0, lmax = 0, fmax = 0;
// 	for (int i = 0; i < signers_num; i++) {
// 		fmax = RLC_MAX(fmax, flen[i]);
// 		imax = RLC_MAX(imax, strlen(id[i]));
// 		for (int j = 0; j < flen[i]; j++) {
// 			lmax = RLC_MAX(lmax, strlen(ls[j]));
// 		}
// 	}
// 	g1_t d, *g = RLC_ALLOCA(g1_t, signers_num);
// 	g1_t *h = RLC_ALLOCA(g1_t, fmax);
// 	g1_null(d); g1_new(d);
// 	gt_t c; gt_null(c); gt_new(c);
// 	gt_t e; gt_null(e); gt_new(e);
// 	g2_t g2; g2_null(g2); g2_new(g2);
// 	uint8_t *str = RLC_ALLOCA(uint8_t, imax + RLC_MAX(strlen(dataset), lmax));
// 	if (g == NULL || h == NULL || str == NULL) {
// 		RLC_FREE(g);
// 		RLC_FREE(h);
// 		RLC_THROW(ERR_NO_MEMORY);
// 	}
// 	for (int j = 0; j < fmax; j++) {
// 			g1_null(h[j]);
// 			g1_new(h[j]);
// 		}
// 	for (int i = 0; i < signers_num; i++) {
// 		memcpy(str, dataset, strlen(dataset));
// 		memcpy(str + strlen(dataset), id[i], strlen(id[i]));
// 		g1_map(d, str, strlen(dataset) + strlen(id[i]));
// 		memcpy(str, id[i], strlen(id[i]));
// 		for (int j = 0; j < flen[i]; j++) {
// 			memcpy(str + strlen(id[i]), ls[j], strlen(ls[j]));
// 			g1_map(h[j], str, strlen(id[i]) + strlen(ls[j]));
// 			g1_add(h[j], h[j], d);
// 		}
// 		g1_norm_sim(h, h, signers_num);
// 		g1_mul_sim_dig(g[i], h, f[i], flen[i]);
// 		g1_mul_gen(h[0], msg_ress[i]);
// 		g1_add(g[i], g[i], h[0]);
// 	}
// 	g1_norm_sim(g, g, signers_num);
// 	g2_get_gen(g2);
// 	pc_map(e, sign_res.signs[0], g2);
// 	pc_map_sim(c, g, pk, signers_num);
// 	if (gt_cmp(c, e) == RLC_EQ) {
// 		ver_res = 1;
// 	}

// 	return ver_res;
// }



// LHSScheme::~LHSScheme() {
// 	// params.free();
// }
