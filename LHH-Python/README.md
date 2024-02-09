# LHH-Python (AMD64 VERSION)
This is a Python wrapper for LHH library for AMD64.
[LHH](./LHH) Linearly Homomorphic Hash supports linear operations on its output, allowing for efficient verification of linear
combinations of data without needing to access the original data. This scheme  is based on the LHH funtion used in [paper](https://eprint.iacr.org/2022/1073) and the library [VeriFL](https://github.com/ErwinSCat/VeriFL)


## build LHH and run the example
  * ### Recommend: Clang++ (>= 10.0) or GNU G++ (>= 9.4), CMake (>= 3.16); Ububtu 22.04

  * ### install openssl
    Ubuntu Download:
      ``` shell
      sudo apt install openssl -y
      sudo apt install libssl-dev
      ```

  * ### install pybind11
    ``` shell
    pip3 install pybind11
    ```

  * ### compile libLHH.a
    ``` shell
    cd LHH-Python/LHH/lib
    make all 
    ```

  * ### compile LHH-Python (global install)
    ``` shell
    cd LHH-Python/
    sudo python3 setup.py install
    ```

  * ### run the example
    ``` shell
    python3 test.py
    ```

  * ### FAQ
    Problem: ImportError: /usr/local/lib/python3.10/dist-packages/LHH.cpython-310-x86_64-linux-gnu.so: undefined symbol: xxx (e.g.EC_POINT_add)
    Solve: 
    ``` shell
    sudo python3 setup.py install
        # output: 
            # ... 
            # x86_64-linux-gnu-g++ -shared -Wl,-O1 -Wl,-Bsymbolic-functions -Wl,-Bsymbolic-functions -g -fwrapv -O2 -Wl,-Bsymbolic-functions -g -fwrapv -O2 -g -fstack-protector-strong -Wformat -Werror=format-security -Wdate-time -D_FORTIFY_SOURCE=2 build/temp.linux-x86_64-3.10/src/wrapper.o -lssl -lcrypto ./LHH/lib/libLHH.a -o build/lib.linux-x86_64-3.10/LHH.cpython-310-x86_64-linux-gnu.so
            # copying build/lib.linux-x86_64-3.10/LHH.cpython-310-x86_64-linux-gnu.so -> /usr/local/lib/python3.10/dist-packages
            # ...
    sudo x86_64-linux-gnu-g++ -shared -Wl,-O1 -Wl,-Bsymbolic-functions -Wl,-Bsymbolic-functions -g -fwrapv -O2 -Wl,-Bsymbolic-functions -g -fwrapv -O2 -g -fstack-protector-strong -Wformat -Werror=format-security -Wdate-time -D_FORTIFY_SOURCE=2 build/temp.linux-x86_64-3.10/src/wrapper.o  -o build/lib.linux-x86_64-3.10/LHH.cpython-310-x86_64-linux-gnu.so ./LHH/lib/libLHH.a -lssl -lcrypto
    sudo scp -r build/lib.linux-x86_64-3.10/LHH.cpython-310-x86_64-linux-gnu.so /usr/local/lib/python3.10/dist-packages
    ```

## modify LHH-Python and test the program
  * ### rebuild libLHH.a (if modify LHH)
    ``` shell
    cd LHH/lib
    make all
    ```

  * ### rebuild LHH-Python
    * method 1: 
    ``` shell
    sudo rm -r ./build 
    sudo python3 setup.py install
    ```

    * method 2: 
    ``` shell
    python3 rebuild.py # modify 'sudoPassword' in rebuild.py
    ```

