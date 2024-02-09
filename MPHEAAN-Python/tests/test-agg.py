from HEAAN import ZZ, SecretKey, Key, Plaintext, Uint64_t

import numpy as np
from math import ceil
import time

from ckks.ckks_parameters import *
from ckks.ckks_key_generator import CKKSKeyGenerator
from ckks.ckks_encryptor import CKKSEncryptor
from ckks.ckks_decryptor import CKKSDecryptor
from ckks.ckks_encoder import CKKSMessage, CKKSEncoder
from ckks.ckks_evaluator import CKKSEvaluator

from ckks.util import reviseMod


logq = 300
logp = 30
logn = 10
n = 1*2**logn
slots = n


params = CKKSParameters(logq, logp, logn)
print_parameters(params)

logp, logq = params.logp, params.logq
np = ceil( (1+logQQ+logN+2) / pbnd )
int_q = int( ring.qpows[logq] )
int_qQ = int( ring.qpows[logq+logQ] )
q, qQ = ZZ( [int_q] ), ZZ( [int_qQ] )


def aggpublickeyGen(pks):
    assert ring, params
    mod = nprimes
    agg_rbx = None
    for pk in pks:
        rbx = pk.rbx
        if agg_rbx:
            ring.addNTTAndEqual(agg_rbx, rbx, mod)
        else:
            agg_rbx = Uint64_t(rbx)
    agg_pk = Key(params.rax, agg_rbx)
    return agg_pk


def aggsecretkeyGen(sks):
    agg_sk = None
    mod = ZZ([2])
    for sk in sks:
        if agg_sk:
            ring.addAndEqual(agg_sk.sx, sk.sx, mod)
        else:
            agg_sk = SecretKey(sk.sx)
    return agg_sk


def decryshareGen(sk, rv=None, return_rv=True):
    assert params
    if not rv:
        rv = ZZ(N)
        ring.sampleZO(rv)
    ax = params.ax
    
    d = ZZ(N)
    # ring.multNTT(d, rv, rax, np, qQ)
    # ring.rightShiftAndEqual(d, logQ)
    # ring.multAndEqual(d, sk.sx, np, q)
    # d.print(3)
    # ring.addGaussAndEqual(d, qQ)

    rv.print(3)


    print('test d:')
    # np_t = ceil( (1+logq+logN+2) / pbnd )
    ring.mult(d, rv, sk.sx, np, qQ)
    d.print(3)
    ring.multAndEqual(d, ax, np, qQ)
    d.print(3)
    # ring.rightShiftAndEqual(d, logQ)
    # d.print(3)

    print('right d:')
    ring.mult(d, rv, ax, np, qQ)
    # d.print(3)
    ring.multAndEqual(d, sk.sx, np, qQ)
    # d.print(3)
    # ring.rightShiftAndEqual(d, logQ)
    d.print(3)
    

    if return_rv:
        return d, rv
    else:
        return d



print('\n =============== TEST =============== ')

key_generator = CKKSKeyGenerator(params)
sk1 = key_generator.generate_secret_key()
pk1 = key_generator.generate_public_key(sk1, fixedax_flag=True)

sk2 = key_generator.generate_secret_key()
pk2 = key_generator.generate_public_key(sk2, fixedax_flag=True)

pks = [pk1, pk2]
agg_pk = aggpublickeyGen(pks)
sks = [sk1, sk2]
agg_sk = aggsecretkeyGen(sks)

encrptor = CKKSEncryptor(params, agg_pk)
decryptor = CKKSDecryptor(params, agg_sk)
evaluator = CKKSEvaluator(params)
encoder = CKKSEncoder(params)

encryptor1 = CKKSEncryptor(params, pk1)
encryptor2 = CKKSEncryptor(params, pk2)


# test: (agg_sk, agg_pk)
mlist = [0.5] * round(slots)
msg = CKKSMessage(slots=slots, mlist=mlist)
cipher = encrptor.encrypt(msg)
dmsg = decryptor.decrypt(cipher)
print('Message:\n\t', msg)
print('Decrypt result:\n\t', dmsg)

# test: d1 + d2
# rax = params.rax
# rv1, d1 = ZZ(N), ZZ(N)
# ring.sampleZO(rv1)
# ring.multNTT(d1, rv1, rax, np, qQ)
# ring.rightShiftAndEqual(d1, logQ)
# ring.multAndEqual(d1, sk1.sx, np, q)
# ring.addGaussAndEqual(d1, qQ)
# d.print()

d1, rv1 = decryshareGen(sk1)
# print("d1:")
# d1.print(3)
plain_d1 = Plaintext(d1, logp, logq, n)
print("plain_d1:")
plain_d1.mx.print(3)
# evaluator.decode(plain_d1)
# plain_d1.mx.print(3)
ct1 = encrptor.encrypt(plain_d1)
dec_1 = decryptor.decrypt(ct1, plain_flag=True)
print("dec_1:")
dec_1.mx.print(3)


d2, rv2 = decryshareGen(sk2)
plain_d2 = Plaintext(d2, logp, logq, n)
print("plain_d2:")
plain_d2.mx.print(3)


ct2 = encrptor.encrypt(plain_d2)
# dec_2 = decryptor.decrypt(ct2, plain_flag=True)
# print("dec_2:")
# dec_2.mx.print(3)


cts = [ct1, ct2]
ct_sum = evaluator.sum(cts)

dec_plain = decryptor.decrypt(ct_sum, plain_flag=True)
print("dec_plain:")
dec_plain.mx.print(3)

# print("q:")
# q.print(0)


# encrypt
cipher1 = encryptor1.encrypt(msg, random_vec=rv1)
cipher2 = encryptor2.encrypt(msg, random_vec=rv2)
ciph_sum = evaluator.sum([cipher1,cipher2])

# # decrypt (using dec_shares directly)
# sum_mx = ZZ(N)
# ds = [d1,d2]
# ring.addAndEqual(sum_mx, ciph_sum.bx, q)
# for d in ds:
#     # ring.multNTTAndEqual(d, params.rax, np, q)
#     # d.print(3)
#     ring.addAndEqual(sum_mx, d, q)
# plain = Plaintext(sum_mx, logp, logq, n)
# msg = encoder.decode(plain)
# print("msg:", msg)

# decrypt (using dec_dec_share_sum)
sum_mx = ZZ(N)
ring.addAndEqual(sum_mx, ciph_sum.bx, qQ)
ring.addAndEqual(sum_mx, dec_plain.mx, qQ)
plain = Plaintext(sum_mx, logp, logq, n)
msg = encoder.decode(plain)
print("msg:", msg)








