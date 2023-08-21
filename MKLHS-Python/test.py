from MKLHS import *


S = 2
L = 2

print("-- init_mklhs")
res = init_mklhs()

print("-- params")
params = LHSParams()
# params.initParams()
params.print()
n_str = params.getNStr()
params.setDataset("dataset1")
params.print()

print("-- msgs")
msgs1 = LHSMsgs(L)
for i in range(L):
	msg_str = "e9f40b6a45f72b7f7ccd5b72805d118fba35dfc624b29fd7c91f0a9ef555c47d19ed18fb54c"
	msgs1.setMsg(msg_str, i, n_str)
	# msgs1.setMsg(11*(i+1), i, params)
# print("\tmsgs size:", msgs1.size)
msgs1.print()

msgs2 = LHSMsgs(L)
for i in range(L):
	msgs2.setMsg(22*(i+1), i, params)
msgs2.print()

mkmsgs = MKLHSMsgs(S, L)
mkmsgs.setMsgs(msgs1, 0)
mkmsgs.setMsgs(msgs2, 1)
# mkmsgs[0].print()


print("-- key")
sk1, pk1 = LHSSKey(), LHSPKey()
sk2, pk2 = LHSSKey(), LHSPKey()
# print(type(sk))
# print(type(pk))


print("-- signs")
signs1 = LHSSigns(L)
# print("\tsigns size:", signs1.size)
# print(type(signs1))
signs2 = LHSSigns(L)
# print("\tsigns size:", signs2.size)
# print(type(signs2))


print("-- scheme")
scheme = LHSScheme(params)

scheme.keyGen(sk1, pk1, "Gillian")
# print("sk1:")
# sk1.print()
print("sk1.getKeyStr():", sk1.getKeyStr())
print("pk1:")
pk1.print()
pk_str = pk1.getKeyStr()
print("pk1.getKeyStr():", pk_str)
pk1.setKey(pk_str)
pk1.print()
# print( pk1.getKeyStr() )

scheme.keyGen(sk2, pk2, "Charlene")
# print("pk2:")
# pk2.print()

scheme.signMsgs(signs1, msgs1, sk1)
print("signer1:", sk1.id)
print("signs1:")
signs1.print()

scheme.signMsgs(signs2, msgs2, sk2, "Charlene")
print("signs2:")
signs2.print()
# signs2_str = signs2.getSignStr()
# print('signs2_str:', signs2_str)
# signs2.setSign(signs2_str)
# print("signs2:")
# signs2.print()

mksigns = MKLHSSigns(S)
mksigns.setSigns(signs1, 0)
mksigns.setSigns(signs2, 1)
print(mksigns.signers_num)
# mksigns[0].print()
# print('set done')


f1, f2 = LHSLinearity(L), LHSLinearity(L)
f1.setCoeff(1, 0)
f1.setCoeff(1, 1)

f2.setCoeff(2, 0)
f2.setCoeff(2, 1)

print('-- fs')
fs = MKLHSLinearity(S, L)
fs.setCoeffs(f1, 0)
fs.setCoeffs(f2, 1)
print("\tfs signers_num:", fs.signers_num)

print('-- eval signs')
signs_res = LHSSigns()
scheme.evalSigns(signs_res, mksigns, fs)
print("signs_res:")
signs_res.print()


print('-- eval msgs')
msgs_res = LHSMsgs(L)
# print("mkmsgs signers_num:", mkmsgs.signers_num)
scheme.evalMsgs(msgs_res, mkmsgs, fs)
print("msgs_res:")
msgs_res.print()

mkpks = MKLHSPKeys(S)
mkpks.setPKs(pk1, 0)
mkpks.setPKs(pk2, 1)
# mkpks[0].setID("Alice")
# print(mkpks[0].id)
ver_res = scheme.verMKLHS(signs_res, mkmsgs, fs, mkpks)
print("ver_res:", ver_res)

print('-- eval sign')
S = 1
signs_res = LHSSigns()
scheme.evalSign(signs_res, signs1, f1)
# print("signs_res:")
# signs_res.print()
# print('-- eval msg')
# msgs_res = LHSMsgs(1)
# scheme.evalMsg(msgs_res, msgs1, f1)
# # print("msgs_res:")
# msgs_res.print()
mkpks = MKLHSPKeys(S)
mkpks.setPKs(pk1, 0)
print('-- msg & f & sign')
mkmsgs = MKLHSMsgs(S, L)
mkmsgs.setMsgs(msgs1, 0)
fs = MKLHSLinearity(S, L)
fs.setCoeffs(f1, 0)
mksigns = MKLHSSigns(S)
mksigns.setSigns(signs1, 0)
# print("\tfs signers_num:", fs.signers_num)


ver_res = scheme.verMKLHS(signs_res, mkmsgs, fs, mkpks)
print("ver_res:", ver_res)
mkmsgs[0].getMsgsStr()
print("f1[0]:", f1[0])

fs = MKLHSLinearity(S, L)
print("fs[0]:", fs[0][0])



