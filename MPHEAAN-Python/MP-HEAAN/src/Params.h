/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#ifndef HEAAN_PARAMS_H_
#define HEAAN_PARAMS_H_

#include <NTL/ZZ.h>
using namespace NTL;

static const long logN = 16;
static const long logQ = 1200;
static const double sigma = 3.2;
static const long h = 64;
static const long pbnd = 59.0;
static const long kbar = pbnd + 1;
static const long kbar2 = 2 * kbar;
static const long logNh = (logN - 1);
static const long logQQ = (2 * logQ);
static const long N = (1 << logN);
static const long Nh = (1 << logNh);
static const long M = (N << 1);
static const long nprimes = (2 + logN + 4 * logQ + pbnd - 1) / pbnd;
static const long Nnprimes = (nprimes << logN);
static const long cbnd = (logQQ + NTL_ZZ_NBITS - 1) / NTL_ZZ_NBITS;
static const long bignum = 0xfffffff;
static const ZZ Q = power2_ZZ(logQ);
static const ZZ QQ = power2_ZZ(logQQ);

static const long &getParamslogN() { return logN; }
static const long &getParamslogQ() { return logQ; }
static const double &getParamssigma() { return sigma; }
static const long &getParamsh() { return h; }
static const long &getParamspbnd() { return pbnd; }
static const long &getParamskbar() { return kbar; }
static const long &getParamskbar2() { return kbar2; }
static const long &getParamslogNh() { return logNh; }
static const long &getParamslogQQ() { return logQQ; }
static const long &getParamsN() { return N; }
static const long &getParamsNh() { return Nh; }
static const long &getParamsM() { return M; }
static const long &getParamsnprimes() { return nprimes; }
static const long &getParamsNnprimes() { return Nnprimes; }
static const long &getParamscbnd() { return cbnd; }
static const long &getParamsbignum() { return bignum; }
static const ZZ &getParamsQ() { return Q; }
static const ZZ &getParamsQQ() { return QQ; }

// class Params {
// public:
// 	const long logN = 16;
// 	const long logQ = 1200;

// 	const double sigma = 3.2;
// 	const long h = 64;
// 	const long pbnd = 59.0;
// 	const long kbar = pbnd + 1;
// 	const long kbar2 = 2 * kbar;
// 	const long logNh = (logN - 1);
// 	const long logQQ = (2 * logQ);
// 	const long N = (1 << logN);
// 	const long Nh = (1 << logNh);
// 	const long M = (N << 1);
// 	const long nprimes = (2 + logN + 4 * logQ + pbnd - 1) / pbnd;
// 	const long Nnprimes = (nprimes << logN);
// 	const long cbnd = (logQQ + NTL_ZZ_NBITS - 1) / NTL_ZZ_NBITS;
// 	const long bignum = 0xfffffff;
// 	const ZZ Q = power2_ZZ(logQ);
// 	const ZZ QQ = power2_ZZ(logQQ);
// };

#endif /* PARAMS_H_ */
