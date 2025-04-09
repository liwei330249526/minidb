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
// Created by Wangyunlai on 2022/12/15
//

#include "sql/operator/project_logical_operator.h"
#include "order_by_logical_operator.h"
#include "sql/stmt/select_stmt.h"


using namespace std;

OrderByLogicalOperator::OrderByLogicalOperator(unique_ptr<OrderByStmt_t> &order_by_stmt) {
  order_by_stmt_ = std::move(order_by_stmt);
}

unique_ptr<OrderByStmt_t> &OrderByLogicalOperator::getOrderByStmt() {
  return order_by_stmt_;
}

void OrderByLogicalOperator::setOrderByStmt(unique_ptr<OrderByStmt_t> &orderByStmt) {
  order_by_stmt_ = std::move(orderByStmt);
}
