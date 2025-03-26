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
// Created by WangYunlai on 2022/12/30.
//

#include "sql/operator/join_physical_operator.h"
#include <string>

NestedLoopJoinPhysicalOperator::NestedLoopJoinPhysicalOperator() {}

RC NestedLoopJoinPhysicalOperator::open(Trx *trx)
{
  if (children_.size() != 2) {
    LOG_WARN("nlj operator should have 2 children");
    return RC::INTERNAL;
  }

  RC rc         = RC::SUCCESS;
  left_         = children_[0].get(); // 左边
  right_        = children_[1].get(); // 右边
  right_closed_ = true;
  round_done_   = true;

  rc   = left_->open(trx); // 打开左边
  trx_ = trx;
  return rc;
}

RC NestedLoopJoinPhysicalOperator::next()
{
  bool left_need_step = (left_tuple_ == nullptr);
  RC   rc             = RC::SUCCESS;
  if (round_done_) {    // 右表一轮结束
    left_need_step = true;
  } else {               // 右表一轮没有结束， 右表继续递进
    rc = right_next();
    if (rc != RC::SUCCESS) {
      if (rc == RC::RECORD_EOF) {
        left_need_step = true;
      } else {
        return rc;
      }
    } else {
      return rc;  // got one tuple from right
    }
  }

  if (left_need_step) { // 右边一轮结束，则坐标递进一次
    rc = left_next();
    if (rc != RC::SUCCESS) {
      return rc;
    }
  }

  rc = right_next();
  return rc;
}

RC NestedLoopJoinPhysicalOperator::close()
{
  RC rc = left_->close();
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to close left oper. rc=%s", strrc(rc));
  }

  if (!right_closed_) {
    rc = right_->close();
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to close right oper. rc=%s", strrc(rc));
    } else {
      right_closed_ = true;
    }
  }
  return rc;
}

Tuple *NestedLoopJoinPhysicalOperator::current_tuple() { return &joined_tuple_; }

RC NestedLoopJoinPhysicalOperator::left_next()
{
  RC rc = RC::SUCCESS;
  rc    = left_->next();
  if (rc != RC::SUCCESS) {
    return rc;
  }

  left_tuple_ = left_->current_tuple();
  joined_tuple_.set_left(left_tuple_);
  return rc;
}

RC NestedLoopJoinPhysicalOperator::right_next()
{
  RC rc = RC::SUCCESS;
	// 右表一轮结束了
  if (round_done_) {
	  // 如果右表算子结束没有结束
    if (!right_closed_) {
      rc = right_->close();

      right_closed_ = true;
      if (rc != RC::SUCCESS) {
        return rc;
      }
    }
    // 右表从新打开，从头开始遍历
    rc = right_->open(trx_);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    right_closed_ = false;

    round_done_ = false;
  }

  rc = right_->next(); // 右表获取next 值
  if (rc != RC::SUCCESS) {
    if (rc == RC::RECORD_EOF) {
      round_done_ = true;
    }
    return rc;
  }

  right_tuple_ = right_->current_tuple(); // 获取值
  joined_tuple_.set_right(right_tuple_); // 设置到 join 的tuple 中
  return rc;
}


// 实现部分
HashSemiJoinPhysicalOperator::HashSemiJoinPhysicalOperator() {}

RC HashSemiJoinPhysicalOperator::open(Trx *trx) {
  if (children_.size() != 2) {
    LOG_WARN("hash semi join operator should have 2 children");
    return RC::INTERNAL;
  }

  left_ = children_[0].get();
  right_ = children_[1].get();
  trx_ = trx;

  // 打开左表
  left_->open(trx);

  // 构建右表哈希表
  RC rc = build_hash_table();
  return rc;
}

RC HashSemiJoinPhysicalOperator::build_hash_table() {
  RC rc = right_->open(trx_);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  // 遍历右表构建哈希表
  while ((rc = right_->next()) == RC::SUCCESS) {
    Tuple *right_tuple = right_->current_tuple();
    Value value;
//    right_tuple->get_value(0, value); // 假设比较第0列
    right_tuple->cell_at(0, value);
    string keyStr = value.to_string();
    hash_table_.emplace(keyStr, right_tuple); // key 是这一列的值， val 是一行的值
  }

  right_->close();
  right_built_ = true;
  return rc == RC::RECORD_EOF ? RC::SUCCESS : rc;
}

RC HashSemiJoinPhysicalOperator::next() {
  while (true) {
    RC rc = left_next();
    if (rc != RC::SUCCESS) {
      return rc; // 左表结束或错误
    }

    Value left_value;
//    left_tuple_->get_value(0, left_value); // 获取左表比较值
    left_tuple_->cell_at(0, left_value);
    // 在哈希表中查找匹配
    string keyStr = left_value.to_string();
    auto it = hash_table_.find(keyStr);
    if (it != hash_table_.end()) {
      // 找到匹配：设置左右tuple
      matched_right_tuple_ = it->second;
      joined_tuple_.set_left(left_tuple_);
      joined_tuple_.set_right(matched_right_tuple_);
      return RC::SUCCESS;
    } else {
      // 未找到匹配：只设置左tuple
      joined_tuple_.set_left(left_tuple_);
      joined_tuple_.set_right(nullptr); // 明确置空右tuple
      return RC::SUCCESS;
    }
  }
}

RC HashSemiJoinPhysicalOperator::left_next() {
  RC rc = left_->next();
  if (rc == RC::SUCCESS) {
    left_tuple_ = left_->current_tuple();
  }
  return rc;
}

RC HashSemiJoinPhysicalOperator::close() {
  hash_table_.clear();
  return left_->close(); // 右表已在build_hash_table中关闭
}

Tuple *HashSemiJoinPhysicalOperator::current_tuple() {
  return &joined_tuple_;
}