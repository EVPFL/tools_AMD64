"""A module to perform computations on ciphertexts in CKKS."""

from HEAAN import Scheme, Ciphertext, Ciphertexts
from ckks.ckks_parameters import *
from tqdm import tqdm

class CKKSEvaluator(Scheme):

    """An instance of an evaluator for ciphertexts.

    This allows us to add, multiply, and relinearize ciphertexts.

    Attributes:
        degree (int): Polynomial degree of ring.
        big_modulus (int): Modulus q of coefficients of polynomial
            ring R_q.
        scaling_factor (float): Scaling factor to encode new plaintexts with.
        boot_context (CKKSBootstrappingContext): Bootstrapping pre-computations.
        crt_context (CRTContext): CRT functions.
    """

    def __init__(self, params=None):
        """Inits Evaluator.
        Args:
            params (Parameters): Parameters including polynomial degree, ciphertext modulus,
                and scaling factor.
        """
        assert ring
        super().__init__(ring) #inherit
        self.params = params

    def __sumAndEqual__(self, ciph_sum, ciphs, ciphs_num, weights=None, tqdm_flag=True):
        # if weights:
        #   assert ciphs_num == len(weights)

        ciph_sum.copy( ciphs[0])
        q = ZZ( [int( ring.qpows[ciph_sum.logq] )] )
        if weights:
            if isinstance(ciph_sum, Ciphertext):
                ring.multByConstAndEqual(ciph_sum.bx, ZZ([weights[0]]), q)
            else:
                for j in range(ciph_sum.size):
                    ring.multByConstAndEqual(ciph_sum.getBxs(j), ZZ([weights[0]]), q)

        if ciphs_num == 1:
            return
        range_ciph_num = tqdm(range(1, ciphs_num)) if tqdm_flag else range(1, ciphs_num)

        if isinstance(ciphs, Ciphertexts):
            w = ZZ([ciphs_num])
            ring.multByConstAndEqual(ciph_sum.ax, w, q)
            for i in range_ciph_num:
                if weights:
                    ring.multByConstAndEqual(ciphs.getBxs(i), ZZ([weights[i]]), q)
                ring.addAndEqual(ciph_sum.bx, ciphs.getBxs(i), q)

        elif isinstance(ciphs, list) and isinstance(ciph_sum, Ciphertext):
            for i in range_ciph_num:
                ciph_i = ciphs[i]
                if weights:
                    ring.multByConstAndEqual(ciph_i.bx, ZZ([weights[i]]), q)
                self.addAndEqual(ciph_sum, ciph_i)

        elif isinstance(ciphs, list) and isinstance(ciph_sum, Ciphertexts):
            for i in range_ciph_num:
                ciph_i = ciphs[i]
                ring.addAndEqual(ciph_sum.ax, ciph_i.ax, q)
                for j in range(ciph_sum.size):
                    if weights:
                        ring.multByConstAndEqual(ciph_i.getBxs(j), ZZ([weights[i]]), q)
                    ring.addAndEqual(ciph_sum.getBxs(j), ciph_i.getBxs(j), q)
        
        else:
            raise ValueError('Input of CKKSEvaluator.sum() must be Ciphertexts(s) or Ciphertext(s).')


    def sumAndEqual(self, ciph_sum, ciphs, ciphs_num=None, weights=None, tqdm_flag=False):
        if isinstance(ciphs, Ciphertexts):
            ciphs_num = ciphs_num if ciphs_num else ciphs.size 
        elif isinstance(ciphs, Ciphertext):
            ciphs_num = 1
        elif isinstance(ciphs, list):
            ciphs_num = ciphs_num if ciphs_num else len(ciphs)
        else:
            raise ValueError('Input of CKKSEvaluator.sum() must be Ciphertexts(s) or Ciphertext(s).')
        self.__sumAndEqual__(ciph_sum, ciphs, ciphs_num, weights=weights, tqdm_flag=tqdm_flag)

        return ciph_sum


    def sum(self, ciphs, ciphs_num=None, weights=None, tqdm_flag=False):
        if isinstance(ciphs, list) and isinstance(ciphs[0], Ciphertexts):
            ciph_sum = Ciphertexts(ciphs[0].size)
        else:
            ciph_sum = Ciphertext()
        self.sumAndEqual(ciph_sum, ciphs, ciphs_num=ciphs_num, weights=weights, tqdm_flag=tqdm_flag)
        return ciph_sum
    

    def all_sum(self, ciph, rot_key, ele_num=None, tqdm_flag=False):
        """ The sum of the elements in the plaintext vector corresponding to a ciphertext.

        Args:
            ciph (Ciphertext): ciphertext.
            rot_keys(Key): left rotation key(r=1).

        Returns:
            A Ciphertext which is the sum of the first ele_num elements in ciph.
        """
        assert isinstance(ciph, Ciphertext)

        ciph_sum = Ciphertext(ciph)
        ciph_rot = Ciphertext(ciph)
        ele_num = ele_num if ele_num else ciph.n
        range_ele_num = tqdm(range(0, ele_num)) if tqdm_flag else range(0, ele_num)
        for rotation in range_ele_num:
            self.leftRotateFastAndEqual(ciph_rot, 1, rot_key)
            self.addAndEqual(ciph_sum, ciph_rot)
        return ciph_sum
