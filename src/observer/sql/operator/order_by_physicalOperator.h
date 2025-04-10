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

#pragma once

#include "sql/operator/physical_operator.h"
#include "sql/expr/expression_tuple.h"

/**
 * @brief 选择/投影物理算子
 * @ingroup PhysicalOperator
 */
 class OrderByStmt_t;
class OrderByPhysicalOperator : public PhysicalOperator
{
public:
    explicit OrderByPhysicalOperator(unique_ptr<OrderByStmt_t> &&OrderByStmt);

    virtual ~OrderByPhysicalOperator() = default;

    PhysicalOperatorType type() const override { return PhysicalOperatorType::ORDER_BY; }

    RC open(Trx *trx) override;
    RC next() override;
    RC close() override;

    Tuple *current_tuple() override;

//    RC tuple_schema(TupleSchema &schema) const override;

private:
//    std::vector<std::unique_ptr<Expression>>     expressions_;  // 投影算子的表达式，例如 select id+1 from exp_table;   id+1; left 是 filed ， right 是valu
//    ExpressionTuple<std::unique_ptr<Expression>> tuple_; // 投影算子的 tuple

    unique_ptr<OrderByStmt_t> OrderByStmt_;
    std::vector<unique_ptr<ValueListTuple>>   sorted_tuples_; // 排序后的元组; 这里保存一份所有数据
    size_t                   current_index_; // 当前处理的元组索引
};
