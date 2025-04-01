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
// Created by Wangyunlai on 2022/07/05.
//

#include <cmath>
#include "sql/expr/expression.h"
#include "sql/expr/tuple.h"
#include "sql/expr/arithmetic_operator.hpp"

#include "sql/stmt/select_stmt.h"
#include "sql/operator/logical_operator.h"
#include "sql/operator/physical_operator.h"

using namespace std;

RC FieldExpr::get_value(const Tuple &tuple, Value &value) const
{
  return tuple.find_cell(TupleCellSpec(table_name(), field_name()), value); // 表名，列名
}

bool FieldExpr::equal(const Expression &other) const
{
  if (this == &other) {
    return true;
  }
  if (other.type() != ExprType::FIELD) {
    return false;
  }
  const auto &other_field_expr = static_cast<const FieldExpr &>(other);
  return table_name() == other_field_expr.table_name() && field_name() == other_field_expr.field_name();
}

// TODO: 在进行表达式计算时，`chunk` 包含了所有列，因此可以通过 `field_id` 获取到对应列。
// 后续可以优化成在 `FieldExpr` 中存储 `chunk` 中某列的位置信息。
RC FieldExpr::get_column(Chunk &chunk, Column &column)
{
  if (pos_ != -1) {
    column.reference(chunk.column(pos_));
  } else {
    column.reference(chunk.column(field().meta()->field_id()));
  }
  return RC::SUCCESS;
}

bool ValueExpr::equal(const Expression &other) const
{
  if (this == &other) {
    return true;
  }
  if (other.type() != ExprType::VALUE) {
    return false;
  }
  const auto &other_value_expr = static_cast<const ValueExpr &>(other);
  return value_.compare(other_value_expr.get_value()) == 0;
}

RC ValueExpr::get_value(const Tuple &tuple, Value &value) const
{
  value = value_;
  return RC::SUCCESS;
}

