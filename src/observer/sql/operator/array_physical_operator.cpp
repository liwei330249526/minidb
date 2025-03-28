#include "array_physical_operator.h"
#include <iostream>

// ArrayPhysicalOperator 类实现
ArrayPhysicalOperator::ArrayPhysicalOperator(const std::vector<Value>& values)
        : values_(values), current_index_(0) {
  // 构造
//  vector<Value> valuesIn;
//  vector<TupleCellSpec> specsIn;
//  for (int i = 0; i < values_.size(); i++)
//  {
//    valuesIn.emplace_back(i);
//    specsIn.emplace_back(TupleCellSpec(std::to_string(i)));
//  }
//
//  valListTuple_.set_cells(valuesIn);
//  valListTuple_.set_names(specsIn);
}

std::string ArrayPhysicalOperator::name() const {
  return "ArrayPhysicalOperator";
}

std::string ArrayPhysicalOperator::param() const {
  std::string result;
  for (auto &val : values_) {
    result += val.to_string() + " ";
  }
  return result;
}

PhysicalOperatorType ArrayPhysicalOperator::type() const {
  return PhysicalOperatorType::ARRAY_OPERATOR;
}

RC ArrayPhysicalOperator::open(Trx* trx) {
  current_index_ = 0; // 初始化index 为0
  return RC::SUCCESS;
}

RC ArrayPhysicalOperator::next() {
  RC rc = RC::SUCCESS;
  while (current_index_ < values_.size()) {
    current_tuple_ = std::make_unique<ValueListTuple>();
    current_tuple_->set_cells({values_[current_index_]});
    current_tuple_->set_names({TupleCellSpec(std::to_string(0))});

//    std::cout << "get a tuple: " << current_tuple_->to_string() << std::endl;
    current_index_++;
    return rc;
  }
  if (current_index_ >= values_.size()) {
    return RC::RECORD_EOF;
  }
  return rc;
}

RC ArrayPhysicalOperator::close() {
  current_index_ = 0;
  current_tuple_.reset();
  return RC::SUCCESS;
}

Tuple* ArrayPhysicalOperator::current_tuple() {
  return current_tuple_.get();
}

void ArrayPhysicalOperator::set_predicates(std::vector<std::unique_ptr<Expression>>&& exprs) {
  predicates_ = std::move(exprs);
}

RC ArrayPhysicalOperator::filter(RowTuple& tuple, bool& result) {
  RC rc = RC::SUCCESS;
  Value value;
  for (std::unique_ptr<Expression>& expr : predicates_) {
    rc = expr->get_value(tuple, value);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    bool tmp_result = value.get_boolean();
    if (!tmp_result) {
      result = false;
      return rc;
    }
  }
  result = true;
  return rc;
}
