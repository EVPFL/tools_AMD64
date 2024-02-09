"""A module to decrypt for the CKKS scheme."""

from HEAAN import Ciphertext, ComplexDouble, Plaintext
from ckks.ckks_parameters import *
from ckks.ckks_encoder import CKKSMessage, CKKSEncoder

class CKKSDecryptor:

    """An object that can decrypt data using CKKS given a secret key.
    """

    def __init__(self, params, secret_key):
        """Initializes decryptor for CKKS scheme.

        Attributes:
            params (Parameters): Parameters including polynomial degree,
                ciphertext modulus, etc.
            secret_key (Key): Secret key used for decryption.
        """
        assert ring and params.slots and secret_key
        self.slots = params.slots
        self.secret_key = secret_key


    def __decrypt__(self, ciphertext, plain_flag=False, msg_flag=False):
        """Decrypts a ciphertext.

        Decrypts the ciphertext and returns the corresponding message.

        Args:
            ciphertext (Ciphertext / list of Ciphertext): Ciphertext to be decrypted.

        Returns:
            The message corresponding to the decrypted ciphertext.
        """
        assert self.secret_key 
        assert isinstance(ciphertext, Ciphertext)
        if plain_flag:
            plain = Plaintext()
            scheme.decryptMsg(plain, self.secret_key, ciphertext)
            return plain
        else:
            dvec = ComplexDouble()
            dvec = scheme.decrypt(self.secret_key, ciphertext)
            dmsg = CKKSMessage(slots=self.slots, mvec=dvec, padding_flag=False) if msg_flag else dvec
            return dmsg


    def decrypt(self, ciphertext, plain_flag=False, msg_flag=False, list_flag=True):
        """Decrypts a ciphertext.

        Decrypts the ciphertext and returns the corresponding message.

        Args:
            ciphertext (Ciphertext / list of Ciphertext): Ciphertext to be decrypted.

        Returns:
            The message corresponding to the decrypted ciphertext.
        """
        assert self.secret_key 

        if isinstance(ciphertext, Ciphertext):
            return self.__decrypt__(ciphertext, msg_flag=msg_flag, plain_flag=plain_flag)

        elif isinstance(ciphertext, list):
            dmsgs = [ self.__decrypt__(ciph, msg_flag=msg_flag, plain_flag=plain_flag) for ciph in ciphertext ]
            if len(dmsgs) == 1 and not list_flag:
                dmsgs = dmsgs[0]
            return dmsgs


        