RC ValueExpr::get_column(Chunk &chunk, Column &column)
{
  column.init(value_);
  return RC::SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////
CastExpr::CastExpr(unique_ptr<Expression> child, AttrType cast_type) : child_(std::move(child)), cast_type_(cast_type)
{}

CastExpr::~CastExpr() {}

RC CastExpr::cast(const Value &value, Value &cast_value) const
{
  RC rc = RC::SUCCESS;
  if (this->value_type() == value.attr_type()) {
    cast_value = value;
    return rc;
  }
  rc = Value::cast_to(value, cast_type_, cast_value);
  return rc;
}

RC CastExpr::get_value(const Tuple &tuple, Value &result) const
{
  Value value;
  RC rc = child_->get_value(tuple, value);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  return cast(value, result);
}

RC CastExpr::try_get_value(Value &result) const
{
  Value value;
  RC rc = child_->try_get_value(value);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  return cast(value, result);
}

////////////////////////////////////////////////////////////////////////////////

ComparisonExpr::ComparisonExpr(CompOp comp, unique_ptr<Expression> left, unique_ptr<Expression> right)
    : comp_(comp), left_(std::move(left)), right_(std::move(right))
{}

ComparisonExpr::~ComparisonExpr() {}

// 判断是否匹配
bool isMatch(const std::string& leftStr, const std::string& rightStr) {
	int m = leftStr.length();
	int n = rightStr.length();

	// 创建一个二维数组 dp 用于存储子问题的解, dp[i][j] 是前left前i个，right前j个是否匹配
	std::vector<std::vector<bool>> dp(m + 1, std::vector<bool>(n + 1, false));

	// 空字符串可以匹配空字符串
	dp[0][0] = true;

	// 处理 rightStr 以 '%' 开头的情况
	for (int j = 1; j <= n; ++j) {
		if (rightStr[j - 1] == '%') {
			dp[0][j] = dp[0][j - 1]; // 空字符串， 和 "%" 匹配,  "%" 匹配0个，则结果看 [0] --  [j-1]
		}
	}

	// 填充 dp 数组
	for (int i = 1; i <= m; ++i) {
		for (int j = 1; j <= n; ++j) {
			if (rightStr[j - 1] == '%') {
				// '%' 可以匹配零个dp[i][j-1]  或dp[i-1][j] 多个字符
				dp[i][j] = dp[i][j - 1] || dp[i - 1][j];
			} else if (rightStr[j - 1] == '_' || leftStr[i - 1] == rightStr[j - 1]) {
				// '_' 匹配一个任意字符，或者字符相等;  则忽略相等的元素 dp[i-1][j-1]
				dp[i][j] = dp[i - 1][j - 1];
			}
		}
	}

	return dp[m][n];
}

RC ComparisonExpr::compare_value(const Value &left, const Value &right, bool &result) const
{
  RC  rc         = RC::SUCCESS;
	// 相等于 null 类型， 直接返回false， case :  select * from exp_table where 7/0 > 2;  filter 永远返回false
	if (left.attr_type() == AttrType::UNDEFINED || right.attr_type() == AttrType::UNDEFINED) {
		result =  false;
		return rc;
	}

  if (left.attr_type() == AttrType::NULLTYPE || right.attr_type() == AttrType::NULLTYPE) {
    result =  false;
    return rc;
  }


  int cmp_result = left.compare(right);
  result         = false;

  if ((comp_ == LIKE_OP || comp_ == NOT_LIKE_OP) && left.attr_type() == AttrType::CHARS && right.attr_type() == AttrType::CHARS) {
    if (isMatch(left.get_string(), right.get_string())) {
      if (comp_ == LIKE_OP) {
		    result = true;
      } else {
		    result = false;
      }
    } else {
      if (comp_ == LIKE_OP) {
		    result = false;
      } else {
		    result = true;
      }
    }
  } else {
	  switch (comp_) {
		  case EQUAL_TO: {
			  result = (0 == cmp_result);
		  } break;
		  case LESS_EQUAL: {
			  result = (cmp_result <= 0);
		  } break;
		  case NOT_EQUAL: {
			  result = (cmp_result != 0);
		  } break;
		  case LESS_THAN: {
			  result = (cmp_result < 0);
		  } break;
		  case GREAT_EQUAL: {
			  result = (cmp_result >= 0);
		  } break;
		  case GREAT_THAN: {
			  result = (cmp_result > 0);
		  } break;
		  case IN_OP: {
        result = (0 == cmp_result);
		  } break;
      case NOT_IN_OP: {
        result = (0 == cmp_result);
      } break;
		  default: {
			  LOG_WARN("unsupported comparison. %d", comp_);
			  rc = RC::INTERNAL;
		  } break;
	  }
  }



  return rc;
}

RC ComparisonExpr::try_get_value(Value &cell) const
{
  if (left_->type() == ExprType::VALUE && right_->type() == ExprType::VALUE) {
    ValueExpr *  left_value_expr  = static_cast<ValueExpr *>(left_.get());
    ValueExpr *  right_value_expr = static_cast<ValueExpr *>(right_.get());
    const Value &left_cell        = left_value_expr->get_value();
    const Value &right_cell       = right_value_expr->get_value();

    bool value = false;
    RC   rc    = compare_value(left_cell, right_cell, value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to compare tuple cells. rc=%s", strrc(rc));
    } else {
      cell.set_boolean(value);
    }
    return rc;
  }

  return RC::INVALID_ARGUMENT;
}

RC ComparisonExpr::get_value(const Tuple &tuple, Value &value) const
{
  Value left_value;
  Value right_value;
  bool bool_value = false;

//  const JoinedTuple * jtuple = dynamic_cast<const JoinedTuple *>(&tuple);
//  if (comp_ == IN_OP || comp_ == NOT_IN_OP) {
//    if (comp_ == IN_OP) {
//      if (jtuple->get_left() != nullptr && jtuple->get_right() != nullptr) {
//        // left 在 right 子查询中
//        bool_value = true;
//      } else {
//        // left 不在 right 子查询中
//        bool_value = false;
//      }
//    } else if (comp_ == NOT_IN_OP) {
//      if (jtuple->get_left() != nullptr && jtuple->get_right() != nullptr) {
//        // left 在 right 子查询中
//        bool_value = false;
//
//      } else {
//        // left 不在 right 子查询中
//        bool_value = true;
//      }
//    }
//    value.set_boolean(bool_value);
//    return RC::SUCCESS;
//  }
  // 1 left 是子查询 ，且 right 也是子查询
  if (left_->type() == ExprType::SUBSELECT && right_->type() == ExprType::SUBSELECT) {
     // left, right 都为1行数据
    Value tem;
    RC rc = left_->get_value(tuple, left_value);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    rc = left_->get_value(tuple, tem);
    if (rc != RC::RECORD_EOF) {
      LOG_DEBUG("left is subquery , right is subquery, left return above 1 row");
      return RC::INTERNAL;
    }

    rc = right_->get_value(tuple, right_value);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    rc = right_->get_value(tuple, tem);
    if (rc != RC::RECORD_EOF) {
      LOG_DEBUG("left is subquery , right is subquery, right return above 1 row");
      return RC::INTERNAL;
    }
    // 比较计算
    rc = compare_value(left_value, right_value, bool_value);
    if (rc == RC::SUCCESS) {
      value.set_boolean(bool_value);
    }
    return rc;
  } else if (left_->type() == ExprType::SUBSELECT || right_->type() == ExprType::SUBSELECT) {
    // 2 left 是子查询， 或 right 是子查询
    // 如果是 exist, not exist, in , not in; 则子查询可以有多行
    // 如果不是 exist, not exist, in , not in; 则子查询只能有一行
    Expression *sub_query = nullptr;
    Expression *other = nullptr;
    Value *sub_query_value;
    Value *other_value;
    if (left_->type() == ExprType::SUBSELECT) {
      sub_query = left_.get();
      other = right_.get();
      sub_query_value = &left_value;
      other_value = &right_value;
    } else {
      sub_query = right_.get();
      other = left_.get();

      sub_query_value = &right_value;
      other_value = &left_value;
    }

    if (comp_ == IN_OP || comp_ == NOT_IN_OP || comp_ == EXISTS_OP || comp_ == NOT_EXISTS_OP) {
      // subquery 可以有多行
      // 扫描，直到一个成功，返回true； 否则，返回 false
      RC rc = other->get_value(tuple, *other_value);
      if (rc != RC::SUCCESS) {
        return rc;
      }
      while ((rc = sub_query->get_value(tuple, *sub_query_value)) == RC::SUCCESS) {
        // 如果成功，则已知获取数据
        // 成功获取数据
        if (comp_ == EXISTS_OP) {
          // 结束了，没有发现，则false
          // 有一个，则true
          value.set_boolean(true);
          return rc;
        } else if (comp_ == NOT_EXISTS_OP) {
          // 结束了，没有发现则true
          // 有一个，则false
          value.set_boolean(false);
          return rc;
        } else if (comp_ == IN_OP){
          // 比较计算
          rc = compare_value(left_value, right_value, bool_value);
          if (rc != RC::SUCCESS) {
            return rc;
          }
          // 扫描到一个成功的; 即返回true; 结束后为匹配则返回 false 子查询可能要扫描多次; 所以 EOF 后要重新打开
          // 3   [1,2,3,4,5], 扫描到3 的时候，返回true；    3.5， 扫描到尾部，返回false；
          if (bool_value) {
            value.set_boolean(true);
            return rc;
          }
        } else {
          // NOT IN OP
          // 比较计算
          rc = compare_value(left_value, right_value, bool_value);
          if (rc != RC::SUCCESS) {
            return rc;
          }
          // 扫描到一个成功的; 即返回true;  子查询可能要扫描多次; 所以 EOF 后要重新打开
          // 不在集合中；
          // 3   [1,2,3,4,5], 扫描到3 的时候，返回 false；    3.5， 扫描到尾部，返回 true；
          if (bool_value) {  // 返回ture, 是不等, 所有都不等，才会true;   返回 false， 是相等, 有一个相等, 即符合 not in false；
            value.set_boolean(false);
            return rc;
          }
        }
      }
      if (rc != RC::RECORD_EOF) {
        return rc;
      }

      // 扫描结束了，还没有一个成功的，则失败
      if(comp_ == EXISTS_OP) {
        value.set_boolean(false);
      } else if (comp_ == NOT_EXISTS_OP) {
        value.set_boolean(true);
      } else if (comp_ == IN_OP) {
        value.set_boolean(false);
      } else {
        // NOT IN OP
        value.set_boolean(true);
      }
      return RC::SUCCESS;  // 这里返回成功，即； leftval  ---- rightsubquery[list] , right 匹配结束，没有找到匹配，则设置为false；但返回成功。继续left.next 匹配下一行

    } else {
      // subquery 只能有一行
      Value tem;
      RC rc = sub_query->get_value(tuple, *sub_query_value);
      if (rc != RC::SUCCESS) {
        return rc;
      }
      rc = sub_query->get_value(tuple, tem);
      if (rc != RC::RECORD_EOF) {
        LOG_DEBUG("left is subquery , right is subquery, left return above 1 row");
        return RC::INTERNAL;
      }

      rc = other->get_value(tuple, *other_value);
      if (rc != RC::SUCCESS) {
        return rc;
      }

      // 比较计算
      rc = compare_value(left_value, right_value, bool_value);
      if (rc == RC::SUCCESS) {
        value.set_boolean(bool_value);
      }
      return rc;
    }
  } else {
    // 3 left, right 都不是子查询
    // 原有逻辑
    // 获取右边的值
    RC rc = left_->get_value(tuple, left_value); // 左边是表达式
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
      return rc;
    }
    // 获取右边的值
    rc = right_->get_value(tuple, right_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
      return rc;
    }
    // 比较计算
    rc = compare_value(left_value, right_value, bool_value);
    if (rc == RC::SUCCESS) {
      value.set_boolean(bool_value);
    }
    return rc;
  }
}

