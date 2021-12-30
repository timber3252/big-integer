#ifndef FDS_LIST_IMPL_
#define FDS_LIST_IMPL_

#include "list.h"

/////////////////////////////////////////////////////////////////////////////////////////
// ListNode 构造函数实现

template<class T>
inline ListNode<T>::ListNode(const T &val) : data(val) {}

/////////////////////////////////////////////////////////////////////////////////////////
// ListIterator 构造函数实现

template<class T>
inline ListIterator<T>::ListIterator(ListNode<T> *raw_ptr) : node(raw_ptr) {}

/////////////////////////////////////////////////////////////////////////////////////////
// ListIterator 的迭代操作实现

template<class T>
inline auto ListIterator<T>::operator++() -> ListIterator<T>& { // 迭代器前置 ++
  node = node->next;
  return *this;
}
template<class T>
inline auto ListIterator<T>::operator++(int) -> ListIterator const { // 迭代器后置 ++
  auto tmp = *this;
  node = node->next;
  return tmp;
}
template<class T>
inline auto ListIterator<T>::operator--() -> ListIterator& { // 迭代器前置 --
  node = node->prev;
  return *this;
}
template<class T>
inline auto ListIterator<T>::operator--(int) -> ListIterator const { // 迭代器后置 --
  auto tmp = *this;
  node = node->prev;
  return tmp;
}

/////////////////////////////////////////////////////////////////////////////////////////
// ListIterator 解引用和访问运算符实现

template<class T>
inline auto ListIterator<T>::operator*() const -> T & { return node->data; }
template<class T>
inline auto ListIterator<T>::operator->() const -> T * { return &(operator*()); }

/////////////////////////////////////////////////////////////////////////////////////////
// ListIterator 判断是否相等实现

template<class T>
inline auto ListIterator<T>::operator==(const ListIterator &other) const -> bool { return node == other.node; }
template<class T>
inline auto ListIterator<T>::operator!=(const ListIterator &other) const -> bool { return node != other.node; }

/////////////////////////////////////////////////////////////////////////////////////////
// ListIterator 获取原始指针实现

template<class T>
inline auto ListIterator<T>::raw() -> ListNode<T> * { return node; }

/////////////////////////////////////////////////////////////////////////////////////////
// List 头节点辅助函数实现

template<class T>
inline auto List<T>::_init_node() -> void { // 构造头节点
  node = new ListNode<T>(T());
  node->next = node;
  node->prev = node;
}
template<class T>
inline auto List<T>::_destroy_node() -> void { // 销毁头节点
  delete node;
}

/////////////////////////////////////////////////////////////////////////////////////////
// List 构造函数和析构函数实现

template<class T>
inline List<T>::List() : siz() { _init_node(); } // 构造函数

template<class T>
List<T>::List(std::size_t count, const T &value) : siz() { // 构造函数
  _init_node();
  for (std::size_t i = 0; i < count; ++i) {
    push_back(value);
  }
}

template<class T>
List<T>::List(const List &other) : siz() { // 复制构造函数
  _init_node();
  for (ListIterator<T> it = other.begin(); it != other.end(); ++it) {
    push_back(*it);
  }
}

template<class T>
inline List<T>::~List() { // 析构函数
  _destroy_node();
}

template<class T>
auto List<T>::reconstruct(const List &other) -> void {
  this->~List();
  new (this)List(other);
}

template<class T>
auto List<T>::reconstruct(std::size_t count, const T& value) -> void {
  this->~List();
  new (this)List(count, value);
}

/////////////////////////////////////////////////////////////////////////////////////////
// List 拷贝（禁止拷贝）与交换

template<class T>
auto List<T>::swap(List &other) -> void { std::swap(node, other.node), std::swap(siz, other.siz); }

template<class T>
auto List<T>::swap(List &&other) -> void {
  std::swap(node, other.node);
  std::swap(siz, other.siz);
}

/////////////////////////////////////////////////////////////////////////////////////////
// List 取头尾元素实现

template<class T>
inline auto List<T>::front() -> T& { return *begin(); }
template<class T>
inline auto List<T>::front() const -> const T& { return *begin(); }
template<class T>
inline auto List<T>::back() -> T& { return *(--end()); }
template<class T>
inline auto List<T>::back() const -> const T& { return *(--end()); }

/////////////////////////////////////////////////////////////////////////////////////////
// ListIterator 取头尾迭代器实现

template<class T>
inline auto List<T>::begin() -> ListIterator<T> { return ListIterator<T>(node->next); }
template<class T>
inline auto List<T>::begin() const -> const ListIterator<T> { return ListIterator<T>(node->next); }
template<class T>
inline auto List<T>::end() -> ListIterator<T> { return ListIterator<T>(node); }
template<class T>
inline auto List<T>::end() const -> const ListIterator<T> { return ListIterator<T>(node); }

