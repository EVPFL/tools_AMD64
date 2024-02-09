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

#include "MKLHS.h"

namespace py = pybind11;
using namespace std;

int init_mklhs(){
	// g1_t h;
	// g1_null(h);
	// g1_new(h);
	// cp_cmlhs_init(h);

	if (core_init() != RLC_OK) {
		core_clean();
		return 0;
	}
	if (pc_param_set_any() != RLC_OK) {
		core_clean();
		return 0;
	}
	return 1;
}

PYBIND11_MODULE(MKLHS, m)
{
	m.doc() = "MKLHS For Python.";

	// init
	m.def("init_mklhs", &init_mklhs);

	// Params
	py::class_<Params>(m, "LHSParams")
		.def(py::init<>())
		// ATTRIBUTE
		.def_readwrite("dataset", &Params::dataset)
		.def_readwrite("label", &Params::label)
		// GET
		.def("getNStr", &Params::getNStr)
		// SETTING
		// .def("initParams", &Params::initParams)
		// .def("setN", &Params::setN)
		.def("setDataset", &Params::setDataset)
		.def("setLabel", &Params::setLabel)
		.def("print", &Params::printParams);

	// SecretKey
	py::class_<SK>(m, "LHSSKey")
		.def(py::init<>())
		.def(py::init<const SK &>())
		// ATTRIBUTE
		.def_readwrite("id", &SK::id)
		// GET
		.def("getKeyStr", &SK::getKeyStr)
		// SETTING
		.def("setID", &SK::setID)
		.def("setKey", (void (SK::*)(string)) &SK::setKey)
		// PRINT
		.def("print", &SK::printKey);

	// PubilcKey
	py::class_<PK>(m, "LHSPKey")
		.def(py::init<>())
		.def(py::init<const PK &>())
		// ATTRIBUTE
		.def_readwrite("id", &PK::id)
		// GET
		.def("getKeyStr", &PK::getKeyStr)
		// SETTING
		.def("setID", &PK::setID)
		.def("setKey", (void (PK::*)(string)) &PK::setKey)
		// PRINT
		.def("print", &PK::printKey);

	// MKPK
	py::class_<MKPK>(m, "MKLHSPKeys")
		.def(py::init<>())
		.def(py::init<int>())
		// ATTRIBUTE
		.def_readwrite("signers_num", &MKPK::signers_num)
		.def("__getitem__", [](const MKPK& mkpk, int i) { return mkpk.mkpks[i]; })
		// SETTING
		.def("setPKs", &MKPK::setPKs);

	// Message
	py::class_<Message>(m, "LHSMsgs")
		.def(py::init<>())
		.def(py::init<int>())
		.def(py::init<const Message &>())
		// ATTRIBUTE
		.def_readwrite("size", &Message::size)
		.def("getMsgsStr", &Message::getMsgsStr)
		// SET
		// .def("setMsg", &Message::setMsg);
		.def("setMsg", (void (Message::*)(bn_t, int, const Params&)) & Message::setMsg)
		.def("setMsg", (void (Message::*)(long, int, const Params&)) & Message::setMsg)
		.def("setMsg", (void (Message::*)(string, int, const Params&)) & Message::setMsg)
		.def("setMsg", (void (Message::*)(string, int, string)) & Message::setMsg)
		.def("setMsg", (void (Message::*)(string, int, int, const Params&)) & Message::setMsg)
		// PRINT
		.def("print", (void (Message::*)()) & Message::printMsgs)
		.def("print", (void (Message::*)(int)) & Message::printMsgs)
		.def("print", (void (Message::*)(int, int)) & Message::printMsgs);

	// MKMessage
	py::class_<MKMessage>(m, "MKLHSMsgs")
		.def(py::init<>())
		.def(py::init<int, int>())
		// ATTRIBUTE
		.def_readwrite("signers_num", &MKMessage::signers_num)
		.def_readwrite("msgs_num", &MKMessage::msgs_num)
		.def("__getitem__", [](const MKMessage& mkmsg, int i) { return mkmsg.mkmsgs[i]; })
		// SET
		.def("setMsgs", &MKMessage::setMsgs);

		
	// Signature
	py::class_<Signature>(m, "LHSSigns")
		.def(py::init<>())
		.def(py::init<int>())
		.def(py::init<const Signature &>())
		// ATTRIBUTE
		.def_readwrite("size", &Signature::size)
		.def("getSignStr", (string (Signature::*)(int)) &Signature::getSignStr)
		.def("getSignStr", (string (Signature::*)()) &Signature::getSignStr)
		// SET
		.def("setSign", (void (Signature::*)(string, int)) & Signature::setSign)
		.def("setSign", (void (Signature::*)(string)) & Signature::setSign)
		// PRINT
		.def("print", (void (Signature::*)()) & Signature::printSigns)
		.def("print", (void (Signature::*)(int)) & Signature::printSigns)
		.def("print", (void (Signature::*)(int, int)) & Signature::printSigns);

	// MKSignature
	py::class_<MKSignature>(m, "MKLHSSigns")
		.def(py::init<>())
		.def(py::init<int>())
		.def(py::init<int, int>())
		// ATTRIBUTE
		.def_readwrite("signers_num", &MKSignature::signers_num)
		.def_readwrite("msgs_num", &MKSignature::msgs_num)
		.def("__getitem__", [](const MKSignature& mksign, int i) {  return mksign.mksigns[i]; })
		// SETTING
		.def("setSigns", &MKSignature::setSigns);

	// Linearity
	py::class_<Linearity>(m, "LHSLinearity")
		.def(py::init<>())
		.def(py::init<int>())
		.def(py::init<const Linearity &>())
		// ATTRIBUTE
		.def_readwrite("size", &Linearity::size)
		.def("__getitem__", &Linearity::getCoeff)
		// SET
		.def("setCoeff", &Linearity::setCoeff)
		.def("copyCoeff", &Linearity::copyCoeff)
		// PRINT
		.def("print", (void (Linearity::*)()) & Linearity::printLinearity)
		.def("print", (void (Linearity::*)(int)) & Linearity::printLinearity)
		.def("print", (void (Linearity::*)(int, int)) & Linearity::printLinearity);

	// MKLinearity
	py::class_<MKLinearity>(m, "MKLHSLinearity")
		.def(py::init<>())
		.def(py::init<int, int>())
		// ATTRIBUTE
		.def_readwrite("signers_num", &MKLinearity::signers_num)
		.def_readwrite("msgs_num", &MKLinearity::msgs_num)
		// .def("__getitem__", [](const MKLinearity& mkf, int i) { return mkf.fs.at(i); })
		.def("__getitem__", [](const MKLinearity& mkf, int i) { return mkf.fs[i]; })
		// SET
		.def("setCoeffs", &MKLinearity::setCoeffs);

	// LHSScheme
	py::class_<LHSScheme>(m, "LHSScheme")
		// .def(py::init<>())
		.def(py::init<const Params &>())
		// ATTRIBUTE
		.def_readwrite("params", &LHSScheme::params)
		// FUNCTIONS
		.def("keyGen",(void (LHSScheme::*)(SK&, PK&)) &LHSScheme::keyGen)
		.def("keyGen",(void (LHSScheme::*)(SK&, PK&, string)) &LHSScheme::keyGen)

		.def("signMsgs",(void (LHSScheme::*)(Signature&, Message&, SK&)) &LHSScheme::signMsgs)
		.def("signMsgs",(void (LHSScheme::*)(Signature&, Message&, SK&, string)) &LHSScheme::signMsgs)

		.def("evalSign", &LHSScheme::evalSign)
		.def("evalSigns", &LHSScheme::evalSigns)

		.def("evalMsg", &LHSScheme::evalMsg)
		.def("evalMsgs",(void (LHSScheme::*)(Message&, MKMessage&, MKLinearity&)) &LHSScheme::evalMsgs)
		// .def("evalMsgs",(void (LHSScheme::*)(Message&, bn_t*, MKMessage&, MKLinearity&)) &LHSScheme::evalMsgs)
		.def("verMKLHS", (int (LHSScheme::*)(Signature&, MKSignature&, MKMessage&, MKLinearity&, MKPK&)) &LHSScheme::verMKLHS)
		.def("verMKLHS", (int (LHSScheme::*)(Signature&, MKMessage&, MKLinearity&, MKPK&)) &LHSScheme::verMKLHS);

}
