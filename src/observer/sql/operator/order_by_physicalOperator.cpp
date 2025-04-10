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
// Created by WangYunlai on 2022/07/01.
//

#include "sql/operator/order_by_physicalOperator.h"
#include "common/log/log.h"
#include "storage/record/record.h"
#include "storage/table/table.h"
#include "sql/stmt/select_stmt.h"
#include <memory>
#include <algorithm>

using namespace std;

OrderByPhysicalOperator::OrderByPhysicalOperator(unique_ptr<OrderByStmt_t> &&OrderByStmt)
        : OrderByStmt_(std::move(OrderByStmt)), current_index_(0)
{
}

RC OrderByPhysicalOperator::open(Trx *trx)
{
  if (children_.empty()) {
    return RC::SUCCESS;
  }

  PhysicalOperator *child = children_[0].get();
  RC                rc    = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }
  // 获取所有数据行
  while ((rc = child->next()) == RC::SUCCESS) {
    Tuple *tuple = child->current_tuple();
    ValueListTuple val_list;
    rc = ValueListTuple::make(*tuple, val_list);
    if (OB_FAIL(rc)) {
      LOG_WARN("failed to get values from expression tuple. rc=%s", strrc(rc));
      return rc;
    }
    sorted_tuples_.push_back(std::make_unique<ValueListTuple>(val_list));
//    Value value;
//    right_tuple->get_value(0, value); // 假设比较第0列
//    right_tuple->cell_at(0, value);
//    string keyStr = value.to_string();
//    hash_table_.emplace(keyStr, right_tuple); // key 是这一列的值， val 是一行的值
  }


  // 定义比较函数
  auto compare = [this](unique_ptr<ValueListTuple> &a, unique_ptr<ValueListTuple> &b) {
      for (size_t i = 0; i < OrderByStmt_->group_by_.size(); i++) {
        // 这里假设 Expression 有一个 evaluate 方法来获取值
        // 实际实现中需要根据具体情况进行调整
        // 这里简单示例，假设可以比较
        // 实际中可能需要处理不同类型的比较
        // 这里只是一个占位实现
        // 可以根据需要修改
        Value leftValue;
        Value rightValue;
        OrderByStmt_->group_by_[i]->get_value(*a, leftValue);
        OrderByStmt_->group_by_[i]->get_value(*b, rightValue);
//        *  -1 表示 left < right
//        *  0 表示 left = right
//        *  1 表示 left > right
        int ret = leftValue.compare(rightValue);
        if (ret != 0) {
          if (OrderByStmt_->dirs_[i]) {  // 升序 , left < right      -1
            return ret == -1;
          } else { // 降序
            return ret == 1;            // 降序 ， left > right     1
          }
        }
        // ret == 0, 继续判断下一个
      }
      // 都判断完了，则相等，相等就返回个true 把
      return false;
  };
  // 排序

  std::sort(sorted_tuples_.begin(), sorted_tuples_.end(), compare);
  child->close();
  return rc == RC::RECORD_EOF ? RC::SUCCESS : rc;
}
// 调儿子的next
RC OrderByPhysicalOperator::next()
{
  if (current_index_ >= sorted_tuples_.size()) {
    return RC::RECORD_EOF;
  }
  current_index_++;
  return RC::SUCCESS;
}

RC OrderByPhysicalOperator::close()
{
  sorted_tuples_.clear();
  return RC::SUCCESS;
}
// project 从 child 获取一个tuple， 设置给自己
Tuple *OrderByPhysicalOperator::current_tuple()
{
  return sorted_tuples_[current_index_-1].get(); // 子类指针，返回给父类
}