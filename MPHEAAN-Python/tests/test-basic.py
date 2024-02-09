from HEAAN import ZZ, SecretKey, Key, Plaintext, Uint64_t

from math import ceil
import time

from ckks.ckks_parameters import CKKSParameters, print_parameters
from ckks.ckks_key_generator import CKKSKeyGenerator
from ckks.ckks_encryptor import CKKSEncryptor
from ckks.ckks_decryptor import CKKSDecryptor
from ckks.ckks_encoder import CKKSMessage, CKKSEncoder

from ckks.util import reviseMod


logq = 300
logp = 30
logn = 10
n = 1*2**logn
slots = n


params = CKKSParameters(logq, logp, logn)
print_parameters(params)

print('\n =============== TEST =============== ')

key_generator = CKKSKeyGenerator(params)
sk1 = key_generator.generate_secret_key()
pk1 = key_generator.generate_public_key(sk1, fixedax_flag=True)
# print('pk1: ', pk1)
# pk1.rax.print(3)
#1547554453702589716916188973829224935956613212519119416745996053456785003644857998801763971
#715588862714759328211478728796649157983949335016195931708446953021942869534108778995299881
coeff = 818868749848758507533865125004921301789807538499403783310424361612861567728156597439054119
coeff = str(coeff)
coeff = '0'* (91-len(coeff)) + coeff
coeff_list = [ coeff[i*11:i*11+11]+'0' for i in range(5) ] + [ coeff[55+i*12:55+i*12+12] for i in range(3) ]
coeff_complex_list = [ complex( int(coeff_list[i]), int(coeff_list[i+1]) ) for i in range(0,8,2) ]

mlist1 = coeff_complex_list * round(slots/4)
msg1 = CKKSMessage(slots=slots, mlist=mlist1)
#mevc1 = msg1.vector
print('Message_1:')
print('\t', msg1.to_string(acc='acc',size=8))
# print(msg1.to_string(size=4))

# print('Encrpyt')
encrptor = CKKSEncryptor(params, pk1)
begin_time = time.time()
cipher1 = encrptor.encrypt(msg1)
end_time = time.time()
print('Encrypt time:', end_time-begin_time)

decryptor = CKKSDecryptor(params, sk1)
dmsg1 = decryptor.decrypt(cipher1)
print('Decrypt result_1:')
print('\t', dmsg1)
#print(dmsg1.to_string(size=4))


sk2 = key_generator.generate_secret_key()
pk2 = key_generator.generate_public_key(sk2, fixedax_flag=False)
encrptor2 = CKKSEncryptor(params, pk2)
decryptor2 = CKKSDecryptor(params, sk2)

mlist2 = [0.5] * round(slots)
msg2 = CKKSMessage(slots=slots, mlist=mlist2)
cipher2 = encrptor2.encrypt(msg2)
dmsg2 = decryptor2.decrypt(cipher2)

print('Message_2:\n\t', msg2)
print('Decrypt result_2:\n\t', dmsg2)
