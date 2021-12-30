#ifndef FDS_BIG_INTEGER_IMPL_
#define FDS_BIG_INTEGER_IMPL_

#include "big_integer.h"

/////////////////////////////////////////////////////////////////////////////////////////
// 大整数构造函数实现

template<std::size_t M>
BigInteger<M>::BigInteger() : data() {}

template<std::size_t M>
BigInteger<M>::BigInteger(const BigInteger &other) {
  data.reconstruct(other.data);
}

template<std::size_t M>
BigInteger<M>::BigInteger(const uint64_t &num) : data() {
  unsigned high = num >> UNSIGNED_LEN & UNSIGNED_MASK;
  unsigned low = num & UNSIGNED_MASK;

  if (high != 0) {
    data.push_back(low);
    data.push_back(high);
  } else if (low != 0) {
    data.push_back(low);
  }
  this->fix();
}

template<std::size_t M>
BigInteger<M>::BigInteger(const std::string &num) : data() {
  decimal_to_binary(num, data);
  this->fix();
}

template<std::size_t M>
BigInteger<M>::~BigInteger() = default;

template<std::size_t M>
BigInteger<M>::BigInteger(List<unsigned int> &other) {
  data.swap(other);
}

/////////////////////////////////////////////////////////////////////////////////////////
// 大整数拷贝与交换的实现

template<std::size_t M>
auto BigInteger<M>::operator=(const BigInteger &other) -> BigInteger & {
  data.reconstruct(other.data);
  return *this;
}
template<std::size_t M>
auto BigInteger<M>::swap(BigInteger &other) -> void { data.swap(other.data); }

template<std::size_t M>
auto BigInteger<M>::swap(BigInteger &&other) -> void {
  data.swap(std::move(other.data));
}

/////////////////////////////////////////////////////////////////////////////////////////
// 大整数加法运算符重载

template<std::size_t M>
auto BigInteger<M>::operator+(const BigInteger &other) const -> BigInteger { return add(*this, other); }
template<std::size_t M>
auto BigInteger<M>::operator+(const uint64_t &other) const -> BigInteger { return add(*this, BigInteger(other)); }
template<std::size_t M>
auto BigInteger<M>::operator+(const std::string &other) const -> BigInteger { return add(*this, BigInteger(other)); }
template<std::size_t M>
auto BigInteger<M>::operator+=(const BigInteger &other) -> BigInteger& { return *this = add(*this, other); }
template<std::size_t M>
auto BigInteger<M>::operator+=(const uint64_t &other) -> BigInteger& { return *this = add(*this, BigInteger(other)); }
template<std::size_t M>
auto BigInteger<M>::operator+=(const std::string &other) -> BigInteger& { return *this = add(*this, BigInteger(other)); }

/////////////////////////////////////////////////////////////////////////////////////////
// 大整数减法运算符重载

template<std::size_t M>
auto BigInteger<M>::operator-(const BigInteger &other) const -> BigInteger { return sub(*this, other); }
template<std::size_t M>
auto BigInteger<M>::operator-(const uint64_t &other) const -> BigInteger { return sub(*this, BigInteger(other)); }
template<std::size_t M>
auto BigInteger<M>::operator-(const std::string &other) const -> BigInteger { return sub(*this, BigInteger(other)); }
template<std::size_t M>
auto BigInteger<M>::operator-=(const BigInteger &other) -> BigInteger& { return *this = sub(*this, other); }
template<std::size_t M>
auto BigInteger<M>::operator-=(const uint64_t &other) -> BigInteger& { return *this = sub(*this, BigInteger(other)); }
template<std::size_t M>
auto BigInteger<M>::operator-=(const std::string &other) -> BigInteger& { return *this = sub(*this, BigInteger(other)); }

/////////////////////////////////////////////////////////////////////////////////////////
// 大整数乘法运算符重载

template<std::size_t M>
auto BigInteger<M>::operator*(const BigInteger &other) const -> BigInteger { return mul(*this, other); }
template<std::size_t M>
auto BigInteger<M>::operator*(const uint64_t &other) const -> BigInteger { return mul(*this, BigInteger(other)); }
template<std::size_t M>
auto BigInteger<M>::operator*(const std::string &other) const -> BigInteger { return mul(*this, BigInteger(other)); }
template<std::size_t M>
auto BigInteger<M>::operator*=(const BigInteger &other) -> BigInteger& { return *this = mul(*this, other); }
template<std::size_t M>
auto BigInteger<M>::operator*=(const uint64_t &other) -> BigInteger& { return *this = mul(*this, BigInteger(other)); }
template<std::size_t M>
auto BigInteger<M>::operator*=(const std::string &other) -> BigInteger& { return *this = mul(*this, BigInteger(other)); }