/////////////////////////////////////////////////////////////////////////////////////////
// List 容量相关，判空与获取大小

template<class T>
inline auto List<T>::empty() const -> bool { return node->next == node; }

template<class T>
auto List<T>::size() const -> std::size_t { return siz; }

/////////////////////////////////////////////////////////////////////////////////////////
// List 主要操作，插入、删除、清空等

template<class T>
auto List<T>::clear() -> void { // 清空链表
  ListNode<T> *cur = node->next;
  while (cur != node) {
    ListNode<T> *tmp = cur;
    delete tmp;
    cur = cur->next;
  }
  node->next = node->prev = node;
  siz = 0;
}

template<class T>
auto List<T>::insert(ListIterator<T> pos, const T &val) -> ListIterator<T> { // 在指定位置前插入元素
  auto *tmp = new ListNode<T>(val);
  tmp->next = pos.raw();
  tmp->prev = pos.raw()->prev;
  pos.raw()->prev->next = tmp;
  pos.raw()->prev = tmp;
  ++siz;
  return ListIterator<T>(tmp);
}

template<class T>
auto List<T>::erase(ListIterator<T> pos) -> ListIterator<T> { // 删除指定位置元素
  ListNode<T> *next_node = pos.raw()->next,
      *prev_node = pos.raw()->prev,
      *curr_node = pos.raw();
  next_node->prev = prev_node;
  prev_node->next = next_node;
  delete curr_node;
  --siz;
  return ListIterator<T>(next_node);
}

template<class T>
auto List<T>::erase(const T &val) -> std::size_t { // 按权值删除元素
  std::size_t count = 0;
  for (ListIterator<T> it = begin(); it != end(); ) {
    if (val == *it)
      it = erase(it), ++count;
    else ++it;
  }
  --siz;
  return count;
}

/////////////////////////////////////////////////////////////////////////////////////////
// List 在头尾插入删除实现

template<class T>
inline auto List<T>::push_back(const T &val) -> void { insert(end(), val); }
template<class T>
inline auto List<T>::push_front(const T &val) -> void { insert(begin(), val); }
template<class T>
inline auto List<T>::pop_back() -> void { erase(--end()); }
template<class T>
inline auto List<T>::pop_front() -> void { erase(begin()); }

/////////////////////////////////////////////////////////////////////////////////////////
template<class T>
auto List<T>::split(std::size_t pos) const -> std::pair<List<T>, List<T>> {
  List<unsigned> p1, p2;
  std::size_t siz = size();
  auto it = begin();

  for (std::size_t i = 0; i < pos && i < siz; ++i) {
    p1.push_back(*it++);
  }
  for (std::size_t i = pos; i < siz; ++i) {
    p2.push_back(*it++);
  }

  return {p1, p2};
}

/////////////////////////////////////////////////////////////////////////////////////////
// List 两个链表的拼接实现

template<class T>
auto List<T>::merge(const List<T> &other) -> void { // 合并链表
  for (ListIterator<T> it = other.begin(); it != other.end(); ++it) {
    push_back(*it);
  }
}

template<class T>
auto List<T>::operator+(const List<T> &other) -> List<T> { // 合并链表
  List<T> result(*this);
  result.merge(other);
  return result;
}

template<class T>
auto List<T>::operator+=(const List<T> &other) -> List<T>& { // 合并链表
  this->merge(other);
  return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////
// List 比较是否相等实现

template<class T>
auto List<T>::operator==(const List<T> &other) const -> bool { // 判断相等
  ListIterator<T> it1 = begin(), it2 = other.begin(), it3 = end(), it4 = other.end();
  while (it1 != it3 && it2 != it4 && *it1 == *it2) {
    ++it1, ++it2;
  }
  return it1 == it3 && it2 == it4;
}

template<class T>
inline auto List<T>::operator!=(const List<T> &other) const -> bool { return !(*this == other); } // 比较不相等

/////////////////////////////////////////////////////////////////////////////////////////
// List 比较字典序大小

template<class T>
auto List<T>::operator<(const List<T> &other) const -> bool { // 按字典序比较
  ListIterator<T> it1, it2;
  for (it1 = begin(), it2 = other.begin(); it1 != end() && it2 != other.end(); ++it1, ++it2) {
    if (*it1 != *it2)
      return *it1 < *it2;
  }
  return (it1 == end() && it2 != other.end());
}

// 比较字典序大小关系
template<class T>
inline auto List<T>::operator>=(const List<T> &other) const -> bool { return !(*this < other); }
template<class T>
inline auto List<T>::operator>(const List<T> &other) const -> bool { return other < *this; }
template<class T>
inline auto List<T>::operator<=(const List<T> &other) const -> bool { return other >= *this; }

/////////////////////////////////////////////////////////////////////////////////////////

#endif //FDS_LIST_IMPL_
