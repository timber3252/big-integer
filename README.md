# Big Integer

This is `Big Integer`, a header-only big integer library, which serves as the assignment of Foundations of Data Structure.

In this project, we are required to implement a simple big integer library for cryptography using linked list and supports addition, subtraction, multiplication, division, power module arithmetic.

## Usage

Since it's a header-only big integer library, you only need to include the header file `big-integer.h` to use it. Here is an example.

```c++
#include "big_integer.h"

int main() {
  BigInteger<2048> a = BigInteger<2048>::from_dec("251273987251372141751215782137213");
  BigInteger<2048> b = BigInteger<2048>::from_hex("fffffffffffffffffffaaaaaaaaaaaaaa");
  std::cout << ((a ^ b) * b + a) << std::endl;
}
```

More details in [big_integer.h](big_integer.h).

## Test

To build this project, make sure you have [conan](https://github.com/conan-io/conan) and CMake installed.

Then, in the project directory, run the following command:

```bash
mkdir build
cd build
conan install ..
cmake -S .. -B .
make
```

Then you will get an executable file for unit test which is located in `build/bin/`, unit tests are performed using [catch2](https://github.com/catchorg/Catch2).

Note: If you are running with MinGW-w64 on Windows, you might need to specify `-G "MinGW Makefiles"` to let CMake use `make` instead of `nmake`.