RC ComparisonExpr::eval(Chunk &chunk, std::vector<uint8_t> &select)
{
  RC     rc = RC::SUCCESS;
  Column left_column;
  Column right_column;

  // 获取左边列
  rc = left_->get_column(chunk, left_column);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }
  // 获取右边列
  rc = right_->get_column(chunk, right_column);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
    return rc;
  }
  // 类型校验
  if (left_column.attr_type() != right_column.attr_type()) {
    LOG_WARN("cannot compare columns with different types");
    return RC::INTERNAL;
  }
  // 列比较
  if (left_column.attr_type() == AttrType::INTS) {
    rc = compare_column<int>(left_column, right_column, select);
  } else if (left_column.attr_type() == AttrType::FLOATS) {
    rc = compare_column<float>(left_column, right_column, select);
  } else {
    // TODO: support string compare
    LOG_WARN("unsupported data type %d", left_column.attr_type());
    return RC::INTERNAL;
  }
  return rc;
}

template <typename T>
RC ComparisonExpr::compare_column(const Column &left, const Column &right, std::vector<uint8_t> &result) const
{
  RC rc = RC::SUCCESS;

  bool left_const  = left.column_type() == Column::Type::CONSTANT_COLUMN;
  bool right_const = right.column_type() == Column::Type::CONSTANT_COLUMN;
  if (left_const && right_const) {
    compare_result<T, true, true>((T *)left.data(), (T *)right.data(), left.count(), result, comp_);
  } else if (left_const && !right_const) {
    compare_result<T, true, false>((T *)left.data(), (T *)right.data(), right.count(), result, comp_);
  } else if (!left_const && right_const) {
    compare_result<T, false, true>((T *)left.data(), (T *)right.data(), left.count(), result, comp_);
  } else {
    compare_result<T, false, false>((T *)left.data(), (T *)right.data(), left.count(), result, comp_);
  }
  return rc;
}

