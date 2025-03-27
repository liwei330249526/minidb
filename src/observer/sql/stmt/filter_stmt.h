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
  std::vector<Value> values;
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

  void init_values(const std::vector<Value> &values)
  {
    is_attr     = 3;  // 值列表
    this->values= values;
  }

  void init_expression(unique_ptr<Expression> left_expression)
  {
    is_attr     = 2;
    this->expression = std::move(left_expression);
  }

  FilterObj() = default;
		// 移动赋值运算符, liwei： 移动赋值运算符用于把一个右值对象的资源所有权转移到一个已存在的对象上
	FilterObj& operator=(FilterObj&& other) noexcept {
		if (this != &other) {
			is_attr = other.is_attr;
			field = std::move(other.field);
			value = std::move(other.value);
			expression = std::move(other.expression);
      values = std::move(other.values);
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
  // liwei 它可以将左值强制转换为右值引用，从而调用移动构造函数或移动赋值运算符，实现资源的高效转移。当你调用 filter_unit->set_right(filter_obj); 时，filter_obj 的资源（如动态分配的内存、文件句柄等）会被转移到 filter_unit 的 right_ 成员变量中。
  // 在 filter_obj 的资源转移到 right_ 之后，filter_obj 就处于有效但未指定的状态。当 filter_obj 被析构时，它已经不再拥有之前的资源，因此析构操作不会影响 right_ 的值。right_ 现在拥有这些资源，并且会在 filter_unit 对象被析构时进行相应的资源释放操作。
  // 移动赋值运算符
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