/////////////////////////////////////////////////////////////////////////////////////////
// 大整数除法运算符重载

template<std::size_t M>
auto BigInteger<M>::operator/(const BigInteger &other) const -> BigInteger { return div(*this, other); }
template<std::size_t M>
auto BigInteger<M>::operator/(const uint64_t &other) const -> BigInteger { return div(*this, BigInteger(other)); }
template<std::size_t M>
auto BigInteger<M>::operator/(const std::string &other) const -> BigInteger { return div(*this, BigInteger(other)); }
template<std::size_t M>
auto BigInteger<M>::operator/=(const BigInteger &other) -> BigInteger & { return *this = div(*this, other); }
template<std::size_t M>
auto BigInteger<M>::operator/=(const uint64_t &other) -> BigInteger & { return *this = div(*this, BigInteger(other)); }
template<std::size_t M>
auto BigInteger<M>::operator/=(const std::string &other) -> BigInteger & { return *this = div(*this, BigInteger(other)); }

/////////////////////////////////////////////////////////////////////////////////////////
// 大整数幂次运算符重载

template<std::size_t M>
auto BigInteger<M>::operator^(const BigInteger &other) const -> BigInteger { return pow(*this, other); }
template<std::size_t M>
auto BigInteger<M>::operator^(const uint64_t &other) const -> BigInteger { return pow(*this, BigInteger(other)); }
template<std::size_t M>
auto BigInteger<M>::operator^(const std::string &other) const -> BigInteger { return pow(*this, BigInteger(other)); }
template<std::size_t M>
auto BigInteger<M>::operator^=(const BigInteger &other) -> BigInteger& { return *this = pow(*this, other); }
template<std::size_t M>
auto BigInteger<M>::operator^=(const uint64_t &other) -> BigInteger& { return *this = pow(*this, BigInteger(other)); }
template<std::size_t M>
auto BigInteger<M>::operator^=(const std::string &other) -> BigInteger& { return *this = pow(*this, BigInteger(other)); }

/////////////////////////////////////////////////////////////////////////////////////////
// 大整数比较运算符重载

template<std::size_t M>
auto BigInteger<M>::operator==(const BigInteger &other) const -> bool { return equal(*this, other); }
template<std::size_t M>
auto BigInteger<M>::operator==(const uint64_t &other) const -> bool { return equal(*this, BigInteger(other)); }
template<std::size_t M>
auto BigInteger<M>::operator==(const std::string &other) const -> bool { return equal(*this, BigInteger(other)); }
template<std::size_t M>
auto BigInteger<M>::operator!=(const BigInteger &other) const -> bool { return !equal(*this, other); }
template<std::size_t M>
auto BigInteger<M>::operator!=(const uint64_t &other) const -> bool { return !equal(*this, BigInteger(other)); }
template<std::size_t M>
auto BigInteger<M>::operator!=(const std::string &other) const -> bool { return !equal(*this, BigInteger(other)); }

/////////////////////////////////////////////////////////////////////////////////////////
// 大整数比较运算符重载

template<std::size_t M>
auto BigInteger<M>::operator<(const BigInteger &other) const -> bool { return less_than(*this, other); }
template<std::size_t M>
auto BigInteger<M>::operator<(const uint64_t &other) const -> bool { return less_than(*this, BigInteger(other)); }
template<std::size_t M>
auto BigInteger<M>::operator<(const std::string &other) const -> bool { return less_than(*this, BigInteger(other)); }
template<std::size_t M>
auto BigInteger<M>::operator>=(const BigInteger &other) const -> bool { return !less_than(*this, other); }
template<std::size_t M>
auto BigInteger<M>::operator>=(const uint64_t &other) const -> bool { return !less_than(*this, BigInteger(other)); }
template<std::size_t M>
auto BigInteger<M>::operator>=(const std::string &other) const -> bool { return !less_than(*this, BigInteger(other)); }
template<std::size_t M>
auto BigInteger<M>::operator>(const BigInteger &other) const -> bool { return less_than(other, *this); }
template<std::size_t M>
auto BigInteger<M>::operator>(const uint64_t &other) const -> bool { return less_than(BigInteger(other), *this); }
template<std::size_t M>
auto BigInteger<M>::operator>(const std::string &other) const -> bool { return less_than(BigInteger(other), *this); }
template<std::size_t M>
auto BigInteger<M>::operator<=(const BigInteger &other) const -> bool { return !less_than(other, *this); }
template<std::size_t M>
auto BigInteger<M>::operator<=(const uint64_t &other) const -> bool { return !less_than(BigInteger(other), *this); }
template<std::size_t M>
auto BigInteger<M>::operator<=(const std::string &other) const -> bool { return !less_than(BigInteger(other), *this); }

