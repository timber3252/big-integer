#ifndef FDS_BIG_INTEGER_
#define FDS_BIG_INTEGER_

#include <cctype>
#include <algorithm>
#include <iostream>
#include <exception>
#include <string>
#include <cassert>
#include <stdexcept>
#include <limits>
#include <cmath>
#include <memory>
#include <sstream>

#include "list.h"

// 实现模 2^M 意义下的大整数运算（正整数）
template <std::size_t M>
class BigInteger {
  static_assert(M != 0, "M == 0 is not allowed for template class BigInteger<M>");

 private: // 基本类型定义
  typedef std::uint64_t Integral;

 private: // 为了让位运算更加直观，预先定义了一些常量
  constexpr static Integral UNSIGNED_MAX = 1LL << 32;
  constexpr static Integral UNSIGNED_MASK = 0xffffffff;
  constexpr static Integral UNSIGNED_LEN = 32;
  constexpr static Integral UNSIGNED_BIT_MASKS[32] = {
      0xffffffff, 0x00000001, 0x00000003, 0x00000007,
      0x0000000f, 0x0000001f, 0x0000003f, 0x0000007f,
      0x000000ff, 0x000001ff, 0x000003ff, 0x000007ff,
      0x00000fff, 0x00001fff, 0x00003fff, 0x00007fff,
      0x0000ffff, 0x0001ffff, 0x0003ffff, 0x0007ffff,
      0x000fffff, 0x001fffff, 0x003fffff, 0x007fffff,
      0x00ffffff, 0x01ffffff, 0x03ffffff, 0x07ffffff,
      0x0fffffff, 0x1fffffff, 0x3fffffff, 0x7fffffff,
  };

 private: // 一些阈值
  constexpr static Integral MUL_KARATSUBA_THRESHOLD = 20;
  constexpr static Integral DIV_BINARY_SEARCH_THRESHOLD = 10;
  constexpr static Integral POW_SLIDING_WINDOW_THRESHOLD = 50;
  constexpr static Integral POW_PACKING_THRESHOLD = 30;

 private: // 操作常数
  constexpr static Integral POW_SLIDING_WINDOW_LENGTH = 8;
  constexpr static Integral POW_SLIDING_WINDOW_STORAGE_SIZE = 1ULL << POW_SLIDING_WINDOW_LENGTH;
  constexpr static Integral POW_SLIDING_WINDOW_MASK = POW_SLIDING_WINDOW_STORAGE_SIZE - 1;

  constexpr static Integral POW_PACKING_WINDOW_LENGTH = 8;
  constexpr static Integral POW_PACKING_WINDOW_STORAGE_SIZE = 1ULL << POW_PACKING_WINDOW_LENGTH;
  constexpr static Integral POW_PACKING_WINDOW_MASK = POW_PACKING_WINDOW_STORAGE_SIZE - 1;

 private: // 取模辅助变量
  constexpr static std::size_t LIMIT_NUMS = (M - 1) / 32 + 1; // 整块的个数
  constexpr static std::size_t REM_BITS = M % 32; // 剩下的二进制位个数

 private: // 用于存储大整数数据
  List<unsigned> data;

 public: // 构造函数与析构函数
  BigInteger();
  BigInteger(const BigInteger &other);
  explicit BigInteger(const std::uint64_t &num);
  explicit BigInteger(const std::string &num);
  ~BigInteger();

 private: // 由链表构造大整数
  explicit BigInteger(List<unsigned> &other);

 public: // 拷贝与交换
  auto operator=(const BigInteger &other) -> BigInteger&;
  auto swap(BigInteger &other) -> void;
  auto swap(BigInteger &&other) -> void;

 public: // 大整数加法运算符重载：直接调用加法辅助函数
  auto operator+(const BigInteger &other) const -> BigInteger;
  auto operator+(const std::uint64_t &other) const -> BigInteger;
  auto operator+(const std::string &other) const -> BigInteger;
  auto operator+=(const BigInteger &other) -> BigInteger&;
  auto operator+=(const std::uint64_t &other) -> BigInteger&;
  auto operator+=(const std::string &other) -> BigInteger&;

 public: // 大整数减法运算符重载：直接调用减法辅助函数
  auto operator-(const BigInteger &other) const -> BigInteger;
  auto operator-(const std::uint64_t &other) const -> BigInteger;
  auto operator-(const std::string &other) const -> BigInteger;
  auto operator-=(const BigInteger &other) -> BigInteger&;
  auto operator-=(const std::uint64_t &other) -> BigInteger&;
  auto operator-=(const std::string &other) -> BigInteger&;

 public: // 大整数乘法运算符重载：直接调用辅助函数
  auto operator*(const BigInteger &other) const -> BigInteger;
  auto operator*(const std::uint64_t &other) const -> BigInteger;
  auto operator*(const std::string &other) const -> BigInteger;
  auto operator*=(const BigInteger &other) -> BigInteger&;
  auto operator*=(const std::uint64_t &other) -> BigInteger&;
  auto operator*=(const std::string &other) -> BigInteger&;

