#include <stdio.h>

#include "relic.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "MKLHS.h"


#define S	2			/* Number of signers. */
#define L	2			/* Number of labels. */

int main(int argc, char *argv[]) {

	bn_t m, n, msg[S][L], sk[S], d[S], x[S][L];
	g1_t _r, h, as[S], cs[S], sig[S];
	g1_t a[S][L], c[S][L], r[S][L];
	g2_t _s, s[S][L], pk[S], y[S], z[S];
	gt_t *hs[S], vk;

	const char *data = "database-identifier";
	const char *id[S] = { "Gillian", "Charlene" };
	dig_t ft[S], *f[S] = { NULL };
	size_t flen[S];
	char *ls[L] = { NULL };

	bn_null(m); bn_null(n);
	g1_null(h); g1_null(_r);
	g2_null(_s); gt_null(vk);

	if (core_init() != RLC_OK) {
		core_clean();
		return 1;
	}
	if (pc_param_set_any() != RLC_OK) {
		core_clean;
		return 1;
	}
	RLC_TRY {
		bn_new(m); bn_new(n);
		g1_new(h); g1_new(_r); 
		g2_new(_s); gt_new(vk);

		for (int i = 0; i < S; i++) {
			hs[i] = RLC_ALLOCA(gt_t, RLC_TERMS);
			for (int j = 0; j < RLC_TERMS; j++) {
				gt_null(hs[i][j]);
				gt_new(hs[i][j]);
			}
			for (int j = 0; j < L; j++) {
				bn_null(x[i][j]); bn_null(msg[i][j]);
				g1_null(a[i][j]); g1_null(c[i][j]); g1_null(r[i][j]); g2_null(s[i][j]); 
				bn_new(x[i][j]); bn_new(msg[i][j]);
				g1_new(a[i][j]); g1_new(c[i][j]); g1_new(r[i][j]); g2_new(s[i][j]);
			}
			bn_null(sk[i]); bn_null(d[i]);
			g1_null(sig[i]); g1_null(as[i]); g1_null(cs[i]);
			g2_null(y[i]); g2_null(z[i]); g2_null(pk[i]);

			bn_new(sk[i]); bn_new(d[i]); 
			g1_new(sig[i]); g1_new(as[i]); g1_new(cs[i]);
			g2_new(y[i]); g2_new(z[i]); g2_new(pk[i]);
		}
		
		printf("simple linear multi-key homomorphic signature is correct.\n");

		/* Initialize scheme for messages of single components. */
		g1_get_ord(n);
		printf("n :"); bn_print(n);

		


		Scheme scheme;

		PublicKey pkey[S];
		SecretKey skey[S];
		MKPublicKey mkpk = MKPublicKey(S);

		Message msgs[S];
		MKMessage mkmsg = MKMessage(S, L);

		Linearity lfs[S];
		MKLinearity mklf = MKLinearity(S);

		Signature signs[S];
		MKSignature mksign = MKSignature(S);

		/* Initialize and generate msgs & keys & fs & signs for signers. */
		for (int j = 0; j < S; j++) {
			// printf("id : %s\n", id[j]);
			scheme.keyGen(skey[j], pkey[j], id[j]);
			
			// msgs[j]->resetMsg(L);
			// msgs[j]->size = L;
			// msgs[j]->msgs = (bn_t*)malloc(sizeof(bn_t)*L);
			// Message msg_tmp = Message(L);
			msgs[j] = Message(L);
			lfs[j] = Linearity(L);
			signs[j] = Signature(L);

			for (int l = 0; l < L; l++) {
				// cout << 1 << endl;
				msgs[j].setMsg((j+1)*11*(l+1), l, scheme.params);
				// mkmsg.mkmsgs.at(j)->setMsg((j+1)*11*(l+1), l, scheme.params);
				lfs[j].setCoeff(l+1, l);
			}

			scheme.signMsgs(signs[j], msgs[j], skey[j]);
			mkpk.setPKs(pkey[j], j);
			mkmsg.setMsgs(msgs[j], j);
			mklf.setCoeffs(lfs[j], j);
			mksign.setSigns(signs[j], j);
			// cout << "sk: "; skey[j].printKey();
			// msgs[j]->printMsgs();
			// signs[j]->printSigns();
		}

		for (int j = 0; j < S; j++) {
			printf("id : %s\n", id[j]);
			mkmsg.mkmsgs.at(j)->printMsgs();
			mksign.mksigns.at(j)->printSigns();
		}

		Signature sign_res;
		scheme.evalSigns(sign_res, mksign, mklf);
		cout << "sign_res: " << endl;
		sign_res.printSigns();


		data = scheme.params.dataset.c_str();
		for (int i = 0; i < L;i++){
			string label = to_string(i);
			label.insert(0, scheme.params.label);
			ls[i] = (char*)malloc(sizeof(char)*strlen(label.c_str()));
			memcpy(ls[i], label.c_str(), strlen(label.c_str()));
		}


		/* Initialize and generate keys for signers. */
		for (int j = 0; j < S; j++) {
			printf("id : %s\n", id[j]);
			bn_copy(sk[j], skey[j].sk);
			g2_copy(pk[j], pkey[j].pk);
			cout << "sk: "; bn_print(sk[j]);

			for (int l = 0; l < L; l++) {
				bn_copy(msg[j][l], mkmsg.mkmsgs[j]->msgs[l]);
				cp_mklhs_sig(a[j][l], msg[j][l], data, id[j], ls[l], sk[j]);
				cout << "msg[" << l << "]: "; bn_print(msg[j][l]);
				cout << "sig[" << l << "]: "; g1_print(a[j][l]);
			}
		}

		/* Define linear function. */
		for (int i = 0; i < S; i++) {
			f[i] = RLC_ALLOCA(dig_t, RLC_TERMS);
			for (int j = 0; j < RLC_TERMS; j++) {
				f[i][j] = mklf.fs[i]->f[j];
			}
			flen[i] = L;
		}

		/* Apply linear function over signatures. i.e., _r=F(a_j) */
		g1_set_infty(_r);
		for (int j = 0; j < S; j++) {
			cp_mklhs_evl(r[0][j], a[j], f[j], L);
			g1_add(_r, _r, r[0][j]);
		}
		g1_norm(_r, _r);
		cout << "_r: " << endl;
		g1_print(_r);


		/* Apply linear function over messages. i.e., d_j=f_j(m_j) */
		for (int j = 0; j < S; j++) {
			cp_mklhs_fun(d[j], msg[j], f[j], L);
			printf("m[%d] :\n", j);
			for (int l = 0; l < L; l++) { printf("\t"); bn_print(msg[j][l]); }
			printf("f[%d] :\n\t%d", j, f[j][0]);
			for (int l = 1; l < L; l++) { printf(", %d", f[j][l]); }
			printf("\nd[%d] :", j); bn_print(d[j]);
		}

		/* Apply linear function over messages. i.e., m=F(m_j) */
		bn_zero(m);
		for (int j = 0; j < S; j++) {
			for (int l = 0; l < L; l++) {
				bn_mul_dig(msg[j][l], msg[j][l], f[j][l]);
				bn_add(m, m, msg[j][l]);
				bn_mod(m, m, n);
			}
		}

		/* Evaluate signature and message. */
		printf("eval..\n");
		int ver_res = cp_mklhs_ver(_r, m, d, data, id, (const char **)ls, (const dig_t **)f, flen, pk, S);
		printf("res: %d \n", ver_res);

		/* test. */
		f[0][0] = 2;
		printf("change & eval..\n");
		ver_res = cp_mklhs_ver(_r, m, d, data, id, (const char **)ls, (const dig_t **)f, flen, pk, S);
		printf("res: %d \n", ver_res);

		cp_mklhs_off(as, ft, id, (const char **)ls, (const dig_t **)f, flen, S);
		cp_mklhs_onv(_r, m, d, data, id, as, ft, pk, S);


	} RLC_CATCH_ANY {
		RLC_THROW(ERR_CAUGHT);
	} RLC_FINALLY {
		bn_free(n); bn_free(m);
		g1_free(h); g1_free(_r);
		g2_free(_s); gt_free(vk);

		for (int i = 0; i < S; i++) {
			RLC_FREE(f[i]);
			for (int j = 0; j < RLC_TERMS; j++) {
				  gt_free(hs[i][j]);
			}
			RLC_FREE(hs[i]);
			for (int j = 0; j < L; j++) {
				bn_free(x[i][j]); bn_free(msg[i][j]);
				g1_free(a[i][j]); g1_free(c[i][j]);
				g1_free(r[i][j]); g2_free(s[i][j]);
			}
			bn_free(sk[i]); bn_free(d[i]);
			g1_free(sig[i]); g1_free(as[i]); g1_free(cs[i]);
			g2_free(y[i]); g2_free(z[i]); g2_free(pk[i]);
		}
	}

	core_clean();
	return 0;
}
