from distutils.core import setup, Extension
from distutils import sysconfig

cfg_vars = sysconfig.get_config_vars()
for key, value in cfg_vars.items():
    if type(value) == str:
        cfg_vars[key] = value.replace('-Wstrict-prototypes', '')

cpp_args = ['-std=c++11']

ext_modules = [
    Extension(
        'LHH',
        ['src/wrapper.cpp'],
        include_dirs=['/home/adriana/.local/lib/python3.10/site-packages/pybind11/include', 'LHH/src','/usr/include/python3.10'],
        language='c++',
        extra_compile_args=cpp_args,
        extra_objects=['-lssl', '-lcrypto', './LHH/lib/libLHH.a'], # all lib need compiled with -fPIC
    ),
]

setup(
    name='LHH',
    version='1.0.0',
    description='Python wrapper for LHH',
    url='',
    license='MIT',
    ext_modules=ext_modules,
)
