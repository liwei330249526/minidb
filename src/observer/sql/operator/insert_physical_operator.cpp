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
// Created by WangYunlai on 2021/6/9.
//

#include "sql/operator/insert_physical_operator.h"
#include "sql/stmt/insert_stmt.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"

using namespace std;

// 参数是一个右值引用 values，它接收一个临时的 std::vector<Value> 对象。
// 右值引用允许我们接收临时对象并进行资源转移，避免不必要的复制操作，提高性能。
InsertPhysicalOperator::InsertPhysicalOperator(Table *table, vector<Value> &&values)
    : table_(table), values_(std::move(values))
    // 使用 std::move 函数将传入的 std::vector<Value> 对象 values 的资源转移给类的成员变量 values_。std::move 会将左值转换为右值引用，
    // 从而触发 std::vector 的移动构造函数，将 values 的内部资源（如动态分配的内存）直接转移给 values_，而不是进行深拷贝，
    // 这样可以避免不必要的内存分配和数据复制，提高性能。
{}

RC InsertPhysicalOperator::open(Trx *trx)
{
  Record record;
  // 构造record
  RC     rc = table_->make_record(static_cast<int>(values_.size()), values_.data(), record);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to make record. rc=%s", strrc(rc));
    return rc;
  }
  // insert 物理算子
  rc = trx->insert_record(table_, record);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to insert record by transaction. rc=%s", strrc(rc));
  }
  return rc;
}

RC InsertPhysicalOperator::next() { return RC::RECORD_EOF; }

RC InsertPhysicalOperator::close() { return RC::SUCCESS; }
