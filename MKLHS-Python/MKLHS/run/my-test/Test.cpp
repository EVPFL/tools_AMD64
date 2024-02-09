#include "relic.h"
#include "MKLHS.h"

#include <iostream>
using namespace std;

int main()
{
    cout << "Test" << endl;

    if (core_init() != RLC_OK) {
		core_clean();
		return 0;
	}
	if (pc_param_set_any() != RLC_OK) {
		core_clean();
		return 0;
	}

	g1_t a;
	g1_null(a); g1_new(a);

	bn_t msg;
	bn_null(msg); bn_new(msg);
	bn_set_dig(msg, 100);
	char *data = "database-identifier";
	char *id = "Charlene";
	char *label = "l";
	bn_t sk1;
	bn_null(sk1); bn_new(sk1);
	g2_t pk1;
	g2_null(pk1); g2_new(pk1);
	cp_mklhs_gen(sk1, pk1);
	cp_mklhs_sig(a, msg, data, id, label, sk1);

	g1_print(a);

	cout << "-- Test --" << endl;

	LHSScheme scheme;
	SK sk;
	PK pk;
	scheme.keyGen(sk, pk, id);
	cout << "sk:" << endl;
	sk.printKey();
	cout << "pk:" << endl;
	pk.getKeyStr();

	Message msgs = Message(1);
	msgs.setMsg("e9f40b6a45f72b7f7ccd5b72805d118fba35dfc624b29fd7c91f0a9ef555c47d19ed18fb54c", 0, scheme.params);
	cout << "msg:" << endl;
	msgs.printMsgs();

	Signature signs = Signature(1);
	scheme.signMsgs(signs, msgs, sk);
	// g1_null(signs.signs[0]);
	// g1_new(signs.signs[0]);

	return 1;
}
