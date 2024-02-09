from distutils.core import setup, Extension
from distutils import sysconfig

cfg_vars = sysconfig.get_config_vars()
for key, value in cfg_vars.items():
    if type(value) == str:
        cfg_vars[key] = value.replace('-Wstrict-prototypes', '')

cpp_args = ['-std=c++11']

ext_modules = [
    Extension(
        'HEAAN',
        ['src/base64.cpp', 'src/wrapper.cpp'],
        include_dirs=['/home/adriana/.local/lib/python3.10/site-packages/pybind11/include', './MP-HEAAN/src', '/usr/include/python3.10', '/usr/local/include'],
        language='c++',
        extra_compile_args=cpp_args,
        extra_objects=['/usr/local/lib/libntl.so', './MP-HEAAN/lib/libHEAAN.a'], # both lib need compiled with -fPIC
    ),
]

setup(
    name='HEAAN',
    version='3.1.0',
    description='Python wrapper for MP-HEAAN',
    url='',
    license='MIT',
    ext_modules=ext_modules,
)