/////////////////////////////////////////////////////////////////////////////////////////
// 输入输出运算符重载

template <std::size_t N>
auto operator>>(std::istream &is, BigInteger<N> &self) -> std::istream & {
  std::string s;
  is >> s;
  self = BigInteger<N>(s);
  return is;
}
template <std::size_t N>
auto operator<<(std::ostream &os, const BigInteger<N> &self) -> std::ostream & {
  std::string s;
  BigInteger<N>::binary_to_decimal(self.data, s);
  return os << s;
}

/////////////////////////////////////////////////////////////////////////////////////////
// 输入输出部分：得到 16 进制字符串

template<std::size_t M>
auto BigInteger<M>::hex() -> std::string {
  this->fix();
  std::stringstream ss;

  auto it = data.end();
  do {
    --it;
    ss << std::hex << *it;
  } while (it != data.begin());

  std::string s;
  ss >> s;

  // 消除前导 0
  bool vis = false;
  for (std::size_t i = 0; i < s.length(); ++i) {
    if (s[i] != '0') {
      s = s.substr(i);
      vis = true;
      break;
    }
  }
  // 针对 0 特殊处理
  if (!vis) s = "0";

  return s;
}

/////////////////////////////////////////////////////////////////////////////////////////
// 输入输出部分：得到 2 进制字符串

template<std::size_t M>
auto BigInteger<M>::bin() -> std::string {
  this->fix();
  std::stringstream ss;

  auto it = data.end();
  do {
    --it;
    for (std::size_t j = 32; j > 0; --j) {
      ss << (char)(((*it >> (j - 1)) & 1) + '0');
    }
  } while (it != data.begin());

  std::string s;
  ss >> s;

  // 消除前导 0
  bool vis = false;
  for (std::size_t i = 0; i < s.length(); ++i) {
    if (s[i] != '0') {
      s = s.substr(i);
      vis = true;
      break;
    }
  }
  // 针对 0 特殊处理
  if (!vis) s = "0";

  return s;
}

/////////////////////////////////////////////////////////////////////////////////////////
// 输入输出部分：得到 10 进制字符串

template<std::size_t M>
auto BigInteger<M>::dec() -> std::string {
  this->fix();

  std::stringstream ss;
  ss << *this;

  std::string s;
  ss >> s;

  return s;
}

/////////////////////////////////////////////////////////////////////////////////////////
// 输入输出部分：由 16 进制字符串构造大整数

template<std::size_t M>
auto BigInteger<M>::from_hex(std::string s) -> BigInteger {
  BigInteger res;
  std::reverse(s.begin(), s.end());
  for (auto &ch : s)
    ch = (char)std::tolower(ch);

  for (std::size_t i = 0; i < s.length(); i += 8) {
    std::string ss = s.substr(i, 8);

    Integral curr = 0;
    std::reverse(ss.begin(), ss.end());
    for (std::size_t j = 0; j < ss.length(); ++j) {
      if (std::isdigit(ss[j])) {
        curr = (curr * 16) + ss[j] - '0';
      } else if (ss[j] >= 'a' && ss[j] <= 'f') {
        curr = (curr * 16) + ss[j] - 'a' + 10;
      } else {
        throw std::logic_error("invalid hex number");
      }
    }

    res.data.push_back(curr);
  }

  res.fix();
  return res;
}

/////////////////////////////////////////////////////////////////////////////////////////
// 输入输出部分：由 2 进制字符串构造大整数

