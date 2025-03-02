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
// Created by Wangyunlai on 2022/5/22.
//

#pragma once

#include "common/rc.h"
#include "sql/stmt/stmt.h"
#include "filter_stmt.h"

class Table;

/**
 * @brief 更新语句
 * @ingroup Statement
 */
class UpdateStmt : public Stmt
{
public:
  UpdateStmt() = default;
  UpdateStmt(Table *table, string attribute_name, Value value, FilterStmt *filter_stmt);
  ~UpdateStmt() override;

public:
	// 静态函数
  static RC create(Db *db, const UpdateSqlNode &update_sql, Stmt *&stmt);

public:
  Table *table() const { return table_; }
  Value values() const { return value_; }
  int    value_amount() const { return value_amount_; }
	FilterStmt *filter_stmt() const { return filter_stmt_; }
	StmtType type() const override { return StmtType::UPDATE; }
	const string & getAttributeName() const;
  void setAttributeName(const string & attributeName);

private:
  Table *table_        = nullptr; // 表
	string attribute_name_; // 1 个列
  // 列名
	Value value_; // 1个列值

  int    value_amount_ = 0; // 设置值个数
  FilterStmt *filter_stmt_ = nullptr; // filter stmt
};
