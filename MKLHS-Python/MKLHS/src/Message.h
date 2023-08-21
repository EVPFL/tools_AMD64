#ifndef MKLHS_MSEEAGE_H_
#define MKLHS_MSEEAGE_H_

#include "relic.h"
#include "Params.h"
#include <vector>
#include <iostream>
using namespace std;


class Message {
public:
	int size;
	bn_t* msgs;

	Message();
	Message(int size);
	Message(const Message& o);

	void resetMsg(int size);
	void setMsg(bn_t msg, int i, const Params& params);
	void setMsg(long msg_dig, int i, const Params& params);
	void setMsg(string msg_dig_str, int i, const Params& params);
	void setMsg(string msg_dig_str, int i, string n_str);
	void setMsg(string msg_dig_str, int str_len, int i, const Params& params);
	
	// string getMsgsStr(int i);
	string getMsgsStr();
	void printMsgs();
	void printMsgs(int msgs_num);
	void printMsgs(int start_inx, int end_inx);


	virtual ~Message();
};


class MKMessage {
public:
	int signers_num;
	int msgs_num;
	Message* mkmsgs;

	MKMessage();
	MKMessage(int signers_num, int msgs_num);

	void setMsgs(Message& msg, int i);

	virtual ~MKMessage();
};

#endif