template<std::size_t M>
auto BigInteger<M>::from_bin(std::string s) -> BigInteger {
  BigInteger res;
  std::reverse(s.begin(), s.end());

  for (std::size_t i = 0; i < s.length(); i += 32) {
    std::string ss = s.substr(i, 32);

    Integral curr = 0;
    std::reverse(ss.begin(), ss.end());
    for (std::size_t j = 0; j < ss.length(); ++j) {
      if (ss[j] >= '0' && ss[j] <= '1') {
        curr = (curr * 2) + ss[j] - '0';
      } else {
        throw std::logic_error("invalid bin number");
      }
    }

    res.data.push_back(curr);
  }

  res.fix();
  return res;
}

/////////////////////////////////////////////////////////////////////////////////////////
// 输入输出部分：由 10 进制字符串构造大整数

template<std::size_t M>
auto BigInteger<M>::from_dec(const std::string &s) -> BigInteger {
  return BigInteger(s);
}


/////////////////////////////////////////////////////////////////////////////////////////
// 辅助函数 add
// 用于实现取模加法

template<std::size_t M>
auto BigInteger<M>::add(const BigInteger &a, const BigInteger &b) -> BigInteger {
  BigInteger res;

  std::size_t len = std::max(a.data.size(), b.data.size());
  auto it1 = a.data.begin(), it2 = b.data.begin();
  Integral rem = 0, lhs, rhs;

  // 设置上限，对于超出模数的部分直接不进行计算，可以减少运算量
  for (std::size_t i = 0; i < len && res.data.size() < LIMIT_NUMS; ++i) {
    lhs = it1 == a.data.end() ? 0 : *it1;
    rhs = it2 == b.data.end() ? 0 : *it2;
    rem += lhs + rhs;

    res.data.push_back(rem & UNSIGNED_MASK);
    rem >>= UNSIGNED_LEN;

    if (it1 != a.data.end()) ++it1;
    if (it2 != b.data.end()) ++it2;
  }

  if (rem > 0) {
    res.data.push_back(rem & UNSIGNED_MASK);
  }

  res.fix();
  return res;
}

/////////////////////////////////////////////////////////////////////////////////////////
// 辅助函数 sub
// 用于实现取模减法

