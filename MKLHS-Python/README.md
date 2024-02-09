# MKLHS-Python (AMD64 VERSION)
This is a Python wrapper for MKLHS library for AMD64.
[MKLHS](./MKLHS) The Multi-key Linearly Homomorphic Signature Scheme is on the [paper](https://eprint.iacr.org/2019/830.pdf), which is implemented in the library [relic](https://github.com/relic-toolkit/relic).

## build MKLHS and run the example
  * ### Recommend: Clang++ (>= 10.0) or GNU G++ (>= 9.4), CMake (>= 3.16); Ububtu 22.04

  * ### install pybind11
    ``` shell
    pip3 install -r requirements.txt
    ```

  * ### install relic (build "./target/")
    ``` shell
    sudo apt install cmake
    cd MKLHS-Python/MKLHS/relic
    unzip src.zip 
    make
    ```

  * ### compile libMKLHS.a
    ``` shell
    cd MKLHS-Python/MKLHS/lib
    make all 
    ```

  * ### compile MKLHS-Python (global install)
    ``` shell
    cd MKLHS-Python/
    sudo python3 setup.py install
    ```

  * ### run the example
    ``` shell
    python3 test.py
    ```

  * ### FQA
    Problem: ImportError: /usr/local/lib/python3.10/dist-packages/MKLHS.cpython-310-x86_64-linux-gnu.so: undefined symbol: xxx (e.g. gmp_mul_1)
    Solve: 
    ``` shell
    sudo python3 setup.py install
    # output: 
        # ... 
        # x86_64-linux-gnu-g++ -shared -Wl,-O1 -Wl,-Bsymbolic-functions -Wl,-Bsymbolic-functions -g -fwrapv -O2 -Wl,-Bsymbolic-functions -g -fwrapv -O2 -g -fstack-protector-strong -Wformat -Werror=format-security -Wdate-time -D_FORTIFY_SOURCE=2 build/temp.linux-x86_64-3.10/src/wrapper.o /usr/local/lib/libgmp.so ./MKLHS/lib/libMKLHS.a ./MKLHS/relic/target/lib/librelic_s.a -o build/lib.linux-x86_64-3.10/MKLHS.cpython-310-x86_64-linux-gnu.so
        # copying build/lib.linux-x86_64-3.10/MKLHS.cpython-310-x86_64-linux-gnu.so -> /usr/local/lib/python3.10/dist-packages
        # ...
    sudo x86_64-linux-gnu-g++ -shared -o build/lib.linux-x86_64-3.10/MKLHS.cpython-310-x86_64-linux-gnu.so -Wl,-O1 -Wl,-Bsymbolic-functions -Wl,-Bsymbolic-functions -g -fwrapv -O2 -Wl,-Bsymbolic-functions -g -fwrapv -O2 -g -fstack-protector-strong -Wformat -Werror=format-security -Wdate-time -D_FORTIFY_SOURCE=2 build/temp.linux-x86_64-3.10/src/wrapper.o ./MKLHS/lib/libMKLHS.a ./MKLHS/relic/target/lib/librelic_s.a -lgmp
    sudo scp build/lib.linux-x86_64-3.10/MKLHS.cpython-310-x86_64-linux-gnu.so /usr/local/lib/python3.10/dist-packages
    ```


## modify MKLHS-Python and test the program
* ### rebuild librelic_s.a (if modify MKLHS/relic/src)
    ``` shell
    cd MKLHS/relic
    make
    ```

* ### rebuild libMKLHS.a (if modify MKLHS)
    ``` shell
    cd MKLHS/lib
    make all
    ```
* ### rebuild LHH-Python
    ``` shell
    sudo rm -r ./build 
    sudo python3 setup.py install
    ```


