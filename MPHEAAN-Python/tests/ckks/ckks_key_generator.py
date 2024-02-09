"""A module to generate public and private keys for the CKKS scheme."""

from HEAAN import Ring, SecretKey, Key

from ckks.ckks_parameters import *

import gc
import sys

class CKKSKeyGenerator:

    """An instance to generate a public/secret key pair and relinearization keys.

    The secret key s is generated randomly, and the public key is the
    pair (-as + e, a). The relinearization keys are generated, as
    specified in the CKKS paper.

    Attributes:
        params (Parameters): Parameters including polynomial degree, plaintext,
            and ciphertext modulus.
        secret_key (Polynomial): secret key randomly generated from R_q.
        public_key (tuple of Polynomials): public key generated from
            secret key.
        relin_key (tuple of Polynomials): relinearization key generated
            from secret key.
    """

    def __init__(self, params, secret_key=None, fixedax_flag=True, ax=None):
        """Generates secret/public key pair for CKKS scheme.

        Args:
            params (Parameters): Parameters including polynomial degree,
                plaintext, and ciphertext modulus.
        """
        self.params = params
        self.secret_key = secret_key
        self.fixedax_flag = fixedax_flag
        self.ax = ax if ax else params.ax
        assert not self.fixedax_flag or (self.fixedax_flag and self.ax)


    def generate_secret_key(self):
        """Generates a secret key for CKKS scheme.

        Args:
            params (Parameters): Parameters including polynomial degree,
                plaintext, and ciphertext modulus.
        """
        secret_key = SecretKey(ring)
        return secret_key

    # @profile(precision=4, stream=open("memory_profiler_pkgen.log", "w+"))
    def generate_public_key(self, secret_key=None, fixedax_flag=-1, ax=None):
        """Generates a public key for CKKS scheme.

        Args:
            params (Parameters): Parameters including polynomial degree,
                plaintext, and ciphertext modulus.
        """
        sk = secret_key if secret_key else self.secret_key
        assert sk
        fixedax_flag = self.fixedax_flag if fixedax_flag == -1 else fixedax_flag
        assert scheme and isinstance(fixedax_flag, bool)

        fixedax_flag = True if ax else fixedax_flag

        if fixedax_flag: # ax is fixed
            if ax: # using given ax
                pk = scheme.getEncKey(sk, ax)
            else: # using ax in self.params
                assert self.params.ax
                # if not self.params.ax: 
                # assert self.params.N and self.params.logQQ, self.params.nprimes
                # N, logQQ, nprimes = self.params.N, self.params.logQQ, self.params.nprimes
                # self.params.ax = generate_ax(ring, N, logQQ)
                if not self.params.rax: 
                    pk = scheme.getEncKey(sk, self.params.ax)
                else: 
                    pk = scheme.getEncKey(sk, self.params.rax)
        else: # ax is not fixed
            pk = scheme.getEncKey(sk)
        #self.__public_key = pk
        # #print('get:', self.public_key.rbx.__getitem__(6) )
        # print('size of pk.rax:', sys.getsizeof(pk.rax))
        # print('pk.rax.size():', pk.rax.size() )
        # pk.rax.free()
        # print('pk.rax.size():', pk.rax.size() )
        # #print( 'size of pk.rax:', sys.getsizeof(pk.rax[2]) )
        return pk


    def generate_relin_key(self, secret_key=None, rot_place=1, fixedax_flag=-1, ax=None):
        """Generates a public key for CKKS scheme.

        Args:
            params (Parameters): Parameters including polynomial degree,
                plaintext, and ciphertext modulus.
        """
        sk = secret_key if secret_key else self.secret_key
        assert sk
        if isinstance(sk, DataSecretKey):
            sk = data2Obj(sk, 'SecretKey')
        fixedax_flag = self.fixedax_flag if fixedax_flag == -1 else fixedax_flag
        assert scheme and isinstance(fixedax_flag, bool)

        fixedax_flag = True if ax else fixedax_flag

        if fixedax_flag: # ax is fixed
            if ax: # using given ax
                rk = scheme.getMultKey(sk, ax)
            else: # using ax/rax in self.params
                assert self.params.ax
                if not self.params.rax: 
                    rk = scheme.getMultKey(sk, self.params.ax)
                else: 
                    rk = scheme.getMultKey(sk, self.params.rax)
        else: # ax is not fixed
            rk = scheme.getMultKey(sk)
        return rk


    def generate_rot_key(self, secret_key=None, rot_place=1, fixedax_flag=-1, ax=None):
        """Generates a public key for CKKS scheme.

        Args:
            params (Parameters): Parameters including polynomial degree,
                plaintext, and ciphertext modulus.
        """
        sk = secret_key if secret_key else self.secret_key
        assert sk
        if isinstance(sk, DataSecretKey):
            sk = data2Obj(sk, 'SecretKey')
        fixedax_flag = self.fixedax_flag if fixedax_flag == -1 else fixedax_flag
        assert scheme and isinstance(fixedax_flag, bool)

        fixedax_flag = True if ax else fixedax_flag

        if fixedax_flag: # ax is fixed
            if ax: # using given ax
                rk = scheme.getLeftRotKey(sk, rot_place, ax)
            else: # using ax/rax in self.params
                assert self.params.ax
                if not self.params.rax: 
                    rk = scheme.getLeftRotKey(sk, rot_place, self.params.ax)
                else: 
                    rk = scheme.getLeftRotKey(sk, rot_place, self.params.rax)
        else: # ax is not fixed
            rk = scheme.getLeftRotKey(sk, rot_place)
        return rk


    def generate_rot_keys(self, secret_key=None, rot_places=8, fixedax_flag=-1, ax=None):
        """Generates a public key for CKKS scheme.

        Args:
            params (Parameters): Parameters including polynomial degree,
                plaintext, and ciphertext modulus.
        """
        sk = secret_key if secret_key else self.secret_key
        assert sk
        fixedax_flag = self.fixedax_flag if fixedax_flag == -1 else fixedax_flag
        assert scheme and isinstance(fixedax_flag, bool)

        fixedax_flag = True if ax else fixedax_flag

        rks = []
        for r in range(1, rot_places+1):
            rks.append( self.generate_rot_key(secret_key=secret_key, rot_place=r, fixedax_flag=fixedax_flag, ax=ax) )

        return rks


    def __del__(self):
        #print('delete CKKSKeyGenerator Object.')
        gc.collect()
