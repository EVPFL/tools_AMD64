"""A module to encrypt for the CKKS scheme."""

from HEAAN import Key, ZZ, Plaintext, Ciphertext, Ciphertexts

from ckks.ckks_parameters import *
from ckks.ckks_encoder import *


class CKKSEncryptor:

    """ An object that can encrypt data using CKKS given a public key. 
    """

    def __init__(self, params, public_key):
        """ Generates private/public key pair for CKKS scheme.

        Attributes:
            params (Parameters): Parameters including polynomial degree,
                ciphertext modulus, etc.
            public_key_scheme (Scheme): HEAAN scheme including 
            the public key used for encryption, and the encrpytion methods.
        """
        assert isinstance(public_key, Key)

        self.params = params
        self.public_key = public_key


    def encryptVec(self, cipher, mvec, n, logp, logq, random_vec=None):
        """ Encrypts a message
        Args:
            message (ComplexDouble): Plaintext to be encrypted.
        """
        assert isinstance(mvec, ComplexDouble)

        if not random_vec:
            scheme.encrypt(cipher, mvec, self.public_key, n, logp, logq)
        else:
            scheme.encryptWithRV(cipher, mvec, self.public_key, n, logp, logq, random_vec)
        #return cipher
        

    def encrypt(self, message, random_vec=None, thread_num=1, list_flag=False, n=None, logp=None, logq=None):
        """Encrypts a message.

        Encrypts the message and returns the corresponding ciphertext.

        Args:
            message (CKKSMessage/ComplexDouble): Plaintext to be encrypted.

        Returns:
            A ciphertext consisting of a pair of polynomials in the ciphertext
            space.
        """
        assert self.public_key

        if isinstance(message, Plaintext):
            cipher = Ciphertext()
            scheme.encryptMsg(cipher, message, self.public_key)
            cipher = [cipher] if list_flag else cipher
            return cipher
        
        if isinstance(message, ComplexDouble):
            message = CKKSMessage(slots=self.params.slots, mvec=message)
        assert isinstance(message, CKKSMessage)
        if isinstance(random_vec, list) and isinstance(random_vec[0], int):
            random_vec = data2Obj(random_vec, 'ZZ')

        # assert self.params.n and self.params.logp and self.params.logq
        # n, logp, logq = self.params.n, self.params.logp, self.params.logq
        n = n if n else self.params.n
        logp = logp if logp else self.params.logp
        logq = logq if logq else self.params.logq
        msg_num = message.msg_num
        ciphers = []
        # ciphers = [ Ciphertext() for _ in range(msg_num) ]
        if thread_num > 1:
            while msg_num % thread_num != 0:
                thread_num -= 1
            # assert msg_num % thread_num == 0 
            msg_num_td = round(msg_num/thread_num)
            # using new method (multiple threads in C++)
            ciphers = Ciphertexts(msg_num)
            vals = message.getVals()
            if not random_vec:
                scheme.encrypt( ciphers, vals, self.public_key, n, logp, logq, msg_num, thread_num )
            else:
                scheme.encryptWithRV( ciphers, vals, self.public_key, n, logp, logq, msg_num, thread_num, random_vec )
            #---------

        else:
            ciphers = [ Ciphertext() for _ in range(msg_num) ]
            mvecs = message.vectors
            range_msg_num = range(msg_num)
            for i in range_msg_num:
                self.encryptVec(ciphers[i], mvecs[i], n, logp, logq, random_vec=random_vec) 

        
        if list_flag and isinstance(ciphers, Ciphertexts):
            cipher_res = [ ciphers[i] for i in range(ciphers.size) ]
                
        else:
            cipher_res = ciphers if msg_num>1 else ciphers[0]
            
        return cipher_res


        