////////////////////////////////////////////////////////////////////////////////
ConjunctionExpr::ConjunctionExpr(Type type, vector<unique_ptr<Expression>> &children)
    : conjunction_type_(type), children_(std::move(children))
{}

RC ConjunctionExpr::get_value(const Tuple &tuple, Value &value) const
{
  RC rc = RC::SUCCESS;
  if (children_.empty()) {
    // 儿子是空的，说明filter 为空；则返回 true
    value.set_boolean(true);
    return rc;
  }

  Value tmp_value;
  for (const unique_ptr<Expression> &expr : children_) {
    rc = expr->get_value(tuple, tmp_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value by child expression. rc=%s", strrc(rc));
      return rc;
    }
    // 每个儿子获取一个 bool 值
    bool bool_value = tmp_value.get_boolean();
    // true and true -> true;  true and false -> false; false and false -> false;
    // true or true -> ture;   true or false -> true;  false or false -> false;
    if ((conjunction_type_ == Type::AND && !bool_value) || (conjunction_type_ == Type::OR && bool_value)) {
      // and 一个false ，则返回false
      // or 一个 true， 则返回true
      value.set_boolean(bool_value);
      return rc;
    }
  }
  // 都是 and true, 则true； 都是 or false， 则false
  bool default_value = (conjunction_type_ == Type::AND);
  value.set_boolean(default_value);
  return rc;
}

