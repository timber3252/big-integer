#include "big_integer.h"

int main() {
  BigInteger<2048> a = BigInteger<2048>::from_dec("251273987251372141751215782137213");
  BigInteger<2048> b = BigInteger<2048>::from_hex("fffffffffffffffffffaaaaaaaaaaaaaa");
  std::cout << ((a ^ b) * b + a) << std::endl;
}