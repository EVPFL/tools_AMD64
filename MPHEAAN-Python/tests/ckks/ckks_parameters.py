"""A module to keep track of parameters for the CKKS scheme."""

from HEAAN import Ring, Uint64_t, ZZ, MPScheme
import HEAAN

logN = HEAAN.logN
logQ = HEAAN.logQ
sigma = HEAAN.sigma
h = HEAAN.h
pbnd = HEAAN.pbnd
kbar = HEAAN.kbar
kbar2 = HEAAN.kbar2
logNh = HEAAN.logNh
logQQ = HEAAN.logQQ
N = HEAAN.N
Nh = HEAAN.Nh
M = HEAAN.M
nprimes = HEAAN.nprimes
Nnprimes = HEAAN.Nnprimes
cbnd = HEAAN.cbnd
bignum = HEAAN.bignum
Q = HEAAN.Q
QQ = HEAAN.QQ
skMod = 2
#skSharesMod = 3
#skSharesMod = 2625393301709025650356725023260873487640204166662639726676506771726662977387120292276944291
#skSharesMod = 1933263083295658986975746442313529621218681546714121084681222891853386083354576440394481239
ring = Ring()
pVec = ring.multiplier.pVec
scheme = MPScheme(ring)

def generate_ax(ring, N, logQQ):
    ax = ZZ(N)
    ring.sampleUniform2(ax, logQQ)
    return ax

def generate_rax(ax, ring, Nnprimes, nprimes):
    rax = Uint64_t(Nnprimes)
    ring.CRT(rax, ax, nprimes)
    return rax


class CKKSParameters():
    """An instance of parameters for the CKKS scheme.
    Attributes:
        HEAAN.Params
    """
    def __init__(self, logq=None, logp=None, logn=None, new_ring=None, fixedax_flag=True, n=None, slots=None, ax=None, rax=None):
        """ Inits Parameters with the given parameters. """
        global ring, scheme
        #self.ring = ring
        if new_ring:
            ring = new_ring
            scheme = MPScheme(ring)
        
        self.logq = logq if logq else 300
        self.logp = logp if logp else 30 
        self.logn = logn if logn else 12

        self.ax = ax
        self.__rax = rax
        if fixedax_flag and not self.ax:
            self.ax = generate_ax(ring, N, logQQ)
            # self.ax = ZZ(self.N)
            # self.ring.sampleUniform2(self.ax, self.logQQ)
            
        if self.ax and not self.__rax:
            self.__rax = generate_rax(self.ax, ring, Nnprimes, nprimes)
            # self.rax = Uint64_t(self.N)
            # self.ring.CRT(self.rax, self.ax, self.nprimes)

        self.n = 1*2**self.logn
        self.slots = self.n
        if n and n!= self.n:
            print("[Params setting mistake] n = 2**logn, new n: ", self.n)
        if slots and slots != self.slots:
            print("[Params setting mistake] slots = n, new slots: ", self.slots)


    @property
    def rax(self):
        return self.__rax



def print_parameters(ckk_parmas):
    """  Prints parameters. """
    print("[ CKKS parameters] ")
    print("  - default ")
    print("\t logN: %d" % (logN))
    print("\t logQ: %d" % (logQ))
    print("\t sigma: ", sigma)
    print("\t h: %d" % (h))
    print("\t pbnd: %d" % (pbnd))
    print("\t kbar: %d" % (kbar))
    print("\t kbar2: %d" % (kbar2))
    print("\t logNh: %d" % (logNh))
    print("\t logQQ: %d" % (logQQ))
    print("\t N: %d" % (N))
    print("\t Nh: %d" % (Nh))
    print("\t M: %d" % (M))
    print("\t nprimes: %d" % (nprimes))
    print("\t Nnprimes: %d" % (Nnprimes))
    print("\t cbnd: %d" % (cbnd))
    print("\t bignum: %d" % (bignum))
    #print("\t Q: ", Q[0])
    #print("\t QQ: ", QQ[0])
    print("\t skMod: %d" % (skMod))
    #print("\t skSharesMod: %d" % (skSharesMod))
    print("\t pVec: " + pVec[0] + ", " + pVec[1]+", ...")

    print("  - setting ")
    print("\t logq: %d" % (ckk_parmas.logq))
    print("\t logp: %d" % (ckk_parmas.logp))
    print("\t logn: %d" % (ckk_parmas.logn))
    print("\t n: %d" % (ckk_parmas.n))
    print("\t slots: %d" % (ckk_parmas.slots))
    # print("\t Pubilc key's ax:", end="\n \t\t")
    # parmas.ax.print(4)
    # print("\t Pubilc key's rax:", end="\n \t\t")
    # parmas.rax.print(4)