////////////////////////////////////////////////////////////////////////////////
// 当在构造函数的初始化列表里把 Expression *left 赋值给 std::unique_ptr<Expression> 成员变量时，
// 实际上是调用了 std::unique_ptr 的这个接收原始指针的构造函数
ArithmeticExpr::ArithmeticExpr(ArithmeticExpr::Type type, Expression *left, Expression *right)
    : arithmetic_type_(type), left_(left), right_(right)
{}
ArithmeticExpr::ArithmeticExpr(ArithmeticExpr::Type type, unique_ptr<Expression> left, unique_ptr<Expression> right)
    : arithmetic_type_(type), left_(std::move(left)), right_(std::move(right))
{}
// 是否相等，类型都是算术运算，left 表达式相等， 和 right 表达式也相等
bool ArithmeticExpr::equal(const Expression &other) const
{
  if (this == &other) {
    return true;
  }
  if (type() != other.type()) {
    return false;
  }
  auto &other_arith_expr = static_cast<const ArithmeticExpr &>(other);
  return arithmetic_type_ == other_arith_expr.arithmetic_type() && left_->equal(*other_arith_expr.left_) &&
         right_->equal(*other_arith_expr.right_);
}
// 左边是整数，右边是整数，算术负号不是除法，则返回值为整数； 其他情况为浮点数
AttrType ArithmeticExpr::value_type() const
{
  if (!right_) {
    return left_->value_type();
  }

  if (left_->value_type() == AttrType::INTS && right_->value_type() == AttrType::INTS &&
      arithmetic_type_ != Type::DIV) {
    return AttrType::INTS;
  }

  if (left_->value_type() == AttrType::VECTORS || right_->value_type() == AttrType::VECTORS) {
    return AttrType::VECTORS;
  }

  return AttrType::FLOATS;
}
// left ， right 得到结果值 value
RC ArithmeticExpr::calc_value(const Value &left_value, const Value &right_value, Value &value) const
{
  RC rc = RC::SUCCESS;
  // 单目，只判断左边，如果是 null， 则返回null
  if (arithmetic_type_ == Type::NEGATIVE) {
	  if (left_value.attr_type() == AttrType::UNDEFINED) {
		  value.reset();
	    return rc;
	  }
  } else {
    // 双目， 判断左右，如果有null， 则返回null
	  if (left_value.attr_type() == AttrType::UNDEFINED || right_value .attr_type() == AttrType::UNDEFINED) {
		  value.reset();
      return rc;
	  }
  }

  if (arithmetic_type_ == Type::NEGATIVE) {
    if (left_value.attr_type() == AttrType::NULLTYPE) {
      value.reset();
      return rc;
    }
  } else {
    // 双目， 判断左右，如果有null， 则返回null
    if (left_value.attr_type() == AttrType::NULLTYPE || right_value .attr_type() == AttrType::NULLTYPE) {
      value.reset();
      return rc;
    }
  }

  const AttrType target_type = value_type();
  value.set_type(target_type);
  // 加减乘除，负号，操作
  switch (arithmetic_type_) {
    case Type::ADD: {
      Value::add(left_value, right_value, value);
    } break;

    case Type::SUB: {
      Value::subtract(left_value, right_value, value);
    } break;

    case Type::MUL: {
      Value::multiply(left_value, right_value, value);
    } break;

    case Type::DIV: {
      Value::divide(left_value, right_value, value);
    } break;

    case Type::NEGATIVE: {
      Value::negative(left_value, value);
    } break;

    default: {
      rc = RC::INTERNAL;
      LOG_WARN("unsupported arithmetic type. %d", arithmetic_type_);
    } break;
  }
  return rc;
}

