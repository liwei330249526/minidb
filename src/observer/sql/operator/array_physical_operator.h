/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#ifndef ARRAY_PHYSICAL_OPERATOR_H
#define ARRAY_PHYSICAL_OPERATOR_H

#include <memory>
#include <string>
#include <vector>
#include "physical_operator.h"

// 数组物理算子类
class ArrayPhysicalOperator : public PhysicalOperator {
public:
    ArrayPhysicalOperator(const std::vector<Value>& values);
    std::string name() const override;
    std::string param() const override;
    PhysicalOperatorType type() const override;
    RC open(Trx* trx) override;
    RC next() override;
    RC close() override;
    Tuple* current_tuple() override;
    void set_predicates(std::vector<std::unique_ptr<Expression>>&& exprs);
    RC filter(RowTuple& tuple, bool& result);
private:
    std::vector<Value> values_;
//    ValueListTuple valListTuple_;
    size_t current_index_;
    std::unique_ptr<ValueListTuple> current_tuple_;
    RowTuple tuple_;
    std::vector<std::unique_ptr<Expression>> predicates_;
};

#endif // ARRAY_PHYSICAL_OPERATOR_H
