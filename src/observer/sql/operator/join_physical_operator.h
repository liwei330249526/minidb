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
// Created by WangYunlai on 2021/6/10.
//

#pragma once

#include "sql/operator/physical_operator.h"
#include "sql/parser/parse.h"
#include <unordered_set>

/**
 * @brief 最简单的两表（称为左表、右表）join算子
 * @details 依次遍历左表的每一行，然后关联右表的每一行
 * @ingroup PhysicalOperator
 */
class NestedLoopJoinPhysicalOperator : public PhysicalOperator
{
public:
  NestedLoopJoinPhysicalOperator();
  virtual ~NestedLoopJoinPhysicalOperator() = default;

  PhysicalOperatorType type() const override { return PhysicalOperatorType::NESTED_LOOP_JOIN; }

  RC     open(Trx *trx) override;
  RC     next() override;
  RC     close() override;
  Tuple *current_tuple() override;

private:
  RC left_next();   //! 左表遍历下一条数据
  RC right_next();  //! 右表遍历下一条数据，如果上一轮结束了就重新开始新的一轮

private:
  Trx *trx_ = nullptr;

  //! 左表右表的真实对象是在PhysicalOperator::children_中，这里是为了写的时候更简单
  PhysicalOperator *left_        = nullptr;
  PhysicalOperator *right_       = nullptr;
  Tuple            *left_tuple_  = nullptr;
  Tuple            *right_tuple_ = nullptr;
  JoinedTuple       joined_tuple_;         //! 当前关联的左右两个tuple
  bool              round_done_   = true;  //! 右表遍历的一轮是否结束
  bool              right_closed_ = true;  //! 右表算子是否已经关闭
};


class HashSemiJoinPhysicalOperator : public PhysicalOperator {
public:
    HashSemiJoinPhysicalOperator();
    virtual ~HashSemiJoinPhysicalOperator() = default;

    PhysicalOperatorType type() const override {
      return PhysicalOperatorType::HASH_SEMI_JOIN;
    }

    RC open(Trx *trx) override;
    RC next() override;
    RC close() override;
    Tuple *current_tuple() override;
    Expression *getLeftFiled() const;
    void setLeftFiled(Expression *leftFiled);
private:
    RC build_hash_table();  // 构建右表哈希表（存储值和对应Tuple）
    RC left_next();        // 左表遍历下一条数据

private:
    Trx *trx_ = nullptr;
    Expression *left_filed_; // id in (subquery), 做表的 filed_;    (subquery) in (subquery))
    PhysicalOperator *left_ = nullptr;
    PhysicalOperator *right_ = nullptr;
    Tuple *left_tuple_ = nullptr;
    Tuple *matched_right_tuple_ = nullptr; // 记录当前匹配的右表Tuple
    JoinedTuple joined_tuple_;

    // 哈希表结构：存储右表Value和对应的Tuple指针
    std::unordered_map<string, Tuple*> hash_table_;
    bool right_built_ = false;
};
