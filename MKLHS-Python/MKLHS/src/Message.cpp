#include "Message.h"

Message::Message() {
	size = 0;
}

Message::Message(int msg_size) {
	size = msg_size;
	msgs = (bn_t*)malloc(sizeof(bn_t)*size);
	for (int i = 0; i < msg_size; i++){
		bn_null(msgs[i]);
		bn_new(msgs[i]);
	}
}

Message::Message(const Message& o){
	size = o.size;
	msgs = (bn_t*)malloc(sizeof(bn_t)*size);
	for (int i = 0; i < size; i++){
		bn_null(msgs[i]);
		bn_new(msgs[i]);
		bn_copy(msgs[i], o.msgs[i]);
	}
}

void Message::resetMsg(int msg_size) {
	size = msg_size;
	msgs = (bn_t*)malloc(sizeof(bn_t)*size);
	for (int i = 0; i < msg_size; i++){
		bn_null(msgs[i]);
		bn_new(msgs[i]);
	}
}

void Message::setMsg(bn_t msg, int i, const Params& params) {
	if(size == 0){
		size = 1;
		msgs = (bn_t*)malloc(sizeof(bn_t)*size);
		bn_null(msgs[0]); 
		bn_new(msgs[0]);
	}
	if(i >= size){ 
		i = 0;
	}
	bn_copy(msgs[i], msg); 
	bn_mod(msgs[i], msgs[i], params.n);
}

void Message::setMsg(long msg_dig, int i, const Params& params) {
	// cout << "n:";
	// bn_print(params.n);
	if(size == 0){
		size = 1;
		msgs = (bn_t*)malloc(sizeof(bn_t)*size);
		bn_null(msgs[0]); 
		bn_new(msgs[0]);
	}
	if(i >= size){ 
		i = 0;
	}
	bn_set_dig(msgs[i], msg_dig);
	bn_mod(msgs[i], msgs[i], params.n);
	// cout << "set msg[" << i << "]: ";
	// bn_print(msgs[i]);
}

void Message::setMsg(string msg_dig_str, int i, const Params& params) {
	if(size == 0){
		size = 1;
		msgs = (bn_t*)malloc(sizeof(bn_t)*size);
		bn_null(msgs[0]); 
		bn_new(msgs[0]);
	}
	if(i >= size){ 
		i = 0;
	}
	bn_read_str(msgs[i], msg_dig_str.c_str(), strlen(msg_dig_str.c_str()), 16);
	bn_mod(msgs[i], msgs[i], params.n);
}

void Message::setMsg(string msg_dig_str, int i, string n_str) {
	if(size == 0){
		size = 1;
		msgs = (bn_t*)malloc(sizeof(bn_t)*size);
		bn_null(msgs[0]); 
		bn_new(msgs[0]);
	}
	if(i >= size){ 
		i = 0;
	}
	bn_t n;
	bn_null(n); bn_new(n);
	bn_read_str(n, n_str.c_str(), strlen(n_str.c_str()), 16);
	bn_read_str(msgs[i], msg_dig_str.c_str(), strlen(msg_dig_str.c_str()), 16);
	bn_mod(msgs[i], msgs[i], n);
}


void Message::setMsg(string msg_dig_str, int str_len, int i, const Params& params) {
	// cout << "msg_dig_str:" << msg_dig_str << endl;
	if(size == 0){
		size = 1;
		msgs = (bn_t*)malloc(sizeof(bn_t)*size);
		bn_null(msgs[0]); 
		bn_new(msgs[0]);
	}
	if(i >= size){ 
		i = 0;
	}
	bn_read_str(msgs[i], msg_dig_str.c_str(), str_len, 16);
	bn_mod(msgs[i], msgs[i], params.n);
}


string Message::getMsgsStr() {
	string msgs_str = "";
	for (int i = 0; i < size; i++){
		char msg_str[100];
		bn_write_str(msg_str, 100, msgs[i], 16);
		msgs_str += msg_str;
		msgs_str += "-" ;
	}
	// cout << "msgs_str:" << msgs_str << endl;
	return msgs_str;
}

void Message::printMsgs() {
	for (int i = 0; i < size; i++){
		cout << "msg[" << i << "]: ";
		bn_print(msgs[i]);
	}
}

void Message::printMsgs(int msgs_num) {
	if (msgs_num > size){
		msgs_num = size;
		cout << "number of msgs: " << size << "." << endl;
	}
	for (int i = 0; i < msgs_num; i++){
		cout << "msg[" << i << "]: ";
		bn_print(msgs[i]);
	}
}

void Message::printMsgs(int start_inx, int end_inx) {
	while (start_inx < 0 and size!=0 ){
		start_inx += size;
	}
	while (end_inx < 0 and size!=0 ){
		end_inx += size;
	}
	if(end_inx > size){
		end_inx = size;
	}
	if(end_inx < start_inx){
		int tmp = end_inx;
		end_inx = start_inx;
		start_inx = tmp;
	}

	for (int i = start_inx; i < end_inx; i++){
		cout << "msg[" << i << "]: ";
		bn_print(msgs[i]);
	}
}


Message::~Message() {
	for (int i = 0; i < size; i++){
		bn_free(msgs[i]);
	}
}



MKMessage::MKMessage() {
	signers_num = 0;
	msgs_num = 0;
}

MKMessage::MKMessage(int s_num, int m_num) {
	signers_num = s_num;
	msgs_num = m_num;
	
	mkmsgs = new Message[signers_num]; 
	for (int i = 0; i < signers_num; i++){
		mkmsgs[i] = Message(msgs_num);
	}
	
}

void MKMessage::setMsgs(Message& msg, int i) {
	if(signers_num == 0){
		signers_num = 1;
	}
	if(i >= signers_num){ 
		i = 0;
	}
	mkmsgs[i] = msg;
}


MKMessage::~MKMessage() {
	delete[] mkmsgs;
}