template<std::size_t M>
auto BigInteger<M>::sub(const BigInteger &a, const BigInteger &b) -> BigInteger {
  if (a < b) {
    // 如果被减数小于减数，则结果为 a + MOD - b，等价于加上补码
    BigInteger<M> rev;
    rev.data.reconstruct(M, 0);

    // 计算反码
    for (auto it1 = rev.data.begin(), it2 = b.data.begin(); it1 != rev.data.end(); ++it1) {
      if (it2 == b.data.end())
        *it1 = UNSIGNED_MASK;
      else
        *it1 = UNSIGNED_MASK ^ *it2;

      if (it2 != b.data.end()) ++it2;
    }

    if (!rev.data.empty())
      rev.data.back() &= UNSIGNED_BIT_MASKS[REM_BITS];

    // 计算补码
    rev = rev + 1;

    return a + rev;
  } else {
    BigInteger<M> result;

    // 如果被减数大于或等于减数，就直接减法
    auto it1 = a.data.begin(), it2 = b.data.begin();
    std::size_t len = std::max(a.data.size(), b.data.size());

    Integral lhs, rhs, minus = 0;

    for (std::size_t i = 0; i < len; ++i) {
      lhs = it1 == a.data.end() ? 0 : *it1;
      rhs = it2 == b.data.end() ? 0 : *it2;

      // 由于减法会发生下溢出，因此写到右边去
      if (lhs < rhs + minus) {
        result.data.push_back(lhs + UNSIGNED_MAX - minus - rhs);
        minus = 1;
      } else {
        result.data.push_back(lhs - minus - rhs);
        minus = 0;
      }

      if (it1 != a.data.end()) ++it1;
      if (it2 != b.data.end()) ++it2;
    }

    result.fix();
    return result;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
// 辅助函数 mul
// 用于调用乘法函数

template<std::size_t M>
inline auto BigInteger<M>::mul(const BigInteger &a, const BigInteger &b) -> BigInteger {
  std::size_t n = a.data.size(), m = b.data.size();

  // 事实上，Karatsuba 是一种很容易推广的算法，例如如果分成四段，可以得到时间复杂度为 O(n^{log{7}/log{4}}) 的做法
  // 但是，作为课程设计，此处只是说明原理的可行性，故没有针对更多的数据规模进行细分采用不同的数据规模处理
  // 当 N 足够大时，FFT 的优势就体现出来了，但一般 N 至少要到 5000 量级，这意味着除非我们的模数是 2^16000 量级，FFT 才会比 TOOM-8H 有明显优势
  if (n > MUL_KARATSUBA_THRESHOLD && m > MUL_KARATSUBA_THRESHOLD) {
    return mul_karatsuba(a, b);
  } else {
    return mul_base(a, b);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
// 辅助函数 mul_base
// 实现朴素乘法函数

template<std::size_t M>
auto BigInteger<M>::mul_base(const BigInteger &a, const BigInteger &b) -> BigInteger {
  BigInteger result;
  auto it2 = b.data.begin();
  std::size_t cnt = 0;

  // 枚举其中一个链表中的元素，然后遍历另一个链表，将乘法转化为加法
  while (it2 != b.data.end()) {
    BigInteger tmp;
    tmp.data.reconstruct(cnt, 0);

    Integral rem = 0, cur0, cur1;
    auto it1 = a.data.begin();
    for (; it1 != a.data.end() && tmp.data.size() < LIMIT_NUMS; ++it1) {
      // 防止溢出 unsigned long long，先乘再进位
      cur0 = 1ULL * (*it2) * (*it1);
      cur1 = cur0 & UNSIGNED_MASK;
      cur0 >>= UNSIGNED_LEN;

      cur1 += rem;
      cur0 += cur1 >> UNSIGNED_LEN;
      cur1 &= UNSIGNED_MASK;

      tmp.data.push_back(cur1);
      rem = cur0;
    }

    if (rem > 0) {
      tmp.data.push_back(rem);
    }
    result += tmp;
    ++cnt, ++it2;
  }

  result.fix();
  return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
// 辅助函数 mul_karatsuba
// 用于调用 karatsuba 递归函数

template<std::size_t M>
auto BigInteger<M>::mul_karatsuba(const BigInteger &a, const BigInteger &b) -> BigInteger {
  BigInteger result = mul_karatsuba_impl(a, b);
  result.fix();
  return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
// 辅助函数 mul_karatsuba_impl
// Karatsuba 主要过程（分治）

template<std::size_t M>
auto BigInteger<M>::mul_karatsuba_impl(const BigInteger &a, const BigInteger &b) -> BigInteger {
  std::size_t n = a.data.size(), m = b.data.size();

  // 如果小于阈值，调用朴素乘法
  if (n < MUL_KARATSUBA_THRESHOLD || m < MUL_KARATSUBA_THRESHOLD) {
    return mul_base(a, b);
  }

  // 特殊情况
  if (n == 0 || m == 0) {
    return BigInteger(0);
  }

  std::size_t mx = std::max(n, m);

  // 链表分裂
  auto ab = a.data.split(mx / 2);
  auto cd = b.data.split(mx / 2);

  BigInteger A(ab.second), C(cd.second), B(ab.first), D(cd.first);
  BigInteger AC, BD, ABCD;

  // 通过分治得到三个局部结果
  AC.swap(mul_karatsuba_impl(A, C));
  BD.swap(mul_karatsuba_impl(B, D));
  ABCD.swap(mul_karatsuba_impl(A + B, C + D));

  // 利用局部结果计算乘积
  return shl_block(AC, mx / 2 * 2) + shl_block(ABCD - AC - BD, mx / 2) + BD;
}

/////////////////////////////////////////////////////////////////////////////////////////
// 辅助函数 div
// 调用除法的实现，并排除平凡情况

template<std::size_t M>
auto BigInteger<M>::div(const BigInteger &a, const BigInteger &b) -> BigInteger {
  // 对于比较平凡的情况进行特判
  if (b.data.empty())
    throw std::logic_error("division by zero");

  if (a.data.empty() || a < b)
    return BigInteger(0);

  std::size_t n = a.data.size(), m = b.data.size();

  // 如果小于二分阈值，则调用朴素除法
  if (n < DIV_BINARY_SEARCH_THRESHOLD && m < DIV_BINARY_SEARCH_THRESHOLD)
    return div_base(a, b);

  // 调用二分除法
  return div_binary_search(a, b);
}

/////////////////////////////////////////////////////////////////////////////////////////
// 辅助函数 div_base
// 转换乘减法的除法

template<std::size_t M>
auto BigInteger<M>::div_base(const BigInteger &a, const BigInteger &b) -> BigInteger {
  BigInteger mod = a, div;

  for (std::size_t i = a.data.size(); i > 0; --i) {
    Integral res = 0;

    // 枚举 2^k * b 是否能被减去，能减去就减去
    for (std::size_t j = UNSIGNED_LEN; j > 0; --j) {
      BigInteger tmp(shl_inside_block(shl_block(b, i - 1), j - 1));

      if (mod >= tmp) {
        mod -= tmp;
        res |= (1 << (j - 1));
      }
    }

    div.data.push_front(res);
  }

  div.fix();
  return div;
}

/////////////////////////////////////////////////////////////////////////////////////////
// 辅助函数 div_binary_search
// 采用二分和乘法验证的除法

template<std::size_t M>
auto BigInteger<M>::div_binary_search(const BigInteger &a, const BigInteger &b) -> BigInteger {
  BigInteger L(1), R(a);

  while (L <= R) {
    // 由于求 L + R 可能上溢出被取模，为此分类讨论
    BigInteger mid = div_by_two(L) + div_by_two(R) + ((L.data.front() & 1) && (R.data.front() & 1));

    // 二分
    if (mid * b > a)
      R = mid - 1;
    else
      L = mid + 1;
  }

  BigInteger result(R.data);
  result.fix();
  return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
// 辅助函数 pow
// 调用幂次的实现

template<std::size_t M>
auto BigInteger<M>::pow(const BigInteger &a, const BigInteger &b) -> BigInteger {
  // 在实际测试中发现，另外两种幂次虽然乘法次数少了但是跑的更慢，所以这里并没有调用另外两种实现
  return pow_base(a, b);
}

/////////////////////////////////////////////////////////////////////////////////////////
// 辅助函数 pow_base
// 使用快速幂进行求幂次

template<std::size_t M>
auto BigInteger<M>::pow_base(const BigInteger &a, const BigInteger &b) -> BigInteger {
  if (b.data.empty())
    return BigInteger(1);

  BigInteger result(1), A(a);

  // 快速幂主体
  for (auto it = b.data.begin(); it != b.data.end(); ++it) {
    for (std::size_t i = 0; i < UNSIGNED_LEN; ++i) {
      if ((*it >> i) & 1)
        result = result * A;
      A = A * A;
    }
  }

  return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
// 辅助函数 pow_packing
// 打包快速幂

template<std::size_t M>
auto BigInteger<M>::pow_packing(const BigInteger &a, const BigInteger &b) -> BigInteger {
  if (b.data.empty())
    return BigInteger(1);

  auto g = new BigInteger[POW_PACKING_WINDOW_STORAGE_SIZE];

  // 预处理
  g[0] = BigInteger(1), g[1] = a;
  for (std::size_t i = 2; i < POW_PACKING_WINDOW_STORAGE_SIZE; ++i)
    g[i] = g[i - 1] * g[1];

  BigInteger result(1), exp(shl(BigInteger(1), POW_PACKING_WINDOW_LENGTH));

  // 分块进行操作
  auto it = b.data.end();
  do {
    --it;

    // 每一轮操作后结果为 result ^ {2 ^ length} * 这段区间的值，与位的权值对应
    result = pow_base(result, exp) * g[*it >> (POW_PACKING_WINDOW_LENGTH * 3) & POW_PACKING_WINDOW_MASK];
    result = pow_base(result, exp) * g[*it >> (POW_PACKING_WINDOW_LENGTH * 2) & POW_PACKING_WINDOW_MASK];
    result = pow_base(result, exp) * g[*it >> (POW_PACKING_WINDOW_LENGTH * 1) & POW_PACKING_WINDOW_MASK];
    result = pow_base(result, exp) * g[*it >> (POW_PACKING_WINDOW_LENGTH * 0) & POW_PACKING_WINDOW_MASK];
  } while (it != b.data.begin());

  result.fix();
  return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
// 辅助函数 pow_sliding_window
// 采用滑动窗口方法，但实测跑的更慢，于是没有被采用

template<std::size_t M>
auto BigInteger<M>::pow_sliding_window(const BigInteger &a, const BigInteger &b) -> BigInteger {
  if (b.data.empty())
    return BigInteger(1);

  auto g = new BigInteger[POW_SLIDING_WINDOW_STORAGE_SIZE];

  // 预处理奇数
  g[0] = BigInteger(1), g[1] = a, g[2] = a * a;
  for (std::size_t i = 3; i < POW_SLIDING_WINDOW_STORAGE_SIZE; i += 2)
    g[i] = g[i - 2] * g[2];

  BigInteger result(1);
  auto pos = std::make_pair(b.data.end(), (std::int64_t)-1);

  do {
    // 解决下溢出问题
    if (pos.second < 0) {
      pos.second += UNSIGNED_LEN;
      --pos.first;
    }

    // 如果是 0，直接平方
    if ((((*pos.first) >> pos.second) & 1) == 0) {
      result = result * result, --pos.second;
    } else {
      std::int64_t index = pos.second - POW_SLIDING_WINDOW_STORAGE_SIZE, mask = -1;
      auto nxt = pos.first;
      --nxt;

      for (std::int64_t i = pos.second - POW_SLIDING_WINDOW_LENGTH + 1; i <= pos.second; ++i) {
        if (i < 0) {
          if (pos.first == b.data.begin())
            continue;

          // 如果 1 在前一个数中，记录位置并计算区间对应的值
          if ((*nxt >> (UNSIGNED_LEN + i)) & 1) {
            index = i;
            mask = (((*pos.first) & ((1LL << (pos.second + 1)) - 1)) << (-i)) | ((*nxt) >> (UNSIGNED_LEN + i));
            break;
          }
        } else {
          // 如果 1 在当前数中，记录位置并记录区间对应的值
          if (((*pos.first) >> i) & 1) {
            index = i;
            mask = (*pos.first >> index) & ((1LL << (pos.second - index + 1)) - 1);
            break;
          }
        }
      }

      // 计算这轮的结果并更新位置
      result = pow_base(result, shl_inside_block(BigInteger(1), pos.second - index + 1)) * g[mask];
      pos.second = index - 1;
    }
  } while (pos.first != b.data.begin() || pos.second >= 0);

  delete[] g;
  return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
// 辅助函数 equal
// 判断大整数是否相等

template<std::size_t M>
auto BigInteger<M>::equal(const BigInteger &a, const BigInteger &b) -> bool {
  return a.data == b.data;
}

/////////////////////////////////////////////////////////////////////////////////////////
// 辅助函数 less_than
// 判断大整数偏序

template<std::size_t M>
auto BigInteger<M>::less_than(const BigInteger &a, const BigInteger &b) -> bool {
  // 如果大小不同，则比较大小
  if (a.data.size() != b.data.size())
    return a.data.size() < b.data.size();

  auto it1 = a.data.end(), it2 = b.data.end();
  do {
    --it1, --it2;

    // 找到第一个不同的位置，可以确定大小关系
    if (*it1 != *it2)
      return *it1 < *it2;
  } while (it1 != a.data.begin() && it2 != b.data.begin());

  // 相等，返回 false
  return false;
}

/////////////////////////////////////////////////////////////////////////////////////////
// 辅助函数 fix
// 用于快速取模和去除前导 0

template<std::size_t M>
inline auto BigInteger<M>::fix() -> void {
  // 如果超出上限，则直接截断
  while (!data.empty() && data.size() > LIMIT_NUMS)
    data.pop_back();

  // 对剩余的若干位进行截断操作
  if (!data.empty()) {
    data.back() &= UNSIGNED_BIT_MASKS[REM_BITS];
  }

  // 如果有前导 0，则直接删除前导 0
  while (!data.empty() && data.back() == 0)
    data.pop_back();
}

/////////////////////////////////////////////////////////////////////////////////////////
// 辅助函数 shl
// 快速乘以 2 ^ k, for any k

template<std::size_t M>
inline auto BigInteger<M>::shl(const BigInteger &x, std::size_t count) -> BigInteger {
  return shl_inside_block(shl_block(x, count / UNSIGNED_LEN), count % UNSIGNED_LEN);
}

/////////////////////////////////////////////////////////////////////////////////////////
// 辅助函数 shl_block
// 快速乘以 (2 ^ 32) ^ count

template<std::size_t M>
inline auto BigInteger<M>::shl_block(const BigInteger &x, std::size_t count) -> BigInteger {
  BigInteger result(x);
  // 添加整块
  for (std::size_t i = 0; i < count; ++i) {
    result.data.push_front(0);
  }
  result.fix();
  return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
// 辅助函数 shl_inside_block
// 快速乘以 2 ^ k, k < 32

template<std::size_t M>
auto BigInteger<M>::shl_inside_block(const BigInteger &x, std::size_t count) -> BigInteger {
  if (count == 0)
    return x;

  BigInteger result;

  assert(count < UNSIGNED_LEN);

  // 进行块内左移操作
  result.data.push_back(0);
  Integral mask = (1LL << (UNSIGNED_LEN - count)) - 1;

  for (auto i : x.data) {
    result.data.back() |= (i & mask) << count;
    result.data.push_back(i >> (UNSIGNED_LEN - count));
  }

  result.fix();
  return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
// 辅助函数 div_by_two
// 快速除以 2

template<std::size_t M>
auto BigInteger<M>::div_by_two(const BigInteger &x) -> BigInteger {
  if (x.data.empty())
    return x;

  BigInteger result;
  result.data.push_back(x.data.front() >> 1);

  // 进行分段右移操作
  auto it = x.data.begin();
  ++it;

  for (; it != x.data.end(); ++it) {
    result.data.back() |= (*it & 1) << 31;
    result.data.push_back(*it >> 1);
  }

  result.fix();
  return result;
}

/////////////////////////////////////////////////////////////////////////////////////////
// 辅助函数 decimal_to_binary
// 将十进制数字（字符串类型）转成 2^32 进制（链表类型）

template<std::size_t M>
auto BigInteger<M>::decimal_to_binary(const std::string &s, List<unsigned int> &list) -> void {
  if (s.empty())
    return;

  auto *a = new Integral[s.length() + 1];
  std::size_t len = s.length();
  std::string res;

  // 转换为数值数组
  for (std::size_t i = 1; i <= len; ++i) {
    if (!std::isdigit(s[i - 1]))
      throw std::logic_error("invalid number");

    a[i] = s[i - 1] - '0';
  }

  // 采取直接除以 2^32 取余的方式
  Integral x = 0;
  for (std::size_t i = 1; i <= len; ++i) {
    Integral div = (x * 10 + a[i]) >> UNSIGNED_LEN;
    x = (x * 10 + a[i]) & UNSIGNED_MASK;

    res.push_back((char)div + '0');
  }

  // 消除前导 0
  bool vis = false;
  for (std::size_t i = 0; i < res.length(); ++i) {
    if (res[i] != '0') {
      res = res.substr(i);
      vis = true;
      break;
    }
  }
  if (!vis) res.clear();

  // 把余数放入链表
  list.push_back(x);

  delete[] a;

  // 对一次除法得到的结果递归处理，直至被除数为 0
  decimal_to_binary(res, list);
}

/////////////////////////////////////////////////////////////////////////////////////////
// 辅助函数 binary_to_decimal
// 将 2^32 进制（链表类型）转成十进制数字（字符串类型）

template<std::size_t M>
auto BigInteger<M>::binary_to_decimal(const List<unsigned int> &list, std::string &s) -> void {
  auto it = list.begin();
  std::string base = "1";
  std::string result;

  for (; it != list.end(); ++it) {
    // 转化为二进制转十进制，可以避免写乘法，时间复杂度相同
    for (std::size_t i = 0; i < UNSIGNED_LEN; ++i) {
      if ((*it >> i) & 1)
        result = decimal_add(result, base);
      base = decimal_add(base, base);
    }
  }

  s.swap(result);
  std::reverse(s.begin(), s.end());

  // 消除前导 0
  bool vis = false;
  for (std::size_t i = 0; i < s.length(); ++i) {
    if (s[i] != '0') {
      s = s.substr(i);
      vis = true;
      break;
    }
  }
  // 针对 0 特殊处理
  if (!vis) s = "0";
}

/////////////////////////////////////////////////////////////////////////////////////////
// 辅助函数 decimal_add
// 实现十进制数字相加，用于辅助进制转换

template<std::size_t M>
auto BigInteger<M>::decimal_add(std::string &a, std::string &b) -> std::string {
  std::string result;
  std::size_t len = std::max(a.length(), b.length());

  Integral rem = 0, lhs, rhs;
  for (std::size_t i = 0; i < len; ++i) {
    lhs = i < a.length() ? a[i] - '0' : 0;
    rhs = i < b.length() ? b[i] - '0' : 0;

    rem += lhs + rhs;

    result.push_back((char)(rem % 10) + '0');
    rem /= 10;
  }

  if (rem > 0) {
    result.push_back((char)rem + '0');
  }

  return result;
}

/////////////////////////////////////////////////////////////////////////////////////////

#endif //FDS_BIG_INTEGER_IMPL_