 public: // 大整数除法运算符重载：直接调用辅助函数
  auto operator/(const BigInteger &other) const -> BigInteger;
  auto operator/(const std::uint64_t &other) const -> BigInteger;
  auto operator/(const std::string &other) const -> BigInteger;
  auto operator/=(const BigInteger &other) -> BigInteger&;
  auto operator/=(const std::uint64_t &other) -> BigInteger&;
  auto operator/=(const std::string &other) -> BigInteger&;

 public: // 大整数幂次运算符重载：直接调用辅助函数
  auto operator^(const BigInteger &other) const -> BigInteger;
  auto operator^(const std::uint64_t &other) const -> BigInteger;
  auto operator^(const std::string &other) const -> BigInteger;
  auto operator^=(const BigInteger &other) -> BigInteger&;
  auto operator^=(const std::uint64_t &other) -> BigInteger&;
  auto operator^=(const std::string &other) -> BigInteger&;

 public: // 大整数判断是否相等运算符重载
  auto operator==(const BigInteger &other) const -> bool;
  auto operator==(const std::uint64_t &other) const -> bool;
  auto operator==(const std::string &other) const -> bool;
  auto operator!=(const BigInteger &other) const -> bool;
  auto operator!=(const std::uint64_t &other) const -> bool;
  auto operator!=(const std::string &other) const -> bool;

 public: // 大整数比较大小运算符重载
  auto operator<(const BigInteger &other) const -> bool;
  auto operator<(const std::uint64_t &other) const -> bool;
  auto operator<(const std::string &other) const -> bool;
  auto operator>=(const BigInteger &other) const -> bool;
  auto operator>=(const std::uint64_t &other) const -> bool;
  auto operator>=(const std::string &other) const -> bool;
  auto operator>(const BigInteger &other) const -> bool;
  auto operator>(const std::uint64_t &other) const -> bool;
  auto operator>(const std::string &other) const -> bool;
  auto operator<=(const BigInteger &other) const -> bool;
  auto operator<=(const std::uint64_t &other) const -> bool;
  auto operator<=(const std::string &other) const -> bool;

 public: // 大整数输入输出函数
  template <std::size_t N> friend auto operator>>(std::istream &is, BigInteger<N> &self) -> std::istream&;
  template <std::size_t N> friend auto operator<<(std::ostream &os, const BigInteger<N> &self) -> std::ostream&;

 public: // 转换为对应进制的字符串
  auto hex() -> std::string;
  auto bin() -> std::string;
  auto dec() -> std::string;

 public: // 从对应进制的字符串构造大整数
  static auto from_hex(std::string s) -> BigInteger;
  static auto from_bin(std::string s) -> BigInteger;
  static auto from_dec(const std::string &s) -> BigInteger;

 private: // 大整数运算辅助函数
  static auto add(const BigInteger &a, const BigInteger &b) -> BigInteger;
  static auto sub(const BigInteger &a, const BigInteger &b) -> BigInteger;
  static auto mul(const BigInteger &a, const BigInteger &b) -> BigInteger;
  static auto mul_base(const BigInteger &a, const BigInteger &b) -> BigInteger;
  static auto mul_karatsuba(const BigInteger &a, const BigInteger &b) -> BigInteger;
  static auto mul_karatsuba_impl(const BigInteger &a, const BigInteger &b) -> BigInteger;
  static auto div(const BigInteger &a, const BigInteger &b) -> BigInteger;
  static auto div_base(const BigInteger &a, const BigInteger &b) -> BigInteger;
  static auto div_binary_search(const BigInteger &a, const BigInteger &b) -> BigInteger;
  static auto pow(const BigInteger &a, const BigInteger &b) -> BigInteger;
  static auto pow_base(const BigInteger &a, const BigInteger &b) -> BigInteger;
  static auto pow_packing(const BigInteger &a, const BigInteger &b) -> BigInteger;
  static auto pow_sliding_window(const BigInteger &a, const BigInteger &b) -> BigInteger;

 private: // 大整数比较和判等辅助函数
  static auto equal(const BigInteger &a, const BigInteger &b) -> bool;
  static auto less_than(const BigInteger &a, const BigInteger &b) -> bool;

 private: // 其他辅助函数
  auto fix() -> void; // 快速取模和去除前导 0
  static auto shl(const BigInteger &x, std::size_t count) -> BigInteger; // 快速乘以 (2 ^ k), for any k
  static auto shl_block(const BigInteger &x, std::size_t count) -> BigInteger; // 快速乘以 (2 ^ 32) ^ count
  static auto shl_inside_block(const BigInteger &x, std::size_t count) -> BigInteger; // 快速乘以 (2 ^ k), k < 32
  static auto div_by_two(const BigInteger &x) -> BigInteger; // 快速除以 2
  static auto decimal_to_binary(const std::string &s, List<unsigned> &list) -> void; // 将十进制数字（字符串类型）转成 2^32 进制（链表类型）
  static auto binary_to_decimal(const List<unsigned> &list, std::string &s) -> void; // 将 2^32 进制（链表类型）转成十进制数字（字符串类型）
  static auto decimal_add(std::string &a, std::string &b) -> std::string; // 将两个十进制数字（字符串类型）相加，用于辅助进制转换
};

#include "big_integer_impl.h"

#endif //FDS_BIG_INTEGER_