template <bool LEFT_CONSTANT, bool RIGHT_CONSTANT>
RC ArithmeticExpr::execute_calc(
    const Column &left, const Column &right, Column &result, Type type, AttrType attr_type) const
{
  RC rc = RC::SUCCESS;
  switch (type) {
    case Type::ADD: {
      // 加，目标是 int 类型
      if (attr_type == AttrType::INTS) {
        // 函数调用，右4个模版类型参数
        // 左边数据，右边数据，结果数据，结果容量
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, int, AddOperator>(
            (int *)left.data(), (int *)right.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, float, AddOperator>(
            (float *)left.data(), (float *)right.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
    } break;
    case Type::SUB:
      if (attr_type == AttrType::INTS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, int, SubtractOperator>(
            (int *)left.data(), (int *)right.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, float, SubtractOperator>(
            (float *)left.data(), (float *)right.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
      break;
    case Type::MUL:
      if (attr_type == AttrType::INTS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, int, MultiplyOperator>(
            (int *)left.data(), (int *)right.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, float, MultiplyOperator>(
            (float *)left.data(), (float *)right.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
      break;
    case Type::DIV:
      if (attr_type == AttrType::INTS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, int, DivideOperator>(
            (int *)left.data(), (int *)right.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        binary_operator<LEFT_CONSTANT, RIGHT_CONSTANT, float, DivideOperator>(
            (float *)left.data(), (float *)right.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
      break;
    case Type::NEGATIVE:
      if (attr_type == AttrType::INTS) {
        unary_operator<LEFT_CONSTANT, int, NegateOperator>((int *)left.data(), (int *)result.data(), result.capacity());
      } else if (attr_type == AttrType::FLOATS) {
        unary_operator<LEFT_CONSTANT, float, NegateOperator>(
            (float *)left.data(), (float *)result.data(), result.capacity());
      } else {
        rc = RC::UNIMPLEMENTED;
      }
      break;
    default: rc = RC::UNIMPLEMENTED; break;
  }
  if (rc == RC::SUCCESS) {
    result.set_count(result.capacity());
  }
  return rc;
}
// 获取左值， 获取右值，计算左右值
RC ArithmeticExpr::get_value(const Tuple &tuple, Value &value) const
{
  RC rc = RC::SUCCESS;

  Value left_value;
  Value right_value;

  rc = left_->get_value(tuple, left_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }
  // 如果不是单目运算
  if (arithmetic_type_ != Type::NEGATIVE) {
	  rc = right_->get_value(tuple, right_value);
	  if (rc != RC::SUCCESS) {
		  LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
		  return rc;
	  }
  }

  return calc_value(left_value, right_value, value);
}

RC ArithmeticExpr::get_column(Chunk &chunk, Column &column)
{
  RC rc = RC::SUCCESS;
  if (pos_ != -1) {
    column.reference(chunk.column(pos_));
    return rc;
  }
  Column left_column;
  Column right_column;
  // 获取左边列
  rc = left_->get_column(chunk, left_column);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get column of left expression. rc=%s", strrc(rc));
    return rc;
  }
  // 获取右边列
  rc = right_->get_column(chunk, right_column);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get column of right expression. rc=%s", strrc(rc));
    return rc;
  }
  return calc_column(left_column, right_column, column);
}

RC ArithmeticExpr::calc_column(const Column &left_column, const Column &right_column, Column &column) const
{
  RC rc = RC::SUCCESS;
  // 返回值类型
  const AttrType target_type = value_type();
  // 列初始化， 值类型，列的宽度， 列的行数
  column.init(target_type, left_column.attr_len(), std::max(left_column.count(), right_column.count()));
  bool left_const  = left_column.column_type() == Column::Type::CONSTANT_COLUMN;
  bool right_const = right_column.column_type() == Column::Type::CONSTANT_COLUMN;
  if (left_const && right_const) {
    // 左， 右都是 CONSTANT
    column.set_column_type(Column::Type::CONSTANT_COLUMN);
    rc = execute_calc<true, true>(left_column, right_column, column, arithmetic_type_, target_type);
  } else if (left_const && !right_const) {
    // 左是 CONSTANT， 右不是
    column.set_column_type(Column::Type::NORMAL_COLUMN);
    rc = execute_calc<true, false>(left_column, right_column, column, arithmetic_type_, target_type);
  } else if (!left_const && right_const) {
    // 左不是， 右是 CONSTANT
    column.set_column_type(Column::Type::NORMAL_COLUMN);
    rc = execute_calc<false, true>(left_column, right_column, column, arithmetic_type_, target_type);
  } else {
    // 左不是，右不是
    column.set_column_type(Column::Type::NORMAL_COLUMN);
    rc = execute_calc<false, false>(left_column, right_column, column, arithmetic_type_, target_type);
  }
  return rc;
}

RC ArithmeticExpr::try_get_value(Value &value) const
{
  RC rc = RC::SUCCESS;

  Value left_value;
  Value right_value;

  rc = left_->try_get_value(left_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }

  if (right_) {
    rc = right_->try_get_value(right_value);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to get value of right expression. rc=%s", strrc(rc));
      return rc;
    }
  }

  return calc_value(left_value, right_value, value);
}

////////////////////////////////////////////////////////////////////////////////

UnboundAggregateExpr::UnboundAggregateExpr(const char *aggregate_name, Expression *child)
    : aggregate_name_(aggregate_name), child_(child)
{}

////////////////////////////////////////////////////////////////////////////////
AggregateExpr::AggregateExpr(Type type, Expression *child) : aggregate_type_(type), child_(child) {}

AggregateExpr::AggregateExpr(Type type, unique_ptr<Expression> child) : aggregate_type_(type), child_(std::move(child))
{}

RC AggregateExpr::get_column(Chunk &chunk, Column &column)
{
  RC rc = RC::SUCCESS;
  if (pos_ != -1) {
    column.reference(chunk.column(pos_));
  } else {
    rc = RC::INTERNAL;
  }
  return rc;
}

bool AggregateExpr::equal(const Expression &other) const
{
  if (this == &other) {
    return true;
  }
  if (other.type() != type()) {
    return false;
  }
  const AggregateExpr &other_aggr_expr = static_cast<const AggregateExpr &>(other);
  return aggregate_type_ == other_aggr_expr.aggregate_type() && child_->equal(*other_aggr_expr.child());
}

//AVG,
//MAX,
//MIN,
// agg 表达式类，创建agg 算子
unique_ptr<Aggregator> AggregateExpr::create_aggregator() const
{
  unique_ptr<Aggregator> aggregator;
  switch (aggregate_type_) {
    case Type::SUM: {
      aggregator = make_unique<SumAggregator>();
      break;
    }
    case Type::COUNT: {
	    aggregator = make_unique<CountAggregator>();
	    break;
    }
	  case Type::AVG: {
		  aggregator = make_unique<AVGAggregator>();
		  break;
	  }
	  case Type::MAX: {
		  aggregator = make_unique<MAXAggregator>();
		  break;
	  }
	  case Type::MIN: {
		  aggregator = make_unique<MINAggregator>();
		  break;
	  }
    default: {
      ASSERT(false, "unsupported aggregate type");
      break;
    }
  }
  return aggregator;
}

RC AggregateExpr::get_value(const Tuple &tuple, Value &value) const
{
  return tuple.find_cell(TupleCellSpec(name()), value);
}

RC AggregateExpr::type_from_string(const char *type_str, AggregateExpr::Type &type)
{
  RC rc = RC::SUCCESS;
  if (0 == strcasecmp(type_str, "count")) {
    type = Type::COUNT;
  } else if (0 == strcasecmp(type_str, "sum")) {
    type = Type::SUM;
  } else if (0 == strcasecmp(type_str, "avg")) {
    type = Type::AVG;
  } else if (0 == strcasecmp(type_str, "max")) {
    type = Type::MAX;
  } else if (0 == strcasecmp(type_str, "min")) {
    type = Type::MIN;
  } else {
    rc = RC::INVALID_ARGUMENT;
  }
  return rc;
}

RC VectorFunctionExpr::get_value(const Tuple &tuple, Value &value) const {
  RC rc = RC::SUCCESS;

  Value left_value;
  Value right_value;
  // left 值
  rc = left_->get_value(tuple, left_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }
  // right 值
  rc = right_->get_value(tuple, right_value);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to get value of left expression. rc=%s", strrc(rc));
    return rc;
  }
  // 计算
  return calc_value(left_value, right_value, value);
}
// 它向编译器和其他开发者表明该函数不会修改调用它的对象的任何非 mutable 数据成员
RC VectorFunctionExpr::calc_value(const Value &left_value, const Value &right_value, Value &value) const {
  RC rc = RC::SUCCESS;

  if (left_value.attr_type() == AttrType::UNDEFINED || right_value .attr_type() == AttrType::UNDEFINED) {
    value.reset();
    return rc;
  }

  const AttrType target_type = value_type();
  value.set_type(target_type);
  float result;
  switch (type_) {
    case VectorFunctionType::L2_DISTANCE: {
      // 向量函数距离表达式 欧几里得距离
      result = l2_distance(left_value.get_vector(), right_value.get_vector());
      value.set_float(result);
    } break;

    case VectorFunctionType::COSINE_DISTANCE: {
      // 向量函数距离表达式 余弦距离
      result = cosine_distance(left_value.get_vector(), right_value.get_vector());
      value.set_float(result);
    } break;

    case VectorFunctionType::INNER_PRODUCT: {
      // 向量函数距离表达式  内积
      result = inner_product(left_value.get_vector(), right_value.get_vector());
      value.set_float(result);
    } break;

    default: {
      rc = RC::INTERNAL;
      LOG_WARN("unsupported VectorFunctionExpr type. %d", type_);
    } break;
  }
  return rc;
}

//l2_distance 函数：
//遍历向量 A 和 B 的每个元素，计算对应元素差值的平方并累加。
//最后对累加和取平方根，得到欧氏距离。
float VectorFunctionExpr::l2_distance(const vector<float> &A, const vector<float> &B) const {
  float sum = 0.0;
  for (size_t i = 0; i < A.size(); ++i) {
    sum += std::pow(A[i] - B[i], 2);
  }
  return std::sqrt(sum);
}

//cosine_distance 函数：
//先计算向量 A 和 B 的点积、A 的模和 B 的模。
//再根据余弦距离公式计算并返回结果。
float VectorFunctionExpr::cosine_distance(const vector<float> &A, const vector<float> &B) const {
  float dot_product = 0.0;
  float norm_A = 0.0;
  float norm_B = 0.0;
  for (size_t i = 0; i < A.size(); ++i) {
    dot_product += A[i] * B[i];
    norm_A += std::pow(A[i], 2);
    norm_B += std::pow(B[i], 2);
  }
  norm_A = std::sqrt(norm_A);
  norm_B = std::sqrt(norm_B);
  return 1 - dot_product / (norm_A * norm_B);
}

//inner_product 函数：
//遍历向量 A 和 B 的每个元素，计算对应元素的乘积并累加，得到内积。
float VectorFunctionExpr::inner_product(const vector<float> &A, const vector<float> &B) const {
  float result = 0.0;
  for (size_t i = 0; i < A.size(); ++i) {
    result += A[i] * B[i];
  }
  return result;
}

RC SubqueryExpr::get_value(const Tuple &tuple, Value &value) const {
  RC rc = RC::SUCCESS;
//  const JoinedTuple * jtuple = dynamic_cast<const JoinedTuple *>(&tuple); // 假定，子查询都有join算子
//  Tuple *tup = jtuple->get_right();
//  int cell_num = tup->cell_num();
//  for (int i = 0; i < cell_num; i++) {
//    rc = tup->cell_at(i, value);
//    if (rc != RC::SUCCESS) {
//      LOG_WARN("failed to get tuple cell value. rc=%s", strrc(rc));
//      return rc;
//    }
//    // 读取的每个值，str
//    string cell_str = value.to_string();
//    break;
//  }

//  while (RC::SUCCESS == (rc = oper->next())) {
//    Tuple *tuple = oper->current_tuple();
//    if (nullptr == tuple) {
//      rc = RC::INTERNAL;
//      LOG_WARN("failed to get tuple from operator");
//      break;
//    }
  // 子查询获取值， 即从物理计划中获取值
  // 如果没有打开，则打开，如果失败，直接返回
  if(!isOpen()) {
    rc = open_sub_query();
    if (rc != RC::SUCCESS) {
      return rc;
    }
  }

//  const unique_ptr<PhysicalOperator> &sub_query_physical_plan = getSubQueryPhysicalPlan();
  rc = sub_query_physical_plan_->next();
  // 如果不成功，则返回 rc
  if (rc != RC::SUCCESS) {
    if (rc == RC::RECORD_EOF) {
      RC rct = close_sub_query(); // 未打开状态
      if (rct != RC::SUCCESS) {
        return rct;
      }
    }
    return rc;
  }

  Tuple *sub_tuple = sub_query_physical_plan_->current_tuple();
  if (nullptr == sub_tuple) {
    rc = RC::INTERNAL;
    RC rct = close_sub_query(); // 未打开状态
    if (rct != RC::SUCCESS) {
      return rct;
    }
    LOG_WARN("failed to get tuple from operator");
    return rc;
  }

  if (sub_tuple->cell_num() > 1) {
    rc = RC::INTERNAL;
    RC rct = close_sub_query(); // 未打开状态
    if (rct != RC::SUCCESS) {
      return rct;
    }
    LOG_WARN("subquery cell must do not above 1");
    return rc;
  }
  // 返回 value
  sub_tuple->cell_at(0, value);
  return rc;
}

AttrType SubqueryExpr::value_type() const {  // value type， query 的type
  return attrType_;
//
//  return AttrType::VECTORS;
}

void SubqueryExpr::setExpSelect(SelectStmt *expSelect) {
  exp_select_ = expSelect;
  attrType_ = expSelect->query_expressions()[0]->value_type();
  table_name_ = expSelect->tables().front()->name();
  filed_name_ = expSelect->query_expressions().front()->name();
}

SelectStmt *SubqueryExpr::getExpSelect() const {
  return exp_select_;
}

const string &SubqueryExpr::getTableName() const {
  return table_name_;
}

void SubqueryExpr::setTableName(const string &tableName) {
  table_name_ = tableName;
}

const string &SubqueryExpr::getFiledName() const {
  return filed_name_;
}

void SubqueryExpr::setFiledName(const string &filedName) {
  filed_name_ = filedName;
}

 unique_ptr<LogicalOperator> &SubqueryExpr::getSubQueryLogicPlan()  {
  return sub_query_logic_plan_;
}

void SubqueryExpr::setSubQueryLogicPlan( unique_ptr<LogicalOperator> &subQueryLogicPlan) {
  sub_query_logic_plan_ = std::move(subQueryLogicPlan);
}

SubqueryExpr::SubqueryExpr(ParsedSqlNode *subSel) : subSel_(subSel){
  is_open_ = false;
  trx_ = nullptr;
}

const unique_ptr<PhysicalOperator> & SubqueryExpr::getSubQueryPhysicalPlan() const {
  return sub_query_physical_plan_;
}

void SubqueryExpr::setSubQueryPhysicalPlan(unique_ptr<PhysicalOperator> &subQueryPhysicalPlan) {
  sub_query_physical_plan_ = std::move(subQueryPhysicalPlan);
}

bool SubqueryExpr::isOpen() const {
  return is_open_;
}

void SubqueryExpr::setIsOpen(bool isOpen) const {
  is_open_ = isOpen;
}

RC SubqueryExpr::open_sub_query() const {
  RC rc = RC::SUCCESS;
  rc = sub_query_physical_plan_->open(trx_);
  if (rc == RC::SUCCESS) {
    is_open_ = true;
  }

  return rc;
}

RC SubqueryExpr::close_sub_query() const {
  RC rc = RC::SUCCESS;
  rc = sub_query_physical_plan_->close();
  if (rc == RC::SUCCESS) {
    is_open_ = false;
  }
  return rc;
}
// 没有帮我们 close subquery 的物理计划；只有在析构的时候，判断物理计划是否打开，如果打开，则关闭物理计划
SubqueryExpr::~SubqueryExpr() {
  if (is_open_) {
    close_sub_query();
  }
}

