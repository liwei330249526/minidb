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
// Created by WangYunlai on 2022/12/08.
// order_by_logical_operator.h
//

#pragma once

#include <memory>
#include <vector>

#include "sql/expr/expression.h"
#include "sql/operator/logical_operator.h"
#include "storage/field/field.h"

/**
 * @brief project 表示投影运算
 * @ingroup LogicalOperator
 * @details 从表中获取数据后，可能需要过滤，投影，连接等等。
 */
class OrderByStmt_t;
class OrderByLogicalOperator : public LogicalOperator
{
public:
    explicit OrderByLogicalOperator(unique_ptr<OrderByStmt_t> &order_by_stmt);
    virtual ~OrderByLogicalOperator() = default;

    LogicalOperatorType type() const override { return LogicalOperatorType::ORDER_BY; }
    unique_ptr<OrderByStmt_t> &getOrderByStmt() ;
    void setOrderByStmt(unique_ptr<OrderByStmt_t> &orderByStmt);

    unique_ptr<OrderByStmt_t> order_by_stmt_;
};
