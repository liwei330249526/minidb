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

#include "sql/expr/expression.h"
#include "sql/parser/parse_defs.h"
#include "sql/stmt/stmt.h"
#include <unordered_map>
#include <vector>

class Db;
class Table;
class FieldMeta;
// filter 对象， 字段， 值
// 当结构体中有一个 std::unique_ptr 指针，将一个结构体变量赋值给另一个结构体变量时，默认情况下会出现编译错误，因为 std::unique_ptr 具有独占所有权的特性，
// 其拷贝构造函数和拷贝赋值运算符被显式删除，不允许直接进行拷贝操作。不过，可以通过移动语义来转移 std::unique_ptr 的所有权。
struct FilterObj
{
		// 是属性 1 ； 是值 0；或算数表达式 2
  int  is_attr;
  //  字段；
  Field field;
  //  值
  Value value;
  // 表达式
  unique_ptr<Expression> expression;  // 很可能是 ArithmeticExpr

  void init_attr(const Field &field)
  {
    is_attr     = 1;
    this->field = field;
  }

  void init_value(const Value &value)
  {
    is_attr     = 0;
    this->value = value;
  }

  void init_expression(unique_ptr<Expression> left_expression)
  {
    is_attr     = 2;
    this->expression = std::move(left_expression);
  }

  FilterObj() = default;
		// 移动赋值运算符
	FilterObj& operator=(FilterObj&& other) noexcept {
		if (this != &other) {
			is_attr = other.is_attr;
			field = std::move(other.field);
			value = std::move(other.value);
			expression = std::move(other.expression);
		}
		return *this;
	}

//	FilterObj& operator=(FilterObj& other)  {
//		if (this != &other) {
//			is_attr = other.is_attr;
//			field = std::move(other.field);
//			value = std::move(other.value);
//			expression = std::move(other.expression);
//		}
//		return *this;
//	}

};
// filter 单元
class FilterUnit
{
public:
  FilterUnit() = default;
  ~FilterUnit() {}

  void set_comp(CompOp comp) { comp_ = comp; }

  CompOp comp() const { return comp_; }

  void set_left(FilterObj &obj) { left_ = std::move(obj); }
  void set_right(FilterObj &obj) { right_ = std::move(obj); }

   FilterObj &left()  { return left_; }
   FilterObj &right()  { return right_; }

private:
		// 操作符，左边对象，右边对象
  CompOp    comp_ = NO_OP;
  FilterObj left_;
  FilterObj right_;
};

/**
 * @brief Filter/谓词/过滤语句
 * @ingroup Statement
 */
class FilterStmt
{
public:
  FilterStmt() = default;
  virtual ~FilterStmt();

public:
  const std::vector<FilterUnit *> &filter_units() const { return filter_units_; }

public:
  static RC create(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
      const ConditionSqlNode *conditions, int condition_num, FilterStmt *&stmt);

  static RC create_filter_unit(Db *db, Table *default_table, std::unordered_map<std::string, Table *> *tables,
      const ConditionSqlNode &condition, FilterUnit *&filter_unit);

private:
  std::vector<FilterUnit *> filter_units_;  // 默认当前都是AND关系
};
