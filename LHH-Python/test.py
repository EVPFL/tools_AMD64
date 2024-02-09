from LHH import *
import random

print("============= LHH basic test =============")
lhh_test()
print("==========================================\n")


print("====== LHH (Python Module) example =======")
msgs_num = 3
msg_len = 10

# set alphas
alphas = LHHVal(msgs_num)
for i in range(msgs_num):
	alpha_i_str = str( random.randint(10, 20) )
	alphas.setVal(alpha_i_str, i)
print('alphas:', end='\n\t')
alphas.print()

# set msgs
msgs = LHHMsgs(msgs_num, msg_len)
msgs_list = []
for i in range(msgs_num):
	msg = LHHVal(msg_len)
	for j in range(msg_len):
		msg_j_str = str( random.randint(1, 1000) )
		msg.setVal(msg_j_str, j)
	msgs_list.append(msg)
	msgs.setMsgs(msg, i)
	print('msg['+str(i)+']:', end='\n\t')
	msg.print()

# init lhhscheme
lhhscheme = LHHScheme(msg_len)

# get msg_agg = agg(msgs, alphas) & msg_agg_hash = hash(msg_agg)
msg_agg = LHHVal(msg_len)
lhhscheme.evalMsgs(msg_agg, msgs, alphas)
msg_agg_hash = LHHHash()
lhhscheme.getHash(msg_agg_hash, msg_agg)
print('msg_agg:', end='\n\t')
msg_agg.print()
print('hash(msg_agg):', end='\n\t')
lhhscheme.printHash(msg_agg_hash)

# get hashes: hashes[i] = hash(msgs[i])
hashes = LHHHash()
lhhscheme.resetHashes(hashes, msgs_num)
for i in range(msgs_num):
	h = LHHHash()
	lhhscheme.getHash(h, msgs_list[i])
	lhhscheme.setHashes(hashes, h, i)
	# print('hash['+str(i)+']:', end='\n\t')
	# lhhscheme.printHash(h)
# print('hashes:', end='\n\t')
# lhhscheme.printHash(hashes)

# eval hashes: hash_agg = agg(hashes, alphas)
hash_agg = LHHHash()
lhhscheme.evalHashes(hash_agg, hashes, alphas)
print('agg(hashes,alphas):', end='\n\t')
lhhscheme.printHash(hash_agg) # print( lhhscheme.getHashesHex(hash_agg) )

# verify homomorphism
# ver_res1: hash_agg == msg_agg_hash
ver_res = lhhscheme.cmpHash(hash_agg, msg_agg_hash)
ver_res = True if ver_res else False
print('agg(hashes,alphas) == hash(msg_agg):', ver_res)
if ver_res:
	print('   >>> homomorphism is satisfied!')
# ver_res2: hash_agg == hash( agg(msgs, alphas) ), i.e., test correctness of lhhscheme.verHashes
ver_res = lhhscheme.verHashes(msg_agg_hash, msgs, alphas)
ver_res = True if ver_res else False
print('agg(hashes,alphas) == hash(agg(msgs,alphas)):', ver_res)
if ver_res:
	print('   >>> lhhscheme.verHashes() function is correct!')
print('LHH (Python) test done!!')
print("==========================================\n")



