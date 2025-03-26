/* Copyright (c) 2023 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai on 2023/08/16.
//

#include "sql/optimizer/logical_plan_generator.h"

#include <common/log/log.h>
#include <src/observer/sql/stmt/update_stmt.h>

#include "sql/operator/calc_logical_operator.h"
#include "sql/operator/delete_logical_operator.h"
#include "sql/operator/explain_logical_operator.h"
#include "sql/operator/insert_logical_operator.h"
#include "sql/operator/update_logical_operator.h"
#include "sql/operator/join_logical_operator.h"
#include "sql/operator/logical_operator.h"
#include "sql/operator/predicate_logical_operator.h"
#include "sql/operator/project_logical_operator.h"
#include "sql/operator/table_get_logical_operator.h"
#include "sql/operator/group_by_logical_operator.h"

#include "sql/stmt/calc_stmt.h"
#include "sql/stmt/delete_stmt.h"
#include "sql/stmt/explain_stmt.h"
#include "sql/stmt/filter_stmt.h"
#include "sql/stmt/insert_stmt.h"
#include "sql/stmt/select_stmt.h"
#include "sql/stmt/stmt.h"

#include "sql/expr/expression_iterator.h"

using namespace std;
using namespace common;

RC LogicalPlanGenerator::create(Stmt *stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  RC rc = RC::SUCCESS;
  switch (stmt->type()) {
    case StmtType::CALC: {
      CalcStmt *calc_stmt = static_cast<CalcStmt *>(stmt);

      rc = create_plan(calc_stmt, logical_operator);
    } break;

    case StmtType::SELECT: {
      SelectStmt *select_stmt = static_cast<SelectStmt *>(stmt);

      rc = create_plan(select_stmt, logical_operator);
    } break;

    case StmtType::INSERT: {
      InsertStmt *insert_stmt = static_cast<InsertStmt *>(stmt);

      rc = create_plan(insert_stmt, logical_operator);
    } break;

    case StmtType::DELETE: {
      DeleteStmt *delete_stmt = static_cast<DeleteStmt *>(stmt);

      rc = create_plan(delete_stmt, logical_operator);
    } break;

	  case StmtType::UPDATE: {
	  	// 增加逻辑计划 update
		  UpdateStmt *update_stmt = static_cast<UpdateStmt *>(stmt);

		  rc = create_plan(update_stmt, logical_operator);
	  } break;

    case StmtType::EXPLAIN: {
      ExplainStmt *explain_stmt = static_cast<ExplainStmt *>(stmt);

      rc = create_plan(explain_stmt, logical_operator);
    } break;
    default: {
      rc = RC::UNIMPLEMENTED;
    }
  }
  return rc;
}

RC LogicalPlanGenerator::create_plan(CalcStmt *calc_stmt, std::unique_ptr<LogicalOperator> &logical_operator)
{
  logical_operator.reset(new CalcLogicalOperator(std::move(calc_stmt->expressions())));
  return RC::SUCCESS;
}

RC LogicalPlanGenerator::create_plan(SelectStmt *select_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  unique_ptr<LogicalOperator> *last_oper = nullptr; // 智能指针的地址

  unique_ptr<LogicalOperator> table_oper(nullptr);
  last_oper = &table_oper;
  // 获取所有表， join 的话，有多个表; 每个表有一个 TableGet算子; no
  const std::vector<Table *> &tables = select_stmt->tables();
  for (Table *table : tables) {

    unique_ptr<LogicalOperator> table_get_oper(new TableGetLogicalOperator(table, ReadWriteMode::READ_ONLY));
    if (table_oper == nullptr) {
    	// 第一个table 作为主表, 主表是 TableGet 算子
      table_oper = std::move(table_get_oper);
    } else {
    	// 后续的表作为join 表, 构造了 join_oper
    	// 创建了 join 的逻辑计划, join 逻辑算子, join 表是一个 TableGet 算子， 共同加入一个 Join 算子
      JoinLogicalOperator *join_oper = new JoinLogicalOperator;
      join_oper->add_child(std::move(table_oper));
      join_oper->add_child(std::move(table_get_oper));
      table_oper = unique_ptr<LogicalOperator>(join_oper);
    }
  }

  // 根据子查询，构造 join
  const std::vector<FilterUnit *>    &filter_units = select_stmt->filter_stmt()->filter_units();
  for (FilterUnit *filter_unit : filter_units) {
    // 子查询
//    FilterObj &filter_obj_left  = filter_unit->left();
    FilterObj &filter_obj_right = filter_unit->right();
    Expression *right = nullptr;
    if (filter_obj_right.is_attr == 0) {
      right = new ValueExpr(filter_obj_right.value);
    } else if (filter_obj_right.is_attr == 1) {
      right = new FieldExpr(filter_obj_right.field);
    } else if (filter_obj_right.is_attr == 2) {
      right = filter_obj_right.expression.get();
    }

    if (right->type() == ExprType::SUBSELECT) {
      SubqueryExpr *sub_sql = reinterpret_cast<SubqueryExpr *>(right);
      unique_ptr<LogicalOperator> sub_oper;
      create_plan(sub_sql->getExpSelect(), sub_oper);
      JoinLogicalOperator *join_oper = new JoinLogicalOperator;
      join_oper->add_child(std::move(table_oper));
      join_oper->add_child(std::move(sub_oper));
      table_oper = unique_ptr<LogicalOperator>(join_oper);
    }
  }
  // 创建 predicate 逻辑计划
  unique_ptr<LogicalOperator> predicate_oper;

  RC rc = create_plan(select_stmt->filter_stmt(), predicate_oper);
  if (OB_FAIL(rc)) {
    LOG_WARN("failed to create predicate logical plan. rc=%s", strrc(rc));
    return rc;
  }

  if (predicate_oper) {
    if (*last_oper) {
      predicate_oper->add_child(std::move(*last_oper));
    }

    last_oper = &predicate_oper;
  }
  // 创建 group by 逻辑计划
  unique_ptr<LogicalOperator> group_by_oper;
  rc = create_group_by_plan(select_stmt, group_by_oper);
  if (OB_FAIL(rc)) {
    LOG_WARN("failed to create group by logical plan. rc=%s", strrc(rc));
    return rc;
  }

  if (group_by_oper) {
    if (*last_oper) {
      group_by_oper->add_child(std::move(*last_oper));
    }

    last_oper = &group_by_oper;
  }

  auto project_oper = make_unique<ProjectLogicalOperator>(std::move(select_stmt->query_expressions()));
  if (*last_oper) {
    project_oper->add_child(std::move(*last_oper));
  }

  logical_operator = std::move(project_oper);
  return RC::SUCCESS;
}
// 建立filter 逻辑算子
RC LogicalPlanGenerator::create_plan(FilterStmt *filter_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  RC                                  rc = RC::SUCCESS;
  std::vector<unique_ptr<Expression>> cmp_exprs;
  const std::vector<FilterUnit *>    &filter_units = filter_stmt->filter_units();
  for (FilterUnit *filter_unit : filter_units) {
     FilterObj &filter_obj_left  = filter_unit->left();
     FilterObj &filter_obj_right = filter_unit->right();

    // 左边是列，或值， 或算数表达式,
    unique_ptr<Expression> left;
	  if (filter_obj_left.is_attr == 0) {
      left = unique_ptr<Expression>(new ValueExpr(filter_obj_left.value));
	  } else if (filter_obj_left.is_attr == 1) {
		  left = unique_ptr<Expression>(new FieldExpr(filter_obj_left.field));
	  } else if (filter_obj_left.is_attr == 2) {
			left = std::move(filter_obj_left.expression) ;
	  }

	  unique_ptr<Expression> right;
	  if (filter_obj_right.is_attr == 0) {
		  right = unique_ptr<Expression>(new ValueExpr(filter_obj_right.value));
	  } else if (filter_obj_right.is_attr == 1) {
		  right = unique_ptr<Expression>(new FieldExpr(filter_obj_right.field));
	  } else if (filter_obj_right.is_attr == 2) {
		  right = std::move(filter_obj_right.expression);
	  }

//    unique_ptr<Expression> left(filter_obj_left.is_attr
//                                    ? static_cast<Expression *>(new FieldExpr(filter_obj_left.field))
//                                    : static_cast<Expression *>(new ValueExpr(filter_obj_left.value)));
//    // 右边是列，或值， 或算数表达式
//    unique_ptr<Expression> right(filter_obj_right.is_attr
//                                     ? static_cast<Expression *>(new FieldExpr(filter_obj_right.field))
//                                     : static_cast<Expression *>(new ValueExpr(filter_obj_right.value)));
    // 如果left 和right 的类型不相等，则满足需要； 即例如 age>10, 即左边是列名，右边是值
    if (left->value_type() != right->value_type()) {
    	// 计算 left 转换为 right 类型的cost； 和right 转换为 left 类型的cost
      auto left_to_right_cost = implicit_cast_cost(left->value_type(), right->value_type());
      auto right_to_left_cost = implicit_cast_cost(right->value_type(), left->value_type());
      if (left_to_right_cost <= right_to_left_cost && left_to_right_cost != INT32_MAX) {
      	// left 转换为 right 类型
        ExprType left_type = left->type();
        // 转换表达式
        auto cast_expr = make_unique<CastExpr>(std::move(left), right->value_type());
        if (left_type == ExprType::VALUE) {
        	// 左边是值， 例如 10<age,
          Value left_val;
          // 根据转换表达式获取转换后的值
          if (OB_FAIL(rc = cast_expr->try_get_value(left_val)))
          {
            LOG_WARN("failed to get value from left child", strrc(rc));
            return rc;
          }
          // 根据转换后的值构造 ValueExpr
          left = make_unique<ValueExpr>(left_val); // C++ 引入了右值引用和移动语义，使得可以高效地处理临时对象的资源转移。当使用右值进行赋值操作时，会优先调用移动赋值运算符，避免了不必要的复制操作，提高了性能。
        } else {
        	// 如果左边是列明，则直接赋值转换表达式
          left = std::move(cast_expr);  // 。通过 std::move 函数，能够将一个左值转换为右值引用，从而触发 std::unique_ptr 的移动赋值运算符。
        }
      } else if (right_to_left_cost < left_to_right_cost && right_to_left_cost != INT32_MAX) {
        ExprType right_type = right->type();
        auto cast_expr = make_unique<CastExpr>(std::move(right), left->value_type());
        if (right_type == ExprType::VALUE) {
          Value right_val;
          if (OB_FAIL(rc = cast_expr->try_get_value(right_val)))
          {
            LOG_WARN("failed to get value from right child", strrc(rc));
            return rc;
          }
          right = make_unique<ValueExpr>(right_val);
        } else {
          right = std::move(cast_expr);
        }

      } else {
        rc = RC::UNSUPPORTED;
        LOG_WARN("unsupported cast from %s to %s", attr_type_to_string(left->value_type()), attr_type_to_string(right->value_type()));
        return rc;
      }
    }
    // 比较表达式， op, 左边， 右边
    ComparisonExpr *cmp_expr = new ComparisonExpr(filter_unit->comp(), std::move(left), std::move(right));
    cmp_exprs.emplace_back(cmp_expr);
  }

  unique_ptr<PredicateLogicalOperator> predicate_oper;
  if (!cmp_exprs.empty()) {
    unique_ptr<ConjunctionExpr> conjunction_expr(new ConjunctionExpr(ConjunctionExpr::Type::AND, cmp_exprs));
    predicate_oper = unique_ptr<PredicateLogicalOperator>(new PredicateLogicalOperator(std::move(conjunction_expr)));
  }

  logical_operator = std::move(predicate_oper);
  return rc;
}

int LogicalPlanGenerator::implicit_cast_cost(AttrType from, AttrType to)
{
  if (from == to) {
    return 0;
  }
  return DataType::type_instance(from)->cast_cost(to);
}

RC LogicalPlanGenerator::create_plan(InsertStmt *insert_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  Table        *table = insert_stmt->table();
  // 这个构造函数接受两个迭代器（或指针）first 和 last，用于指定一个元素范围。它会将 [first, last) 区间内的元素复制到新创建的 std::vector 中。
  // 这里的 first 指向区间的起始位置，last 指向区间的结束位置（但不包含 last 所指向的元素）
  vector<Value> values(insert_stmt->values(), insert_stmt->values() + insert_stmt->value_amount());

  InsertLogicalOperator *insert_operator = new InsertLogicalOperator(table, values);
  logical_operator.reset(insert_operator);
  return RC::SUCCESS;
}
// delete -- predicate -- get
RC LogicalPlanGenerator::create_plan(DeleteStmt *delete_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  Table                      *table       = delete_stmt->table();
  FilterStmt                 *filter_stmt = delete_stmt->filter_stmt();
  // get 操作
  unique_ptr<LogicalOperator> table_get_oper(new TableGetLogicalOperator(table, ReadWriteMode::READ_WRITE));
  // 谓词操作
  unique_ptr<LogicalOperator> predicate_oper;

  RC rc = create_plan(filter_stmt, predicate_oper);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  unique_ptr<LogicalOperator> delete_oper(new DeleteLogicalOperator(table));

  if (predicate_oper) {
    predicate_oper->add_child(std::move(table_get_oper));
    delete_oper->add_child(std::move(predicate_oper));
  } else {
    delete_oper->add_child(std::move(table_get_oper));
  }

  logical_operator = std::move(delete_oper);
  return rc;
}

RC LogicalPlanGenerator::create_plan(ExplainStmt *explain_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  unique_ptr<LogicalOperator> child_oper;

  Stmt *child_stmt = explain_stmt->child();

  RC rc = create(child_stmt, child_oper);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to create explain's child operator. rc=%s", strrc(rc));
    return rc;
  }

  logical_operator = unique_ptr<LogicalOperator>(new ExplainLogicalOperator);
  logical_operator->add_child(std::move(child_oper));
  return rc;
}

RC LogicalPlanGenerator::create_group_by_plan(SelectStmt *select_stmt, unique_ptr<LogicalOperator> &logical_operator)
{
  vector<unique_ptr<Expression>> &group_by_expressions = select_stmt->group_by();
  vector<Expression *> aggregate_expressions;
  vector<unique_ptr<Expression>> &query_expressions = select_stmt->query_expressions();
  function<RC(std::unique_ptr<Expression>&)> collector = [&](unique_ptr<Expression> &expr) -> RC {
    RC rc = RC::SUCCESS;
    if (expr->type() == ExprType::AGGREGATION) {
      expr->set_pos(aggregate_expressions.size() + group_by_expressions.size());
      aggregate_expressions.push_back(expr.get());
    }
    rc = ExpressionIterator::iterate_child_expr(*expr, collector);
    return rc;
  };

  function<RC(std::unique_ptr<Expression>&)> bind_group_by_expr = [&](unique_ptr<Expression> &expr) -> RC {
    RC rc = RC::SUCCESS;
    for (size_t i = 0; i < group_by_expressions.size(); i++) {
      auto &group_by = group_by_expressions[i];
      if (expr->type() == ExprType::AGGREGATION) {
        break;
      } else if (expr->equal(*group_by)) {
        expr->set_pos(i);
        continue;
      } else {
        rc = ExpressionIterator::iterate_child_expr(*expr, bind_group_by_expr);
      }
    }
    return rc;
  };

 bool found_unbound_column = false;
  function<RC(std::unique_ptr<Expression>&)> find_unbound_column = [&](unique_ptr<Expression> &expr) -> RC {
    RC rc = RC::SUCCESS;
    if (expr->type() == ExprType::AGGREGATION) {
      // do nothing
    } else if (expr->pos() != -1) {
      // do nothing
    } else if (expr->type() == ExprType::FIELD) {
      found_unbound_column = true;
    }else {
      rc = ExpressionIterator::iterate_child_expr(*expr, find_unbound_column);
    }
    return rc;
  };
  

  for (unique_ptr<Expression> &expression : query_expressions) {
    bind_group_by_expr(expression);
  }

  for (unique_ptr<Expression> &expression : query_expressions) {
    find_unbound_column(expression);
  }

  // collect all aggregate expressions
  for (unique_ptr<Expression> &expression : query_expressions) {
    collector(expression);
  }

  if (group_by_expressions.empty() && aggregate_expressions.empty()) {
    // 既没有group by也没有聚合函数，不需要group by
    return RC::SUCCESS;
  }

  if (found_unbound_column) {
    LOG_WARN("column must appear in the GROUP BY clause or must be part of an aggregate function");
    return RC::INVALID_ARGUMENT;
  }

  // 如果只需要聚合，但是没有group by 语句，需要生成一个空的group by 语句

  auto group_by_oper = make_unique<GroupByLogicalOperator>(std::move(group_by_expressions),
                                                           std::move(aggregate_expressions));
  logical_operator = std::move(group_by_oper);
  return RC::SUCCESS;
}
// 生成 update 逻辑计划
RC LogicalPlanGenerator::create_plan(UpdateStmt *update_stmt, unique_ptr<LogicalOperator> & logical_operator){
	// 获取表，filter
	Table                      *table       = update_stmt->table();
	FilterStmt                 *filter_stmt = update_stmt->filter_stmt();
	// get 操作
	unique_ptr<LogicalOperator> table_get_oper(new TableGetLogicalOperator(table, ReadWriteMode::READ_WRITE));
	// 谓词操作
	unique_ptr<LogicalOperator> predicate_oper;

	RC rc = create_plan(filter_stmt, predicate_oper);
	if (rc != RC::SUCCESS) {
		return rc;
	}
  // todo , set a = 3
	unique_ptr<LogicalOperator> update_oper(new UpdateLogicalOperator(table, update_stmt->getAttributeName(), update_stmt->values()));
  // delete(update) -- predicate -- get
	if (predicate_oper) {
		predicate_oper->add_child(std::move(table_get_oper));
		update_oper->add_child(std::move(predicate_oper));
	} else {
		update_oper->add_child(std::move(table_get_oper));
	}

	logical_operator = std::move(update_oper);
	return rc;
}
