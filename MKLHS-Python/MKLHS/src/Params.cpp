#include "Params.h"

Params::Params() {
	bn_null(n); 
	bn_new(n);
	g1_get_ord(n);
	// string n_str = "1";
	// n_str.resize( int(300/4), '0' ); // logp=300 (for CKKS), 16=2**4
	// bn_read_str(n, n_str.c_str(), strlen(n_str.c_str()), 16);
	dataset = "database-identifier";
	label = "l";

}

// void Params::setN(string n_str) {
// 	bn_read_str(n, n_str.c_str(), strlen(n_str.c_str()), 16);
// }

void Params::setDataset(string dataset_o) {
	dataset = dataset_o;
}

void Params::setLabel(string label_o) {
	label = label_o;
}

string Params::getNStr() {
	char n_str[100];
	bn_write_str(n_str, 100, n, 16);
	return n_str;
}

void Params::printParams() {
	cout << "\tn: "; bn_print(n);
	cout << "\tdataset_name: " << dataset << endl;
	cout << "\tlabel: " << label << endl;
}

void Params::free() {
	g1_free(h);
	bn_free(n);
	string().swap(dataset);
	string().swap(label);
}

// Params::~Params() {
// 	g1_free(h);
// 	bn_free(n);
// 	delete dataset;
// }



