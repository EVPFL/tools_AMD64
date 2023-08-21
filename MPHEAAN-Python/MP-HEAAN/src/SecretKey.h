/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/
#ifndef HEAAN_SECRETKEY_H_
#define HEAAN_SECRETKEY_H_

#include <NTL/ZZ.h>

#include "Ring.h"

using namespace std;
using namespace NTL;

class SecretKey {
public:
	ZZ* sx = new ZZ[N];

	SecretKey(Ring& ring);
	SecretKey(const SecretKey& o);
	SecretKey(ZZ* sx_o);
	
	void setSK(ZZ* sx_o);
};


class SKShare {
public:
	long np = nprimes;
	uint64_t index = -1;
	uint64_t* rsx_share;

	SKShare(uint64_t index);
	SKShare(uint64_t index, long np);
	SKShare(uint64_t index, long np, uint64_t* rsx_share);

	virtual ~SKShare();
};


class SKShares {
public:
	long size = 0;
	long np = nprimes;
	uint64_t* indexs;
	uint64_t* rsx_shares;

	// SKShares(long size);
	SKShares(long size, long np);
	SKShares(long size, long np, uint64_t* indexs, uint64_t* rsx_shares);

	uint64_t getInxsItem(long i);
	uint64_t* getRsxsItem(long i);
	SKShare getItem(long i);
	
	void setInxsItem(long i, uint64_t index);
	void setInxsItem(long i, long index);
	void setRsxsItem(long i, uint64_t* share);
	void setItem(long i, uint64_t index, uint64_t* share);
	void setItem(long i, long index, uint64_t* share);
	

	virtual ~SKShares();
};

#endif
