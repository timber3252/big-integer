#ifndef FDS_LIST_
#define FDS_LIST_

#include <cstdint>

// 双向循环链表节点定义
template <class T>
struct ListNode {
  T data;
  ListNode *next, *prev;
  explicit ListNode(const T& val);
};

// 双向循环链表迭代器定义
template <class T>
class ListIterator {
  ListNode<T> *node;

 public: // 构造函数
  explicit ListIterator(ListNode<T>* raw_ptr = nullptr);

 public: // 迭代器的迭代操作
  auto operator++() -> ListIterator&;
  auto operator++(int) -> ListIterator const;
  auto operator--() -> ListIterator&;
  auto operator--(int) -> ListIterator const;

 public:// 解引用和访问运算符
  auto operator*() const -> T&;
  auto operator->() const -> T*;
  auto operator==(const ListIterator &other) const -> bool;
  auto operator!=(const ListIterator &other) const -> bool;

 public:// 获取原始指针
  auto raw() -> ListNode<T>*;
};

template <class T>
class List {
 public: // 头节点和大小
  ListNode<T> *node;
  std::size_t siz;

 private: // 头节点操作
  void _init_node();
  void _destroy_node();

 public: // 构造与析构函数
  List();
  explicit List(std::size_t count, const T& value = T());
  List(const List &other);
  ~List();
  auto reconstruct(const List &other) -> void;
  auto reconstruct(std::size_t count, const T& value) -> void;

 public: // 拷贝与交换
  auto operator=(const List &other) -> List& = delete;
  auto swap(List &other) -> void;
  auto swap(List &&other) -> void;

 public: // 返回头尾元素
  auto front() -> T&;
  auto front() const -> const T&;
  auto back() -> T&;
  auto back() const -> const T&;

 public: // 获取头尾迭代器
  auto begin() -> ListIterator<T>;
  auto begin() const -> ListIterator<T> const;
  auto end() -> ListIterator<T>;
  auto end() const -> ListIterator<T> const;

 public: // 判空与获取容量
  auto empty() const -> bool;
  auto size() const -> std::size_t;

 public: // 链表插入、删除、清空
  auto clear() -> void;
  auto insert(ListIterator<T> pos, const T &val) -> ListIterator<T>; // 在指定迭代器前面插入元素
  auto erase(ListIterator<T> pos) -> ListIterator<T>; // 基于迭代器删除元素
  auto erase(const T& val) -> std::size_t; // 基于权值删除元素

 public: // 头尾增删元素
  auto push_back(const T& val) -> void;
  auto push_front(const T& val) -> void;
  auto pop_back() -> void;
  auto pop_front() -> void;

 public: // 链表分裂为 [first, pos), [pos, last)
  auto split(std::size_t pos) const -> std::pair<List<T>, List<T>>;

 public: // 两个链表的拼接
  auto merge(const List<T> &other) -> void;
  auto operator+(const List<T> &other) -> List<T>;
  auto operator+=(const List<T> &other) -> List<T>&;

 public: // 比较是否相等
  auto operator==(const List<T> &other) const -> bool;
  auto operator!=(const List<T> &other) const -> bool;

 public: // 比较字典序
  auto operator<(const List<T> &other) const -> bool;
  auto operator>=(const List<T> &other) const -> bool;
  auto operator>(const List<T> &other) const -> bool;
  auto operator<=(const List<T> &other) const -> bool;
};

#endif //FDS_LIST_

#include "list_impl.h"