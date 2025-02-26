/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2022.09.02
//

#pragma once

#include "common/lang/functional.h"
#include "common/lang/unordered_set.h"

namespace common {
// FrameId, Frame *, BPFrameIdHasher
template <typename Key, typename Value, typename Hash = hash<Key>, typename Pred = equal_to<Key>>
class LruCache
{
  // 这是一个双向链表节点类，
  class ListNode
  {
  public:
    Key   key_; // FrameId
    Value value_; // Frame *

    ListNode *prev_ = nullptr;
    ListNode *next_ = nullptr;

  public:
    ListNode(const Key &key, const Value &value) : key_(key), value_(value) {}
  };
  // 类， 重载了(), 使用hasher_ 计算key 的hash值
  // 这是一个函数对象类，用于计算 ListNode* 类型对象的哈希值。通过调用 hasher_ 对节点的键进行哈希计算。
  class PListNodeHasher
  {
  public:
    size_t operator()(ListNode *node) const
    {
      if (node == nullptr) {
        return 0;
      }
      return hasher_(node->key_);
    }

  private:
    Hash hasher_;
  };
  // 比较
  class PListNodePredicator
  {
  public:
    bool operator()(ListNode *const node1, ListNode *const node2) const
    {
      if (node1 == node2) {
        return true;
      }

      if (node1 == nullptr || node2 == nullptr) {
        return false;
      }

      return pred_(node1->key_, node2->key_);
    }

  private:
    Pred pred_;
  };

public:
  LruCache(size_t reserve = 0)
  {
    if (reserve > 0) {
      searcher_.reserve(reserve);
    }
  }

  ~LruCache() { destroy(); }
  // 遍历 searcher_ 中的所有节点，释放节点的内存，然后清空 searcher_，并将链表的头指针和尾指针置为 nullptr
  void destroy()
  {
    for (ListNode *node : searcher_) {
      delete node;
    }
    searcher_.clear();

    lru_front_ = nullptr;
    lru_tail_  = nullptr;
  }
  // 返回缓存中键值对的数量。
  size_t count() const { return searcher_.size(); }
  // 尝试从缓存中查找指定键的值。如果找到，调用 lru_touch() 方法将该节点移动到链表头部（表示最近使用），并将值赋给 value，返回 true；如果未找到，返回 false。
  bool get(const Key &key, Value &value)
  {
    auto iter = searcher_.find((ListNode *)&key);
    if (iter == searcher_.end()) {
      return false;
    }

    lru_touch(*iter);
    value = (*iter)->value_;
    return true;
  }
  // 如果键已经存在于缓存中，更新该键对应的值，并将该节点移动到链表头部；如果键不存在，创建一个新的节点，并将其插入到链表头部。
  void put(const Key &key, const Value &value)
  {
    auto iter = searcher_.find((ListNode *)&key);
    if (iter != searcher_.end()) {
      ListNode *ln = *iter;
      ln->value_   = value;
      lru_touch(ln);
      return;
    }

    ListNode *ln = new ListNode(key, value);
    lru_push(ln);
  }
  // 尝试从缓存中移除指定键的节点。如果找到，调用 lru_remove() 方法将该节点从链表和 searcher_ 中移除。
  void remove(const Key &key)
  {
    auto iter = searcher_.find((ListNode *)&key);
    if (iter != searcher_.end()) {
      lru_remove(*iter);
    }
  }

  void pop(Value *&value)
  {
    // TODO
    value = nullptr;
  }

  void foreach (function<bool(const Key &, const Value &)> func)
  {
    for (ListNode *node = lru_front_; node != nullptr; node = node->next_) {
      bool ret = func(node->key_, node->value_);
      if (!ret) {
        break;
      }
    }
  }

  void foreach_reverse(function<bool(const Key &, const Value &)> func)
  {
    for (ListNode *node = lru_tail_; node != nullptr; node = node->prev_) {
      bool ret = func(node->key_, node->value_);
      if (!ret) {
        break;
      }
    }
  }

private:
		// 方法将指定节点移动到链表头部，表示该节点最近被使用。
  void lru_touch(ListNode *node)
  {
    // move node to front
    if (nullptr == node->prev_) {
      return;
    }

    node->prev_->next_ = node->next_;

    if (node->next_ != nullptr) {
      node->next_->prev_ = node->prev_;
    } else {
      lru_tail_ = node->prev_;
    }

    node->prev_ = nullptr;
    node->next_ = lru_front_;
    if (lru_front_ != nullptr) {
      lru_front_->prev_ = node;
    }
    lru_front_ = node;
  }
  // 方法将指定节点插入到链表头部。
  void lru_push(ListNode *node)
  {
    // push front
    if (nullptr == lru_tail_) {
      lru_tail_ = node;
    }

    node->prev_ = nullptr;
    node->next_ = lru_front_;
    if (lru_front_ != nullptr) {
      lru_front_->prev_ = node;
    }

    lru_front_ = node;
    searcher_.insert(node);
  }
  // 方法将指定节点从链表和 searcher_ 中移除，并释放节点的内存。
  void lru_remove(ListNode *node)
  {
    if (node->prev_ != nullptr) {
      node->prev_->next_ = node->next_;
    }

    if (node->next_ != nullptr) {
      node->next_->prev_ = node->prev_;
    }

    if (lru_front_ == node) {
      lru_front_ = node->next_;
    }
    if (lru_tail_ == node) {
      lru_tail_ = node->prev_;
    }

    searcher_.erase(node);
    delete node;
  }

private:
  using SearchType = unordered_set<ListNode *, PListNodeHasher, PListNodePredicator>;
  SearchType searcher_;
  ListNode  *lru_front_ = nullptr;
  ListNode  *lru_tail_  = nullptr;
};

}  // namespace common
