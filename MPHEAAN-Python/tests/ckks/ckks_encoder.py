"""A module to encode integers as specified in the CKKS scheme.
"""

from HEAAN import ComplexDouble, Double, Plaintext, Ciphertext
from ckks.ckks_parameters import *

from math import ceil


def list2Vec(mlist):
    if isinstance(mlist, HEAAN.ComplexDouble):
        return mlist
    elif isinstance(mlist, HEAAN.Double):
        return mlist
    else:
        return ComplexDouble(mlist)

def list2str(l):
    assert isinstance(l, list)
    s = str(l[0])
    for li in l[1:]:
        s = s + ', ' + str(li)
    return s

class CKKSMessage:

    """ An message for several complex numbers as specified in the CKKS scheme.

    Attributes:
        slots (int): length of one message for encode.
        mlist (list float): float list to encode.
        mvec (ComplexDouble): vector = ComplexDouble(mlist).
    """

    def __init__(self, slots=None, mlist=None, mvec=None, msglist=None, padding_flag=True):
        """Inits CKKSMessage with the given parameters.
        """
        if msglist: # using lists of mlist (msg is a list of float/complex)
            assert isinstance(msglist, list) and msglist[0]
            self.msg_num = len(msglist)
            self.msgs_flag = True if self.msg_num > 1 else False
            self.length = msglist[0].length
            self.lists = msglist
            self.vectors = [ vc for msg in msglist for vc in msg.vectors ]

        else: # using mlist (a list of float/complex)
            assert slots
            self.length = slots
            self.__mlist = mlist
            self.__mvec = mvec

            # messages
            mlist = self.mlist
            self.msg_num = ceil(len(mlist)/self.length)
            self.msgs_flag = True if self.msg_num > 1 else False
            #msg_rem = len(mlist) % slots
            if padding_flag:
                mlist_pad = mlist + [0] * (slots-len(mlist)%slots)
                self.lists = [ mlist_pad[slots*i:slots*(i+1)] for i in range(self.msg_num) ]
            else:
                self.length = len(mlist)
                self.lists = [ mlist ]
            self.vectors = [ list2Vec(l) for l in self.lists ]
            

    @property
    def mlist(self):
        if not self.__mlist:
            assert self.__mvec
            self.__mlist = [ self.__mvec[i] for i in range(self.__mvec.size())]
        return self.__mlist

    @property
    def mvec(self):
        if not self.__mvec:
            assert self.__mlist
            self.__mvec = list2Vec(self.__mlist)
        return self.__mvec

    def getVals(self, inxs=(0,'default')):
        assert self.vectors
        if not self.msgs_flag:
            return self.vectors[0]
        else:
            assert self.lists
            (s,e) = inxs
            if e == 'default': 
                e = self.msg_num
            lists = self.lists[s:e]
            vals_list = [ m for l in lists for m in l ]
            return list2Vec(vals_list)


    def to_string(self, size=5, acc=4, omi_flag=False, imag_flag=False):
        """ prints message as a readable string. """
        assert self.vectors
        size = min( size, self.vectors[0].size() )
        mlists = [ [ mvec[j] for j in range(size) ] for mvec in self.vectors ]
        msg_num = self.msg_num if not omi_flag else min(self.msg_num, 5)
        
        s = ''
        for i in range(msg_num):
            if acc == 'acc':
                mreal = [ str(m.real()) for m in mlists[i] ]
            else:
                mreal = [ round(float(str(m.real())), acc) for m in mlists[i] ]
            #mreal = [ str(m.real()) for m in mlists[i] ]
            if imag_flag: 
                if acc == 'acc':
                    mimag = [ str(m.imag()) for m in mlists[i] ]
                else:
                    mimag = [ round(float(str(m.imag())), acc) for m in mlists[i] ]
                #mimag = [ str(m.imag()) for m in mlists[i] ]
                mlist = [ (mreal[i], mimag[i]) for i in range(len(mreal)) ]
            else:
                mlist = mreal
            if msg_num > 1:
                if size < self.length:
                    s += '\t[' + str(i) + ']: [' + list2str(mlist) + ', ... ]\n'
                else: 
                    s += '\t[' + str(i) + ']:' + str(mlist) + '\n'
            else:
                if size < self.length:
                    s += '\t[' + list2str(mlist) + ', ... ]\n'
                else:
                    s += '\t' + str(mlist) + '\n'
        if self.msg_num > msg_num:
            s += '\t......\n'
        return s


    def __str__(self):
        """Represents message as a readable string.
        """
        return self.to_string()



class CKKSEncoder():

    """ An object that can encode data for CKKS scheme. 
    """

    def __init__(self, params):
        """ Generates a encoder for CKKS scheme.
        Args:
            slots(int), n(int) & logp(int) of ckks_params.
        """
        #assert params.ring and params.slots
        self.slots = params.slots
        self.n = params.n
        self.logp = params.logp
        self.logq = params.logq


    def list2Vec(self, mlist, vec_type='ComplexDouble'):
        if isinstance(mlist, HEAAN.ComplexDouble):
            return mlist
        elif isinstance(mlist, HEAAN.Double):
            return mlist
        else:
            slots = self.slots
            mlist_pad = mlist + [0] * (slots-len(mlist)%slots)
            if vec_type=='Double':
                return Double(mlist_pad)
            else:
                return ComplexDouble(mlist_pad)


    def encode(self, msg, plain_flag=False, rescale_flag=False):
        slots = self.slots
        n, logp, logq = self.n, self.logp, self.logq

        if isinstance(msg, ComplexDouble) or isinstance(msg, Double):
            vals_list = [ msg ]

        elif isinstance(msg, list):
            if isinstance(msg[0], ComplexDouble) or isinstance(msg[0], Double):
                vals_list = msg
            else:
                msg_num = ceil( len(msg)/slots )
                msg_pad = msg + [0] * (slots-len(msg)%slots)
                vals_list = [ list2Vec(msg_pad[i:i*slots+slots]) for i in range(msg_num) ]
        
        else:
            raise ValueError("Input to encode must be ComplexDouble(s)/Double(s) or a list of elements which can transfer to ComplexDouble(s)/Double(s).")

        mxs = []
        for vals in vals_list:
            mx = ZZ(N)
            ring.encode(mx, vals, n, logp+logQ)
            mxs.append(mx)
        ress = mxs if not plain_flag else [ Plaintext(mx, logp, logq, n) for mx in mxs ]

        if plain_flag and rescale_flag: # rescale plaintxt to mod(p)
            for res in ress:
                ring.rightShiftAndEqual(res.mx, logQ)
        
        if len(ress)==1:
            return ress[0]
        else:
            return ress



    def decode(self, plain, raw_plain_flag=False, msg_flag=True):
        """
            return ckks_msg(msg_flag=msg_flag) or mvec(msg_flag=False)
        """
        assert isinstance(plain, Plaintext)

        mvec = None
        if raw_plain_flag: # plaintxt by encoder
            plain_tmp = Plaintext(plain.mx, plain.logp, plain.logq, plain.n)
            ring.rightShiftAndEqual(plain_tmp.mx, logQ)
            mvec = scheme.decode(plain_tmp)
        else: # plaintxt by decryptor
            mvec = scheme.decode(plain)

        if msg_flag:
            return CKKSMessage(slots=self.slots, mvec=mvec)
        else:
            return mvec
        




