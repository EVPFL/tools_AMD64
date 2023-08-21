/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#include "Plaintext.h"

Plaintext::Plaintext(long logp, long logq, long n) : logp(logp), logq(logq), n(n) {

}

Plaintext::Plaintext(ZZ* mx_o, long logp_o, long logq_o, long n_o) {
	logp = logp_o;
	logq = logq_o;
	n = n_o;

	for (long i = 0; i < N; i++) {
		mx[i] = mx_o[i];
	}
}

Plaintext::~Plaintext() {
	delete[] mx;
}
