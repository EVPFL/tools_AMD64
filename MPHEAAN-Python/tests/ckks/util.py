from HEAAN import ZZ, Ciphertext

from ckks.ckks_parameters import *

def reviseMod(z, mod_q, ZZ_size=1):
    assert isinstance(mod_q, int) and mod_q > 0
    half_q = mod_q >> 1
    if isinstance(z, int):
        z_rem = z % mod_q
        if z_rem > half_q: z_rem -= mod_q
        return z_rem
    if isinstance(z, ZZ):
        z_rems = []
        for i in range(ZZ_size):
            zi = int(z[i])
            zi_rem = zi % mod_q
            if zi_rem > half_q: zi_rem -= mod_q
            z.setitem(i, zi_rem)
            z_rems.append(zi_rem)
        return z_rems


def getZZMAC(zz, mod):
    assert isinstance(zz, ZZ)
    mac_sum = 0
    for i in range(N):
        mac_i = int( zz[i] )
        mac_sum += mac_i
    return mac_sum%mod, mod


def getPlaintxtMAC(plain, mod=None):
    assert isinstance(plain, Plaintext)
    if not mod:
        mod = int( ring.qpows[plain.logq] )
    mx_mac = getZZMAC(plain.mx, mod=mod)[0]
    return mx_mac, mod


def getCiphtxtMAC(ciph, mod=None):
    assert isinstance(ciph, Ciphertext)
    if not mod:
        mod = int( ring.qpows[ciph.logq] )
    ax_mac, bx_mac = getZZMAC(ciph.ax, mod=mod), getZZMAC(ciph.bx, mod=mod)
    ax_mac, bx_mac = ax_mac[0], bx_mac[0]
    return (ax_mac+bx_mac)%mod, mod


def getCiphtxtBXMAC(ciph, mod=None):
    assert isinstance(ciph, Ciphertext)
    if not mod:
        mod = int( ring.qpows[ciph.logq] )
    bx_mac = getZZMAC(ciph.bx, mod=mod)[0]
    return bx_mac%mod, mod


# msg_sum_int= [565761839817912475877553652200333176992383823276590246895717060640246909874244011727075449, 0]
# msgs_int= [[943655912917819574819578804764070216509787081201386265546154520116384381189147490707200208, 0],
# [287733417309485349358603394319661638385676301894912324351387997472643522505707317988411564, 0],
# [1371408485925093637967817141525979483148388833846227907634314992405600305942725909214861053,0]]
# fs_int= [[1.,1.], [1., 1.],[1., 1.]]
# mod= 52435875175126190479447740508185965837690552500527637822603658699938581184513