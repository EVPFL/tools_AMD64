#include <pybind11/pybind11.h>
#include <pybind11/complex.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "LHH.h"
#include <iostream>
using namespace std;


namespace py = pybind11;
using namespace std;


PYBIND11_MODULE(LHH, m)
{
	m.doc() = "LHH For Python.";

	// lhh_test
	m.def("lhh_test", &_test_HomHash);

	// LHHVal
	py::class_<LHHVal>(m, "LHHVal")
		.def(py::init<int>())
		// FUNCTIONS
		.def("setVal", &LHHVal::setVal)
		.def("getVal", (string (LHHVal::*)(int)) &LHHVal::getVal)
		.def("getVal", (string (LHHVal::*)()) &LHHVal::getVal)
		.def("print", (void (LHHVal::*)(int)) &LHHVal::printVal)
		.def("print", (void (LHHVal::*)()) &LHHVal::printVal);

	// LHHMsgs
	py::class_<LHHMsgs>(m, "LHHMsgs")
		.def(py::init<>())
		.def(py::init<int, int>())
		// ATTRIBUTE
		.def_readwrite("msgs_num", &LHHMsgs::msgs_num)
		.def_readwrite("val_len", &LHHMsgs::val_len)
		.def_readwrite("msgs", &LHHMsgs::msgs)
		// FUNCTIONS
		// .def("__getitem__", [](const LHHMsgs& msgs, int i) { return msgs.msgs[i]; })
		.def("reset", &LHHMsgs::reset)
		.def("setMsgs", &LHHMsgs::setMsgs);

	// LHHHash
	py::class_<LHHHash>(m, "LHHHash")
		.def(py::init<>());


	// LHHScheme
	py::class_<LHHScheme>(m, "LHHScheme")
		.def(py::init<int>())
		// GET ATTRIBUTE
		// .def("getValLen", &LHHMsgs::getValLen)
		.def("getModStr", &LHHScheme::getModStr)
		// BASIC HASH FUNCTIONS
		.def("getHashesHex", (string (LHHScheme::*)(const LHHHash&, int)) &LHHScheme::getHashesHex)
		.def("getHashesHex", (string (LHHScheme::*)(const LHHHash&)) &LHHScheme::getHashesHex)
		.def("printHash", (void (LHHScheme::*)(const LHHHash&, int)) &LHHScheme::printHash)
		.def("printHash", (void (LHHScheme::*)(const LHHHash&)) &LHHScheme::printHash)
		.def("resetHashes", &LHHScheme::resetHashes)
		.def("setHashes", (void (LHHScheme::*)(LHHHash&, const LHHHash&, int)) &LHHScheme::setHashes)
		.def("setHashes", (void (LHHScheme::*)(LHHHash&, string, int)) &LHHScheme::setHashes)
		.def("cmpHash", &LHHScheme::cmpHash)

		// COMPUTATION FUNCTIONS
		.def("getHash", &LHHScheme::getHash)
		.def("evalHashes", &LHHScheme::evalHashes)
		.def("evalMsgs", &LHHScheme::evalMsgs)
		.def("verHashes", &LHHScheme::verHashes);

}
