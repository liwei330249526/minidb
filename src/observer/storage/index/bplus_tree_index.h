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
// Created by wangyunlai.wyl on 2021/5/19.
//

#pragma once

#include "storage/index/bplus_tree.h"
#include "storage/index/index.h"

/**
 * @brief B+树索引
 * @ingroup Index
 */
class BplusTreeIndex : public Index
{
public:
  BplusTreeIndex() = default;
  virtual ~BplusTreeIndex() noexcept;
  // 创建索引，table， file_name, 索引元数据(索引名字，索引列集合)， 列集合
  RC create(Table *table, const char *file_name, const IndexMeta &index_meta, const vector<FieldMeta*> &field_metas) override;
	// 删除索引， table， file_name， 索引元数据
  virtual RC drop(Table *table, const char *file_name, const IndexMeta &index_meta) override;
  // 打开索引
  RC open(Table *table, const char *file_name, const IndexMeta &index_meta, vector<FieldMeta*> &field_metas) override;
  // 关闭索引
  RC close();
  // 插入数据， 通过record 和列集合和rid 构造key， 将rid 插入索引
  RC insert_entry(const char *record, const RID *rid) override;
  // 插入数据， 通过record 和列集合和rid 构造key， 将rid 插入索引
  RC delete_entry(const char *record, const RID *rid) override;

  /**
   * 扫描指定范围的数据
   */
   // 构造扫描器
  IndexScanner *create_scanner(vector<Value> &left_key, bool left_inclusive, vector<Value> &right_key, bool right_inclusive) override;

  RC sync() override;

private:
  bool             inited_ = false;
  Table           *table_  = nullptr;
  BplusTreeHandler index_handler_;
};

/**
 * @brief B+树索引扫描器
 * @ingroup Index
 */
class BplusTreeIndexScanner : public IndexScanner
{
public:
  BplusTreeIndexScanner(BplusTreeHandler &tree_handle);
  ~BplusTreeIndexScanner() noexcept override;

  RC next_entry(RID *rid) override;
  RC destroy() override;

  RC open(vector<Value> &left_key, bool left_inclusive, vector<Value> &right_key,bool right_inclusive);

private:
  BplusTreeScanner tree_scanner_;
};
