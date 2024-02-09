#include <pybind11/pybind11.h>
#include <pybind11/complex.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <malloc.h>
#include "HEAAN.h"
#include "base64.h"

namespace py = pybind11;

using namespace std;
using namespace NTL;

PYBIND11_MAKE_OPAQUE(complex<double>);
PYBIND11_MAKE_OPAQUE(double);
PYBIND11_MAKE_OPAQUE(NTL::ZZ);
PYBIND11_MAKE_OPAQUE(std::uint64_t);

using ComplexDouble = complex<double>;
using Double = double;
using ZZ = NTL::ZZ;
using Uint64_t = std::uint64_t;

std::string zToString(const ZZ &z)
{
	std::stringstream buffer;
	buffer << z;
	return buffer.str();
}

std::string uintToString(const Uint64_t &z)
{
	std::stringstream buffer;
	buffer << z;
	return buffer.str();
}

PYBIND11_MODULE(HEAAN, m)
{
	m.doc() = "HEAAN For Python.";

	// Uint64_t
	py::class_<Uint64_t, std::unique_ptr<Uint64_t, py::nodelete>>(m, "Uint64_t", py::buffer_protocol())
		.def_buffer([](Uint64_t *v) -> py::buffer_info {
			return py::buffer_info(
				v,
				sizeof(std::uint64_t),
				py::format_descriptor<std::uint64_t>::format(),
				1,
				{malloc_usable_size(v) / sizeof(std::uint64_t)},
				{sizeof(std::uint64_t)});
		})
		.def(py::init<>())
		.def(py::init([](int len) { return new uint64_t[len]; }))
		.def(py::init([](py::array_t<uint64_t> in) {
			uint64_t *out = new uint64_t[in.size()];
			py::buffer_info in_info = in.request();
			uint64_t *in_ptr = (uint64_t *)in_info.ptr;
			NTL_EXEC_RANGE(in_info.size, first, last)
			for (auto i = first; i < last; i++)
			{
				out[i] = in_ptr[i];
			}
			NTL_EXEC_RANGE_END
			return out;
		}))
		.def("__int__", [](Uint64_t z) { return long(z); })
		.def("size", [](Uint64_t *z) { return long(malloc_usable_size(z)/sizeof(std::uint64_t)); })
		.def("__getitem__", [](const Uint64_t *z, std::int64_t i) { return uintToString(z[i]); })
		.def("setitem", [](Uint64_t *z, std::int64_t i, std::uint64_t item) { z[i] = item; })
		.def("copy", [](Uint64_t *z1, Uint64_t *z2) {
			long z1_size = long(malloc_usable_size(z1)/sizeof(std::uint64_t));
			long copy_size = long(malloc_usable_size(z2)/sizeof(std::uint64_t));
			if (z1_size < copy_size) { copy_size = z1_size; }
			for (long i = 0; i < copy_size; ++i) {
				z1[i] = int(z2[i]); 
			}
		})
		.def("free",[](Uint64_t *z) { delete[] z;} )
		.def("print", [](const uint64_t *vals, long size = 5) {
			std::cout << "[";
			std::cout << vals[0];
			for (long i = 1; i < size; ++i)
			{
				std::cout << ", " << vals[i];
			}
			std::cout << "]" << std::endl;
		});

	// ZZ pointer array
	py::class_<ZZ, std::unique_ptr<ZZ, py::nodelete>>(m, "ZZ")
		.def(py::init<>())
		//.def(py::init([](std::uint64_t len) { return new NTL::ZZ[len]; }))
		.def(py::init([](int len) { return new NTL::ZZ[len]; }))
		.def(py::init([](py::array_t<std::int64_t> in) {
			NTL::ZZ *out = new NTL::ZZ[in.size()];
			py::buffer_info in_info = in.request();
			std::int64_t *in_ptr = (std::int64_t *)in_info.ptr;

			NTL_EXEC_RANGE(in_info.size, first, last)
			for (auto i = first; i < last; i++)
			{
				//out[i] = NTL::to_ZZ(in_ptr[i]);
				out[i] = NTL::ZZ(in_ptr[i]);
			}
			NTL_EXEC_RANGE_END

			return out;
		}))
		.def(py::init([](py::list in) {
			NTL::ZZ *out = new NTL::ZZ[in.size()];
			long i = 0;
			for (py::handle obj : in){ //iterators
				NTL::ZZ obj_num(NTL::INIT_VAL, obj.attr("__str__")().cast<std::string>().c_str());
				out[i++] = obj_num;
			}
			return out;
		}))
		.def("__getitem__", [](const ZZ *z, std::int64_t i) { return zToString(z[i]); })
		.def("setitem", [](ZZ *z, std::int64_t i, ZZ& zz) { z[i] = zz; })
		.def("free",[](ZZ *z) { delete[] z; } )
		// PRINT
		.def("print", [](const ZZ *vals, long size = 5) {
			std::cout << "[";
			std::cout << vals[0];
			for (long i = 1; i < size; ++i)
			{
				std::cout << ", " << vals[i];
			}
			std::cout << "]" << std::endl;
		})
		// PRINT: index and value of the non-zero element in ZZ array
		.def("printInxs", [](const ZZ *vals, long size = 10) {
			int count = 0;
			std::cout << "[";
			for (long i = 0; i < N; ++i) {
				if (vals[i] != 0) {
					if (count == 0) { std::cout << i << ": " << vals[i]; }
					else { std::cout << ", " << i << ": " << vals[i]; }
					count++;
				}
				if (count == size) { break; }
			}
			std::cout << "]" << std::endl;
		});

	// ComplexDouble
	py::class_<ComplexDouble>(m, "ComplexDouble", py::buffer_protocol())
		.def_buffer([](ComplexDouble *v) -> py::buffer_info {
			return py::buffer_info(
				v,
				sizeof(std::complex<double>),
				py::format_descriptor<std::complex<double>>::format(),
				1,
				{malloc_usable_size(v) / sizeof(std::complex<double>)},
				{sizeof(std::complex<double>)});
		})
		.def(py::init<>())
		//.def(py::init([](std::uint64_t len) { return new complex<double>[len]; }))
		.def(py::init([](int len) { return new complex<double>[len]; }))
		.def(py::init([](py::array_t<complex<double>> in) {
			complex<double> *out = new complex<double>[in.size()];
			py::buffer_info in_info = in.request();
			complex<double> *in_ptr = (complex<double> *)in_info.ptr;

			NTL_EXEC_RANGE(in_info.size, first, last)
			for (auto i = first; i < last; i++)
			{
				out[i] = in_ptr[i];
			}
			NTL_EXEC_RANGE_END

			return out;
		}))
		.def("real", [](const complex<double> z) { return z.real(); })
		.def("imag", [](const complex<double> z) { return z.imag(); })
		.def("size", [](ComplexDouble *z) { return long(malloc_usable_size(z)/sizeof(std::complex<double>)); })
		.def("__getitem__", [](const ComplexDouble *z, std::int64_t i) { return z[i]; })
		.def("__repr__", [](const ComplexDouble &a) {
			return "(" + to_string(a.real()) + ", " + to_string(a.imag()) + ")";
		})
		.def("free",[](ComplexDouble *z) { delete[] z; } )
		.def("print", [](const complex<double> *vals, long size = 5) {
			std::cout << "[";
			std::cout << vals[0];
			for (long i = 1; i < size; ++i)
			{
				std::cout << ", " << vals[i];
			}
			std::cout << "]" << std::endl;
		});

	// Double
	py::class_<Double>(m, "Double", py::buffer_protocol())
		.def_buffer([](Double *v) -> py::buffer_info {
			return py::buffer_info(
				v,
				sizeof(double),
				py::format_descriptor<double>::format(),
				1,
				{malloc_usable_size(v) / sizeof(double)}, // when the length is even, the return from malloc will increase by 1
				{sizeof(double)});
		})
		.def(py::init<>())
		.def(py::init([](std::uint64_t len) { return new double[len]; }))
		.def(py::init([](double i) { return double(i); }))
		.def(py::init([](py::array_t<double> in) {
			double *out = new double[in.size()];
			py::buffer_info in_info = in.request();
			double *in_ptr = (double *)in_info.ptr;

			NTL_EXEC_RANGE(in_info.size, first, last)
			for (auto i = first; i < last; i++)
			{
				out[i] = in_ptr[i];
			}
			NTL_EXEC_RANGE_END

			return out;
		}))
		.def("__getitem__", [](const Double *z, std::int64_t i) { return z[i]; })
		.def("__repr__", [](const Double &a) { return to_string(a); })
		.def("free", [](Double *z) { delete[] z; } )
		.def("print", [](const double *vals, long size = 5) {
			std::cout << "[";
			std::cout << vals[0];
			for (long i = 1; i < size; ++i)
			{
				std::cout << ", " << vals[i];
			}
			std::cout << "]" << std::endl;
		});

	//Params
	m.attr("logN") = getParamslogN();
	m.attr("logQ") = getParamslogQ();
	m.attr("sigma") = getParamssigma();
	m.attr("h") = getParamsh();
	m.attr("pbnd") = getParamspbnd();
	m.attr("kbar") = getParamskbar();
	m.attr("kbar2") = getParamskbar2();
	m.attr("logNh") = getParamslogNh();
	m.attr("logQQ") = getParamslogQQ();
	m.attr("N") = getParamsN();
	m.attr("Nh") = getParamsNh();
	m.attr("M") = getParamsM();
	m.attr("nprimes") = getParamsnprimes();
	m.attr("Nnprimes") = getParamsNnprimes();
	m.attr("cbnd") = getParamscbnd();
	m.attr("bignum") = getParamsbignum();
	m.attr("Q") = getParamsQ();
	m.attr("QQ") = getParamsQQ();

	// TestScheme
	py::class_<TestScheme>(m, "TestScheme")
		// STANDARD TESTS
		.def("testEncrypt", &TestScheme::testEncrypt)
		.def("testEncryptSingle", &TestScheme::testEncryptSingle)
		.def("testAdd", &TestScheme::testAdd)
		.def("testMult", &TestScheme::testMult)
		.def("testiMult", &TestScheme::testiMult)
		// ROTATE & CONJUGATE TESTS
		.def("testRotateFast", &TestScheme::testRotateFast)
		.def("testConjugate", &TestScheme::testConjugate)
		// POWER & PRODUCT TESTS
		.def("testPowerOf2", &TestScheme::testPowerOf2)
		.def("testPower", &TestScheme::testPower)
		// FUNCTION TESTS
		.def("testInverse", &TestScheme::testInverse)
		.def("testLogarithm", &TestScheme::testLogarithm)
		.def("testExponent", &TestScheme::testExponent)
		.def("testExponentLazy", &TestScheme::testExponentLazy)
		.def("testSigmoid", &TestScheme::testSigmoid)
		.def("testSigmoidLazy", &TestScheme::testSigmoidLazy)
		// BOOTSTRAPPING TESTS
		.def("testBootstrap", &TestScheme::testBootstrap)
		.def("testBootstrapSingleReal", &TestScheme::testBootstrapSingleReal)
		.def("testWriteAndRead", &TestScheme::testWriteAndRead);

	// MPScheme
	py::class_<MPScheme>(m, "MPScheme")
		.def(py::init<Ring &>(), py::arg())
		// KEYS GENERATION
		.def("getKey", (Key (MPScheme::*)(SecretKey &, long)) & MPScheme::getKey, py::return_value_policy::take_ownership)
		.def("getKey", (Key (MPScheme::*)(SecretKey &, long, long)) & MPScheme::getKey, py::return_value_policy::take_ownership)
		.def("getEncKey", (Key (MPScheme::*)(SecretKey &)) & MPScheme::getEncKey, py::return_value_policy::take_ownership)
		.def("getEncKey", (Key (MPScheme::*)(SecretKey &, ZZ *)) & MPScheme::getEncKey, py::return_value_policy::take_ownership)
		.def("getEncKey", (Key (MPScheme::*)(SecretKey &, Uint64_t *)) & MPScheme::getEncKey, py::return_value_policy::take_ownership)
		//.def("getMultKey", &MPScheme::getMultKey, py::return_value_policy::take_ownership)
		.def("getMultKey", (Key (MPScheme::*)(SecretKey &)) & MPScheme::getMultKey, py::return_value_policy::take_ownership)
		.def("getMultKey", (Key (MPScheme::*)(SecretKey &, ZZ *)) & MPScheme::getMultKey, py::return_value_policy::take_ownership)
		.def("getMultKey", (Key (MPScheme::*)(SecretKey &, Uint64_t *)) & MPScheme::getMultKey, py::return_value_policy::take_ownership)
		.def("getConjKey", &MPScheme::getConjKey, py::return_value_policy::take_ownership)
		//.def("getLeftRotKey", &MPScheme::getLeftRotKey, py::return_value_policy::take_ownership)
		.def("getLeftRotKey", (Key (MPScheme::*)(SecretKey &, long)) & MPScheme::getLeftRotKey, py::return_value_policy::take_ownership)
		.def("getLeftRotKey", (Key (MPScheme::*)(SecretKey &, long, ZZ *)) & MPScheme::getLeftRotKey, py::return_value_policy::take_ownership)
		.def("getLeftRotKey", (Key (MPScheme::*)(SecretKey &, long, Uint64_t *)) & MPScheme::getLeftRotKey, py::return_value_policy::take_ownership)
		
		.def("getRightRotKey", &MPScheme::getRightRotKey, py::return_value_policy::take_ownership)
		.def("getLeftRotKeys", &MPScheme::getLeftRotKeys, py::return_value_policy::take_ownership)
		.def("getRightRotKeys", &MPScheme::getRightRotKeys, py::return_value_policy::take_ownership)
		// ENCODING & DECODING
		.def("encode", (void (MPScheme::*)(Plaintext &, complex<double> *, long, long, long)) & MPScheme::encode)
		.def("encode", (void (MPScheme::*)(Plaintext &, double *, long, long, long)) & MPScheme::encode)
		.def("decode", (complex<double> * (MPScheme::*)(Plaintext &)) & MPScheme::decode)
		.def("encodeSingle", (void (MPScheme::*)(Plaintext &, complex<double>, long, long)) & MPScheme::encodeSingle)
		.def("encodeSingle", (void (MPScheme::*)(Plaintext &, double, long, long)) & MPScheme::encodeSingle)
		.def("decodeSingle", (complex<double>(MPScheme::*)(Plaintext &, double *, long, long, long)) & MPScheme::decodeSingle)
		// ENCRYPTION & DECRYPTION
		// basic
		.def("encryptMsg", (void (MPScheme::*)(Ciphertext &, Plaintext &, Key &)) & MPScheme::encryptMsg)
		.def("decryptMsg", (void (MPScheme::*)(Plaintext &, SecretKey &, Ciphertext &)) & MPScheme::decryptMsg)
		.def("encrypt", (void (MPScheme::*)(Ciphertext &, complex<double> *, Key &, long, long, long)) & MPScheme::encrypt)
		.def("encrypt", (void (MPScheme::*)(Ciphertext &, double *, Key &, long, long, long)) & MPScheme::encrypt)
		.def("decrypt", (complex<double> * (MPScheme::*)(SecretKey &, Ciphertext &)) & MPScheme::decrypt)
		.def("encryptZeros", (void (MPScheme::*)(Ciphertext &, Key &, long, long, long)) & MPScheme::encryptZeros)
		.def("encryptSingle", (void (MPScheme::*)(Ciphertext &, complex<double>, Key &, long, long)) & MPScheme::encryptSingle)
		.def("encryptSingle", (void (MPScheme::*)(Ciphertext &, double, Key &, long, long)) & MPScheme::encryptSingle)
		.def("decryptSingle", (complex<double>(MPScheme::*)(SecretKey &, Ciphertext &)) & MPScheme::decryptSingle)
		// using fixed random vector
		.def("encryptWithRV", (void (MPScheme::*)(Ciphertext &, complex<double> *, Key &, long, long, long, ZZ *)) & MPScheme::encryptWithRV)
		.def("encryptWithRV", (void (MPScheme::*)(Ciphertext &, double *, Key &, long, long, long, ZZ *)) & MPScheme::encryptWithRV)
		// multiple messages
		.def("encrypt", (void (MPScheme::*)(Ciphertexts &, complex<double> *, Key &, long, long, long, long)) & MPScheme::encrypt)
		.def("encrypt", (void (MPScheme::*)(Ciphertexts &, double *, Key &, long, long, long, long)) & MPScheme::encrypt)
		.def("encrypt", (void (MPScheme::*)(Ciphertexts &, complex<double> *, Key &, long, long, long, long, ZZ *)) & MPScheme::encrypt)
		.def("encrypt", (void (MPScheme::*)(Ciphertexts &, double *, Key &, long, long, long, long, ZZ *)) & MPScheme::encrypt)
		// multiple threads
		.def("encrypt", (void (MPScheme::*)(Ciphertexts &, complex<double> *, Key &, long, long, long, long, int)) & MPScheme::encrypt)
		.def("encryptWithRV", (void (MPScheme::*)(Ciphertexts &, complex<double> *, Key &, long, long, long, long, int, ZZ *)) & MPScheme::encryptWithRV)
		// sk shares
		.def("shareSecret", &MPScheme::shareSecret)
		.def("shareSecretKey", &MPScheme::shareSecretKey)
		.def("recoverSecret", (void (MPScheme::*)(uint64_t*, SKShares&)) & MPScheme::recoverSecret)
		.def("recoverSecret", (void (MPScheme::*)(uint64_t*, SKShares&, long)) & MPScheme::recoverSecret)
		.def("recoverSecret", (void (MPScheme::*)(ZZ*, long, SKShares&)) & MPScheme::recoverSecret)
		.def("recoverSecret", (void (MPScheme::*)(ZZ*, long, SKShares&, long)) & MPScheme::recoverSecret);


	// Scheme
	py::class_<Scheme>(m, "Scheme")
		.def(py::init<SecretKey &, Ring &, bool>(), py::arg(), py::arg(), py::arg("isSerialized") = false)
		.def(py::init<Ring &, bool>(), py::arg(), py::arg("isSerialized") = false)
		// .def(py::pickle(&serialize<Scheme>, &deserialize<Scheme>))
		// ATTRIBUTE
		.def_readwrite("keyMap", &Scheme::keyMap)
		// KEYS GENERATION
		.def("addEncKey", &Scheme::addEncKey)
		.def("addMultKey", &Scheme::addMultKey)
		.def("addConjKey", &Scheme::addConjKey)
		.def("addLeftRotKey", &Scheme::addLeftRotKey)
		.def("addRightRotKey", &Scheme::addRightRotKey)
		.def("addLeftRotKeys", &Scheme::addLeftRotKeys)
		.def("addRightRotKeys", &Scheme::addRightRotKeys)
		.def("addBootKey", &Scheme::addBootKey)
		// ENCODING & DECODING
		.def("encode", (void (Scheme::*)(Plaintext &, complex<double> *, long, long, long)) & Scheme::encode)
		.def("encode", (void (Scheme::*)(Plaintext &, double *, long, long, long)) & Scheme::encode)
		.def("decode", (complex<double> * (Scheme::*)(Plaintext &)) & Scheme::decode)
		.def("encodeSingle", (void (Scheme::*)(Plaintext &, complex<double>, long, long)) & Scheme::encodeSingle)
		.def("encodeSingle", (void (Scheme::*)(Plaintext &, double, long, long)) & Scheme::encodeSingle)
		.def("decodeSingle", (complex<double>(Scheme::*)(Plaintext &, double *, long, long, long)) & Scheme::decodeSingle)
		// ENCRYPTION & DECRYPTION
		.def("encryptMsg", (void (Scheme::*)(Ciphertext &, Plaintext &)) & Scheme::encryptMsg)
		.def("decryptMsg", (void (Scheme::*)(Plaintext &, SecretKey &, Ciphertext &)) & Scheme::decryptMsg)
		.def("encrypt", (void (Scheme::*)(Ciphertext &, complex<double> *, long, long, long)) & Scheme::encrypt)
		.def("encrypt", (void (Scheme::*)(Ciphertext &, double *, long, long, long)) & Scheme::encrypt)
		.def("encryptZeros", (void (Scheme::*)(Ciphertext &, long, long, long)) & Scheme::encryptZeros)
		.def("decrypt", (complex<double> * (Scheme::*)(SecretKey &, Ciphertext &)) & Scheme::decrypt)
		.def("encryptSingle", (void (Scheme::*)(Ciphertext &, complex<double>, long, long)) & Scheme::encryptSingle)
		.def("encryptSingle", (void (Scheme::*)(Ciphertext &, double, long, long)) & Scheme::encryptSingle)
		.def("decryptSingle", (complex<double>(Scheme::*)(SecretKey &, Ciphertext &)) & Scheme::decryptSingle)
		// HOMOMORPHIC OPERATIONS
		.def("negate", &Scheme::negate)
		.def("negateAndEqual", &Scheme::negateAndEqual)
		.def("add", &Scheme::add)
		.def("addAndEqual", &Scheme::addAndEqual)
		.def("addConst", (void (Scheme::*)(Ciphertext &, Ciphertext &, double, long)) & Scheme::addConst)
		.def("addConst", (void (Scheme::*)(Ciphertext &, Ciphertext &, NTL::RR &, long)) & Scheme::addConst)
		.def("addConst", (void (Scheme::*)(Ciphertext &, Ciphertext &, complex<double>, long)) & Scheme::addConst)
		.def("addConstAndEqual", (void (Scheme::*)(Ciphertext &, double, long)) & Scheme::addConstAndEqual)
		.def("addConstAndEqual", (void (Scheme::*)(Ciphertext &, NTL::RR &, long)) & Scheme::addConstAndEqual)
		.def("addConstAndEqual", (void (Scheme::*)(Ciphertext &, complex<double>, long)) & Scheme::addConstAndEqual)
		.def("sub", &Scheme::sub)
		.def("subAndEqual", &Scheme::subAndEqual)
		.def("subAndEqual2", &Scheme::subAndEqual2)
		.def("imult", &Scheme::imult)
		.def("idiv", &Scheme::idiv)
		.def("imultAndEqual", &Scheme::imultAndEqual)
		.def("idivAndEqual", &Scheme::idivAndEqual)
		//.def("mult", &Scheme::mult)
		.def("mult", (void (Scheme::*)(Ciphertext &, Ciphertext &, Ciphertext &)) &Scheme::mult)
		.def("mult", (void (Scheme::*)(Ciphertext &, Ciphertext &, Ciphertext &, Key &)) &Scheme::mult)
		//.def("multAndEqual", &Scheme::multAndEqual)
		.def("multAndEqual", (void (Scheme::*)(Ciphertext &, Ciphertext &)) &Scheme::multAndEqual)
		.def("multAndEqual", (void (Scheme::*)(Ciphertext &, Ciphertext &, Key &)) &Scheme::multAndEqual)
		//.def("square", &Scheme::square)
		.def("square", (void (Scheme::*)(Ciphertext &, Ciphertext &)) &Scheme::square)
		.def("square", (void (Scheme::*)(Ciphertext &, Ciphertext &, Key &)) &Scheme::square)
		//.def("squareAndEqual", &Scheme::squareAndEqual)
		.def("squareAndEqual", (void (Scheme::*)(Ciphertext &)) &Scheme::squareAndEqual)
		.def("squareAndEqual", (void (Scheme::*)(Ciphertext &, Key &)) &Scheme::squareAndEqual)
		.def("multByConst", (void (Scheme::*)(Ciphertext &, Ciphertext &, double, long)) & Scheme::multByConst)
		.def("multByConst", (void (Scheme::*)(Ciphertext &, Ciphertext &, complex<double>, long)) & Scheme::multByConst)
		.def("multByConstVec", &Scheme::multByConstVec)
		.def("multByConstVecAndEqual", &Scheme::multByConstVecAndEqual)
		.def("multByConstAndEqual", (void (Scheme::*)(Ciphertext &, double, long)) & Scheme::multByConstAndEqual)
		.def("multByConstAndEqual", (void (Scheme::*)(Ciphertext &, NTL::RR &, long)) & Scheme::multByConstAndEqual)
		.def("multByConstAndEqual", (void (Scheme::*)(Ciphertext &, complex<double>, long)) & Scheme::multByConstAndEqual)
		.def("multByPoly", &Scheme::multByPoly)
		.def("multByPolyNTT", &Scheme::multByPolyNTT)
		.def("multByPolyAndEqual", &Scheme::multByPolyAndEqual)
		.def("multByPolyNTTAndEqual", &Scheme::multByPolyNTTAndEqual)
		.def("multByMonomial", &Scheme::multByMonomial)
		.def("multByMonomialAndEqual", &Scheme::multByMonomialAndEqual)
		.def("leftShift", &Scheme::leftShift)
		.def("leftShiftAndEqual", &Scheme::leftShiftAndEqual)
		.def("doubleAndEqual", &Scheme::doubleAndEqual)
		.def("divByPo2", &Scheme::divByPo2)
		.def("divByPo2AndEqual", &Scheme::divByPo2AndEqual)
		// RESCALING
		.def("reScaleBy", &Scheme::reScaleBy)
		.def("reScaleTo", &Scheme::reScaleTo)
		.def("reScaleByAndEqual", &Scheme::reScaleByAndEqual)
		.def("reScaleToAndEqual", &Scheme::reScaleToAndEqual)
		.def("modDownBy", &Scheme::modDownBy)
		.def("modDownByAndEqual", &Scheme::modDownByAndEqual)
		.def("modDownTo", &Scheme::modDownTo)
		.def("modDownToAndEqual", &Scheme::modDownToAndEqual)
		// ROTATIONS & CONJUGATIONS
		.def("leftRotateFast", &Scheme::leftRotateFast)
		.def("rightRotateFast", &Scheme::rightRotateFast)
		.def("leftRotateFastAndEqual", (void (Scheme::*)(Ciphertext &, long, Key &)) &Scheme::leftRotateFastAndEqual)
		.def("rightRotateFastAndEqual", &Scheme::rightRotateFastAndEqual)
		.def("conjugate", &Scheme::conjugate)
		.def("conjugateAndEqual", &Scheme::conjugateAndEqual)
		// BOOTSTRAPPING
		.def("normalizeAndEqual", &Scheme::normalizeAndEqual)
		.def("coeffToSlotAndEqual", &Scheme::coeffToSlotAndEqual)
		.def("slotToCoeffAndEqual", &Scheme::slotToCoeffAndEqual)
		.def("exp2piAndEqual", &Scheme::exp2piAndEqual)
		.def("evalExpAndEqual", &Scheme::evalExpAndEqual)
		.def("bootstrapAndEqual", &Scheme::bootstrapAndEqual)
		// FOR EVALUATION
		.def("allSum", (void (Scheme::*)(Ciphertext &, Ciphertext &, Key &, long)) &Scheme::allSum)
		.def("allSum", (void (Scheme::*)(Ciphertext &, Ciphertext&, Key&, Key&, long, int)) &Scheme::allSum)
		.def("evalAndEqual", (void (Scheme::*)(Ciphertext &, Ciphertext &, Ciphertext &, Ciphertext &, Ciphertext &, Key &, Key &, long)) &Scheme::evalAndEqual)
		.def("evalAndEqual", (void (Scheme::*)(Ciphertext &, Ciphertext &, Ciphertext &, Ciphertext &, Ciphertext &, Key &, Key &, Key &, long, int)) &Scheme::evalAndEqual);

	// BootContext
	py::class_<BootContext>(m, "BootContext")
		.def(py::init<>())
		.def_readwrite("bnd1", &BootContext::bnd1)
		.def_readwrite("bnd2", &BootContext::bnd2)
		.def_readwrite("logp", &BootContext::logp);

	// Ring
	py::class_<Ring>(m, "Ring")
		.def(py::init<>())
		// ATTRIBUTE
		.def_readwrite("qpows", &Ring::qpows)
		.def_readwrite("rotGroup", &Ring::rotGroup)
		.def_readwrite("ksiPows", &Ring::ksiPows)
		.def_readwrite("multiplier", &Ring::multiplier)
		// Encode & Decode
		.def("arrayBitReverse", &Ring::arrayBitReverse)
		.def("EMB", &Ring::EMB)
		.def("EMBInvLazy", &Ring::EMBInvLazy)
		.def("EMBInv", &Ring::EMBInv)
		.def("encode", (void (Ring::*)(NTL::ZZ *, double *, long, long)) & Ring::encode)
		.def("encode", (void (Ring::*)(NTL::ZZ *, complex<double> *, long, long)) & Ring::encode)
		.def("decode", &Ring::decode)
		// CONTEXT
		.def("addBootContext", &Ring::addBootContext)
		// MULTIPLICATION
		.def("maxBits", &Ring::maxBits)
		//.def("CRT", &Ring::CRT)
		.def("CRT", (void (Ring::*)(uint64_t *, ZZ *, const long)) & Ring::CRT)
		//.def("addNTTAndEqual", &Ring::addNTTAndEqual)
		.def("addNTTAndEqual", (void (Ring::*)(uint64_t *, uint64_t *, const long)) &Ring::addNTTAndEqual)
		//.def("mult", &Ring::mult)
		.def("mult", (void (Ring::*)(ZZ *, ZZ *, ZZ *, long, const ZZ&)) &Ring::mult)
		//.def("multNTT", &Ring::multNTT)
		.def("multNTT", (void (Ring::*)(ZZ *, ZZ *, uint64_t *, long, const ZZ &)) &Ring::multNTT)
		.def("multDNTT", &Ring::multDNTT)
		.def("multAndEqual", (void (Ring::*)(ZZ *, ZZ *, long, const ZZ &)) &Ring::multAndEqual)
		.def("multNTTAndEqual", &Ring::multNTTAndEqual)
		.def("square", &Ring::square)
		.def("squareNTT", &Ring::squareNTT)
		.def("squareAndEqual", &Ring::squareAndEqual)
		// OTHER
		.def("mod", &Ring::mod)
		.def("modAndEqual", &Ring::modAndEqual)
		.def("negate", &Ring::negate)
		.def("negateAndEqual", &Ring::negateAndEqual)
		//.def("add", &Ring::add)
		.def("add", (void (Ring::*)(ZZ *, ZZ *, ZZ *, const ZZ &)) & Ring::add)
		//.def("addAndEqual", &Ring::addAndEqual)
		.def("addAndEqual", (void (Ring::*)(ZZ *, ZZ *, const ZZ &)) & Ring::addAndEqual)
		.def("sub", &Ring::sub)
		.def("subAndEqual", &Ring::subAndEqual)
		.def("subAndEqual2", &Ring::subAndEqual2)
		.def("multByMonomial", &Ring::multByMonomial)
		.def("multByMonomialAndEqual", &Ring::multByMonomialAndEqual)
		.def("multByConst", &Ring::multByConst)
		.def("multByConstAndEqual", &Ring::multByConstAndEqual)
		.def("leftShift", &Ring::leftShift)
		.def("leftShiftAndEqual", &Ring::leftShiftAndEqual)
		.def("doubleAndEqual", &Ring::doubleAndEqual)
		.def("rightShift", &Ring::rightShift)
		//.def("rightShiftAndEqual", &Ring::rightShiftAndEqual)
		.def("rightShiftAndEqual", (void (Ring::*)(ZZ *, long)) &Ring::rightShiftAndEqual)
		// ROTATION & CONJUGATION
		.def("leftRotate", &Ring::leftRotate)
		.def("conjugate", &Ring::conjugate)
		// SAMPLING
		.def("subFromGaussAndEqual", &Ring::subFromGaussAndEqual)
		.def("addGaussAndEqual", &Ring::addGaussAndEqual)
		.def("sampleHWT", (void (Ring::*)(ZZ *)) &Ring::sampleHWT)
		.def("sampleZO", &Ring::sampleZO)
		//.def("sampleUniform2", &Ring::sampleUniform2)
		.def("sampleUniform2", (void (Ring::*)(ZZ *, long)) & Ring::sampleUniform2);
		
	// DFT

	// RingMultiplier
	py::class_<RingMultiplier>(m, "RingMultiplier")
		.def(py::init<>())
		// ATTRIBUTE
		.def_readwrite("pVec", &RingMultiplier::pVec)
		// METHODS
		.def("primeTest", &RingMultiplier::primeTest)
		.def("NTT", &RingMultiplier::NTT)
		.def("INTT", &RingMultiplier::primeTest)
		//.def("CRT", &RingMultiplier::CRT)
		.def("CRT", (void (RingMultiplier::*)(uint64_t *, ZZ *, const long)) & RingMultiplier::CRT)
		.def("addNTTAndEqual", &RingMultiplier::addNTTAndEqual)
		.def("reconstruct", &RingMultiplier::reconstruct)
		//.def("mult", &RingMultiplier::mult)
		.def("mult", (void (RingMultiplier::*)(ZZ *, ZZ *, ZZ *, long, const ZZ&)) &RingMultiplier::mult)
		.def("mult", (void (RingMultiplier::*)(uint64_t *, ZZ *, ZZ *, long)) &RingMultiplier::mult)
		//.def("multNTT", &RingMultiplier::multNTT)
		.def("multNTT", (void (RingMultiplier::*)(ZZ *, ZZ *, uint64_t *, long, const ZZ&)) &RingMultiplier::multNTT)
		.def("multNTT", (void (RingMultiplier::*)(uint64_t *, ZZ *, uint64_t *, long)) &RingMultiplier::multNTT)
		//.def("multDNTT", &RingMultiplier::multDNTT)
		.def("multDNTT", (void (RingMultiplier::*)(ZZ *, uint64_t *, uint64_t *, long, const ZZ&)) &RingMultiplier::multDNTT)
		.def("multDNTT", (void (RingMultiplier::*)(uint64_t *, uint64_t *, uint64_t *, long)) &RingMultiplier::multDNTT)
		.def("multAndEqual", &RingMultiplier::multAndEqual)
		.def("multNTTAndEqual", &RingMultiplier::multNTTAndEqual)
		.def("square", &RingMultiplier::square)
		.def("squareNTT", &RingMultiplier::squareNTT)
		.def("squareAndEqual", &RingMultiplier::squareAndEqual)
		.def("mulMod", &RingMultiplier::mulMod)
		.def("mulModBarrett", &RingMultiplier::mulModBarrett)
		.def("butt", &RingMultiplier::butt)
		.def("ibutt", &RingMultiplier::ibutt)
		.def("idivN", &RingMultiplier::idivN)
		.def("invMod", &RingMultiplier::invMod)
		.def("powMod", &RingMultiplier::powMod)
		.def("inv", &RingMultiplier::inv)
		.def("pow", &RingMultiplier::pow)
		.def("bitReverse", &RingMultiplier::bitReverse)
		.def("findPrimeFactors", &RingMultiplier::findPrimeFactors)
		.def("findPrimitiveRoot", &RingMultiplier::findPrimitiveRoot)
		.def("findMthRootOfUnity", &RingMultiplier::findMthRootOfUnity);


	// Plaintext
	py::class_<Plaintext, std::unique_ptr<Plaintext, py::nodelete>>(m, "Plaintext")
		.def(py::init<long, long, long>(), py::arg("logp") = 0, py::arg("logq") = 0, py::arg("n") = 0)
		.def(py::init<ZZ *, long, long, long>())
		// ATTRIBUTE
		.def_readwrite("mx", &Plaintext::mx)
		.def_readwrite("logp", &Plaintext::logp)
		.def_readwrite("logq", &Plaintext::logq)
		.def_readwrite("n", &Plaintext::n);

	// Ciphertext
	py::class_<Ciphertext, std::unique_ptr<Ciphertext, py::nodelete>>(m, "Ciphertext")
		// .def(py::init([](int len) { return new Ciphertext[len]; }))
		.def(py::init<>())
		.def(py::init<long, long, long>(), py::arg("logp") = 0, py::arg("logq") = 0, py::arg("n") = 0)
		.def(py::init<long, long, long, ZZ*, ZZ*>())
		.def(py::init<const Ciphertext &>())
		.def("copyParams", &Ciphertext::copyParams)
		.def("copy", &Ciphertext::copy)
		//.def("free", &Ciphertext::free)
		// .def("free",[](Ciphertext *c) { delete[] c; } )
		// ATTRIBUTE
		.def_readwrite("ax", &Ciphertext::ax)
		.def_readwrite("bx", &Ciphertext::bx)
		.def_readwrite("logp", &Ciphertext::logp)
		.def_readwrite("logq", &Ciphertext::logq)
		.def_readwrite("n", &Ciphertext::n)
		// .def("__getitem__", [](const Ciphertext *c, std::int64_t i) { return c[i]; })
		.def("__repr__", [](const Ciphertext &p) {
			return "<class.Ciphertext logp: "+to_string(p.logp)+" logq: "+to_string(p.logq)+" n: "+to_string(p.n)+">";
		})
		.def(py::pickle(
			[](const Ciphertext &c) { // __getstate_
				std::stringstream output(std::ios::binary | std::ios::out);
				long np = ceil(((double)c.logq + 1)/8);
				ZZ q = conv<ZZ>(1) << c.logq;
				unsigned char* bytes = new unsigned char[np];
				for (long i = 0; i < N; ++i) {
					c.ax[i] %= q;
					BytesFromZZ(bytes, c.ax[i], np);
					output.write(reinterpret_cast<char*>(bytes), np);
				}
				for (long i = 0; i < N; ++i) {
					c.bx[i] %= q;
					BytesFromZZ(bytes, c.bx[i], np);
					output.write(reinterpret_cast<char*>(bytes), np);
				}
				std::string cipherstr = output.str();
				std::string encoded_cipher = base64_encode(reinterpret_cast<const unsigned char *>(cipherstr.c_str()), (unsigned int)cipherstr.length());

				return py::make_tuple(c.logp, c.logq, c.n, encoded_cipher);
			},
			[](py::tuple t) { // __setstate__
				if (t.size() != 4)
					throw std::runtime_error("Invalid state!");

				long logp = t[0].cast<int>();
				long logq = t[1].cast<int>();
				long n = t[2].cast<int>();
				Ciphertext cipher(logp, logq, n);

				std::string encoded_cipher = t[3].cast<string>();
				std::string cipherstr_decoded = base64_decode(encoded_cipher);
				std::stringstream input(std::ios::binary | std::ios::in);
				input.str(cipherstr_decoded);
				long np = ceil(((double)logq + 1)/8);
				unsigned char* bytes = new unsigned char[np];
				for (long i = 0; i < N; ++i) {
					input.read(reinterpret_cast<char*>(bytes), np);
					ZZFromBytes(cipher.ax[i], bytes, np);
				}
				for (long i = 0; i < N; ++i) {
					input.read(reinterpret_cast<char*>(bytes), np);
					ZZFromBytes(cipher.bx[i], bytes, np);
				}
				return cipher;
			}));

	// Ciphertexts
	py::class_<Ciphertexts>(m, "Ciphertexts")
		.def(py::init<long>())
		.def(py::init<long, long, long, long>())
		.def(py::init<long, long, long, long, ZZ*, ZZ*>())
		.def("copy", &Ciphertexts::copy)
		// ATTRIBUTE
		.def_readwrite("size", &Ciphertexts::size)
		.def_readwrite("logp", &Ciphertexts::logp)
		.def_readwrite("logq", &Ciphertexts::logq)
		.def_readwrite("n", &Ciphertexts::n)
		.def_readwrite("ax", &Ciphertexts::ax)
		.def_readwrite("bxs", &Ciphertexts::bxs)
		.def("__len__",[](const Ciphertexts &cs) {return cs.size;})
		.def("__getitem__", &Ciphertexts::getItem, py::return_value_policy::take_ownership)
		.def("getBxs", &Ciphertexts::getBxsItem, py::return_value_policy::take_ownership)
		.def("__repr__", [](const Ciphertexts &cs) {
			return "<class.Ciphertexts size "+to_string(cs.size)+" logp: "+to_string(cs.logp)+" logq: "+to_string(cs.logq)+" n: "+to_string(cs.n)+">";
		});

	// EvaluatorUtils
	py::class_<EvaluatorUtils>(m, "EvaluatorUtils")
		// RANDOM REAL AND COMPLEX NUMBERS
		.def_static("randomReal", &EvaluatorUtils::randomReal, py::arg("bound") = 1.0)
		.def_static("randomComplex", &EvaluatorUtils::randomComplex, py::arg("bound") = 1.0)
		.def_static("randomCircle", &EvaluatorUtils::randomCircle, py::arg("anglebound") = 1.0)
		.def_static("randomRealArray", &EvaluatorUtils::randomRealArray, py::arg(), py::arg("bound") = 1.0)
		.def_static("randomComplexArray", &EvaluatorUtils::randomComplexArray, py::arg(), py::arg("bound") = 1.0)
		// .def("randomComplexArray", [](long size, double bound = 1.0){ return EvaluatorUtils::randomComplexArray(size, bound);})
		.def_static("randomCircleArray", &EvaluatorUtils::randomCircleArray, py::arg(), py::arg("bound") = 1.0)
		// DOUBLE & RR <-> ZZ
		.def_static("scaleDownToReal", &EvaluatorUtils::scaleDownToReal)
		// .def_static("scaleUpToZZ", (static ZZ (EvaluatorUtils::*)(const double, const long)) &EvaluatorUtils::scaleUpToZZ);
		.def("scaleUpToZZ", [](const double x, const long logp) { return EvaluatorUtils::scaleUpToZZ(x, logp); })
		.def("scaleUpToZZ", [](const NTL::RR &x, const long logp) { return EvaluatorUtils::scaleUpToZZ(x, logp); })
		// ROTATIONS
		.def_static("leftRotateAndEqual", &EvaluatorUtils::leftRotateAndEqual)
		.def_static("rightRotateAndEqual", &EvaluatorUtils::rightRotateAndEqual);

	// SchemeAlgo
	py::class_<SchemeAlgo>(m, "SchemeAlgo")
		.def(py::init<Scheme &>())
		.def("powerOf2", &SchemeAlgo::powerOf2)
		.def("powerOf2Extended", &SchemeAlgo::powerOf2Extended)
		.def("power", &SchemeAlgo::power)
		.def("powerExtended", &SchemeAlgo::powerExtended)
		.def("inverse", &SchemeAlgo::inverse)
		.def("function", &SchemeAlgo::function)
		.def("functionLazy", &SchemeAlgo::functionLazy);

	// SecretKey
	py::class_<SecretKey>(m, "SecretKey")
		.def(py::init<Ring &>())
		.def(py::init<const SecretKey &>())
		.def(py::init<ZZ *>())
		// SETTING
		.def("setSK", (void (SecretKey::*)(ZZ*)) & SecretKey::setSK)
		.def("setSK", (void (SecretKey::*)(long)) & SecretKey::setSK)
		// ATTRIBUTE
		.def_readwrite("sx", &SecretKey::sx)
		// PRINT: index of the non-zero coefficient in SecretKey.sx
		.def("printInxs", [](const SecretKey& sk, long size = 10) {
			int count = 0;
			std::cout << "[";
			for (long i = 0; i < N; ++i) {
				if (sk.sx[i] != 0) {
					if (count == 0) { std::cout << i; }
					else { std::cout << ", " << i; }
					count++;
				}
				if (count == size) { break; }
			}
			std::cout << "]" << std::endl;
		});

	// SecretKeyShare
	py::class_<SKShare>(m, "SKShare")
		.def(py::init<long>())
		.def(py::init<long, long>())
		.def(py::init<long, long, uint64_t*>())
		// ATTRIBUTE
		.def_readwrite("index", &SKShare::index)
		.def_readwrite("np", &SKShare::np)
		.def_readwrite("rsx_share", &SKShare::rsx_share);

	// SecretKeyShares
	py::class_<SKShares>(m, "SKShares")
		// .def(py::init<long>())
		.def(py::init<long, long>())
		.def(py::init<long, long, uint64_t*, uint64_t*>())
		// ATTRIBUTE
		.def_readwrite("size", &SKShares::size)
		.def_readwrite("np", &SKShares::np)
		.def_readwrite("indexs", &SKShares::indexs)
		.def_readwrite("rsx_shares", &SKShares::rsx_shares)
		.def("__len__",[](const SKShares &ss) {return ss.size;})
		.def("__getitem__", &SKShares::getItem, py::return_value_policy::take_ownership)
		.def("getInxs", &SKShares::getInxsItem)
		.def("getRsxs", &SKShares::getRsxsItem, py::return_value_policy::take_ownership)
		// SET
		.def("setInxs", (void (SKShares::*)(long, uint64_t)) &SKShares::setInxsItem)
		.def("setInxs", (void (SKShares::*)(long, long)) &SKShares::setInxsItem)
		.def("setRsxs", &SKShares::setRsxsItem)
		.def("setShares", (void (SKShares::*)(long, uint64_t, uint64_t*)) &SKShares::setItem)
		.def("setShares", (void (SKShares::*)(long, long, uint64_t*)) &SKShares::setItem);

	// Key
	py::class_<Key>(m, "Key")
		.def(py::init<>())
		.def(py::init<const Key &>())
		.def(py::init<Uint64_t *, Uint64_t *>())
		// // SET
		// .def("setKey", (void (Key::*)(Uint64_t*, Uint64_t*)) & SecretKey::setSK)
		// ATTRIBUTE
		.def_readwrite("rax", &Key::rax)
		.def_readwrite("rbx", &Key::rbx)
		// isNULL
		.def("isEmpty", (bool(Key::*)()) & Key::isEmpty);

	// StringUtils
	py::class_<StringUtils>(m, "StringUtils")
		// SHOW ARRAY
		.def_static("showVec", (void (*)(long *, long)) & StringUtils::showVec)
		.def_static("showVec", (void (*)(double *, long)) & StringUtils::showVec)
		.def_static("showVec", (void (*)(complex<double> *, long)) & StringUtils::showVec)
		.def_static("showVec", (void (*)(NTL::ZZ *, long)) & StringUtils::showVec)
		// SHOW & COMPARE ARRAY
		.def_static("compare", (void (*)(double, double, string)) & StringUtils::compare)
		.def_static("compare", (void (*)(complex<double>, complex<double>, string)) & StringUtils::compare)
		.def_static("compare", (void (*)(double *, double *, long, string)) & StringUtils::compare)
		.def_static("compare", (void (*)(complex<double> *, complex<double> *, long, string)) & StringUtils::compare)
		.def_static("compare", (void (*)(double *, double, long, string)) & StringUtils::compare)
		.def_static("compare", (void (*)(complex<double> *, complex<double>, long, string)) & StringUtils::compare)
		.def_static("compare", (void (*)(double, double *, long, string)) & StringUtils::compare)
		.def_static("compare", (void (*)(complex<double>, complex<double> *, long, string)) & StringUtils::compare);

	// TimeUtils
	py::class_<TimeUtils>(m, "TimeUtils")
		.def(py::init<>())
		.def("start", &TimeUtils::start)
		.def("stop", &TimeUtils::stop);

	py::class_<SerializationUtils>(m, "SerializationUtils")
		.def_static("writeCiphertext", &SerializationUtils::writeCiphertext)
		.def_static("readCiphertext", &SerializationUtils::readCiphertext)
		.def_static("writeKey", (void (*)(Key &, string)) &SerializationUtils::writeKey)
		.def_static("readKey", &SerializationUtils::readKey);
}
