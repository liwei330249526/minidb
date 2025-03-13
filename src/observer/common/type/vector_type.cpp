/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include <iomanip>
#include "common/lang/comparator.h"
#include "common/log/log.h"
#include "common/type/vector_type.h"
#include "common/value.h"

int VectorType::compare(const Value &left, const Value &right) const
{
  ASSERT(!((left.attr_type() != AttrType::VECTORS && left.attr_type() != AttrType::CHARS) ||
         (right.attr_type() != AttrType::VECTORS && right.attr_type() != AttrType::CHARS)), "invalid type");
//  if (left.attr_type() != AttrType::VECTORS || right.attr_type() != AttrType::VECTORS) {
//    return RC::INVALID_ARGUMENT;
//  }
  // 常量引用可以绑定到临时对象，延长临时对象的生命周期，避免了不必要的拷贝。这里的临时对象就是 get_vector() 函数返回的 std::vector<float> 对象的副本。
  const auto &left_vec = left.get_vector();
  const auto &right_vec = right.get_vector();
  ASSERT(left_vec.size() == right_vec.size(), "invalid size");
  int ret;
//  if (left_vec.size() != right_vec.size()) {
//    return RC::INVALID_ARGUMENT;
//  }
  // 遍历每个元素，进行加法操作
  std::vector<float> result_vec(left_vec.size());
  for (size_t i = 0; i < left_vec.size(); ++i) {

    float left_val  = left_vec[i];
    float right_val = right_vec[i];
    ret = common::compare_float((void *)&left_val, (void *)&right_val);
    if (ret != 0) {
       return ret;
    }
  }
  return ret;
}

// 向量加法
RC VectorType::add(const Value &left, const Value &right, Value &result) const {
  if ((left.attr_type() != AttrType::VECTORS && left.attr_type() != AttrType::CHARS) ||
      (right.attr_type() != AttrType::VECTORS && right.attr_type() != AttrType::CHARS)) {
    return RC::INVALID_ARGUMENT;
  }
  // 常量引用可以绑定到临时对象，延长临时对象的生命周期，避免了不必要的拷贝。这里的临时对象就是 get_vector() 函数返回的 std::vector<float> 对象的副本。
  const auto &left_vec = left.get_vector();
  const auto &right_vec = right.get_vector();
  if (left_vec.size() != right_vec.size()) {
    return RC::INVALID_ARGUMENT;
  }
  // 遍历每个元素，进行加法操作
  std::vector<float> result_vec(left_vec.size());
  for (size_t i = 0; i < left_vec.size(); ++i) {
    result_vec[i] = left_vec[i] + right_vec[i];
  }
  result.set_vector(result_vec);
  return RC::SUCCESS;
}

// 向量减法
RC VectorType::subtract(const Value &left, const Value &right, Value &result) const {
  if ((left.attr_type() != AttrType::VECTORS && left.attr_type() != AttrType::CHARS) ||
      (right.attr_type() != AttrType::VECTORS && right.attr_type() != AttrType::CHARS)) {
    return RC::INVALID_ARGUMENT;
  }
  // 常量引用可以绑定到临时对象，延长临时对象的生命周期，避免了不必要的拷贝。这里的临时对象就是 get_vector() 函数返回的 std::vector<float> 对象的副本。
  const auto &left_vec = left.get_vector();
  const auto &right_vec = right.get_vector();
  if (left_vec.size() != right_vec.size()) {
    return RC::INVALID_ARGUMENT;
  }
  // 遍历每个元素，进行减法操作
  std::vector<float> result_vec(left_vec.size());
  for (size_t i = 0; i < left_vec.size(); ++i) {
    result_vec[i] = left_vec[i] - right_vec[i];
  }
  result.set_vector(result_vec);
  return RC::SUCCESS;
}

RC VectorType::multiply(const Value &left, const Value &right, Value &result) const {
  if ((left.attr_type() != AttrType::VECTORS && left.attr_type() != AttrType::CHARS) ||
      (right.attr_type() != AttrType::VECTORS && right.attr_type() != AttrType::CHARS)) {
    return RC::INVALID_ARGUMENT;
  }
  // 常量引用可以绑定到临时对象，延长临时对象的生命周期，避免了不必要的拷贝。这里的临时对象就是 get_vector() 函数返回的 std::vector<float> 对象的副本。
  const auto &left_vec = left.get_vector();
  const auto &right_vec = right.get_vector();
  if (left_vec.size() != right_vec.size()) {
    return RC::INVALID_ARGUMENT;
  }
  // 遍历每个元素，进行减法操作
  std::vector<float> result_vec(left_vec.size());
  for (size_t i = 0; i < left_vec.size(); ++i) {
    result_vec[i] = left_vec[i] * right_vec[i];
  }
  result.set_vector(result_vec);
  return RC::SUCCESS;
}

// 点积
RC VectorType::dot(const Value &left, const Value &right, Value &result) const {
  if ((left.attr_type() != AttrType::VECTORS && left.attr_type() != AttrType::CHARS) ||
      (right.attr_type() != AttrType::VECTORS && right.attr_type() != AttrType::CHARS)) {
    return RC::INVALID_ARGUMENT;
  }
  const auto &left_vec = left.get_vector();
  const auto &right_vec = right.get_vector();
  if (left_vec.size() != right_vec.size()) {
    return RC::INVALID_ARGUMENT;
  }
  float dot_product = 0.0f;
  for (size_t i = 0; i < left_vec.size(); ++i) {
    dot_product += left_vec[i] * right_vec[i];
  }
  result.set_float(dot_product);
  return RC::SUCCESS;
}

RC VectorType::to_string(const Value &val, string &result) const {
  stringstream ss;
  const auto &vec = val.get_vector();
  for (size_t i = 0; i < vec.size(); ++i) {
    float v = vec[i];
    ss << common::double_to_str(v);
    if (i != vec.size()-1) {
      ss << ",";
    }
  }
  result = ss.str();
  return RC::SUCCESS;
}


