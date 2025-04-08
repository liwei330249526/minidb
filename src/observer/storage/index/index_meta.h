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
// Created by Wangyunlai on 2021/5/12.
//

#pragma once

#include "common/rc.h"
#include "common/lang/string.h"

class TableMeta;
class FieldMeta;

namespace Json {
class Value;
}  // namespace Json

/**
 * @brief 描述一个索引
 * @ingroup Index
 * @details 一个索引包含了表的哪些字段，索引的名称等。
 * 如果以后实现了多种类型的索引，还需要记录索引的类型，对应类型的一些元数据等
 */
class IndexMeta
{
public:
  IndexMeta() = default;

  RC init(const char *name, const vector<FieldMeta*> &field_metas);
  RC init(const char *name, vector<FieldMeta> &field_metas);

public:
  const char *name() const;
  // 列名字集合的字符串
  const char *field() const;

  void desc(ostream &os) const;

  void      to_json(Json::Value &json_value) const;
  static RC from_json(const TableMeta &table, const Json::Value &json_value, IndexMeta &index);
  const vector<FieldMeta> &getFieldMetas() const;
  void setFieldMetas(const vector<FieldMeta> &fieldMetas);

protected:
  string name_;   // index's name   索引名字
  mutable vector<FieldMeta> field_metas_; // FieldMeta 数组

protected:
    // 可以是多个列的名字， 这里不能是指针，需要时数据
  string field_metas_str_;  // field's name   可以是多个列的名字, name1-name2-name3
};
