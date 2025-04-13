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
// Created by Wangyunlai.wyl on 2021/5/18.
//

#include "storage/index/vector_index_meta.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "storage/field/field_meta.h"
#include "storage/table/table_meta.h"
#include "json/json.h"

const static Json::StaticString FIELD_NAME("name");
const static Json::StaticString FIELD_FIELD_NAME("field_name");

RC VectorIndexMeta::init(const char *name, const vector<FieldMeta*> &field_metas,  VectorIndexType type, DistanceType distance, int lists, int probes)
{
  if (common::is_blank(name)) {
    LOG_ERROR("Failed to init index, name is empty.");
    return RC::INVALID_ARGUMENT;
  }
  // field_metas 设置
  name_  = name;
  for (size_t i = 0; i < field_metas.size(); i++) {
    FieldMeta *fm = field_metas[i];
    field_metas_.push_back(*fm);
    field_metas_str_ += fm->name();
    if (i != field_metas.size()) {
      field_metas_str_ += '-';
    }
  }
  type_ = type;
  distance_ = distance;
  lists_ = lists;
  probes_ = probes;
//  for (auto &fm : field_metas) {
//    field_.push_back(fm->name());
//    field_mame_ = field_mame_ + fm->name() + "_";
//  }
//  field_ = field.name();
  return RC::SUCCESS;
}

RC VectorIndexMeta::init(const char *name, vector<FieldMeta> &field_metas,  VectorIndexType type, DistanceType distance, int lists, int probes) {
  if (common::is_blank(name)) {
    LOG_ERROR("Failed to init index, name is empty.");
    return RC::INVALID_ARGUMENT;
  }

  name_  = name;
  for (auto &fm : field_metas) {
    field_metas_.push_back(fm);
  }
  type_ = type;
  distance_ = distance;
  lists_ = lists;
  probes_ = probes;
  return RC::SUCCESS;
}

void VectorIndexMeta::to_json(Json::Value &json_value) const
{
  // 索引名字
  json_value[FIELD_NAME]       = name_;
  for (auto &fm : field_metas_) {
    Json::Value v;
    fm.to_json(v);
    // FieldMeta 数组
    json_value[FIELD_FIELD_NAME].append(v);
  }
}

RC VectorIndexMeta::from_json(const TableMeta &table, const Json::Value &json_value, VectorIndexMeta &index)
{
  // index 名字
  const Json::Value &name_value  = json_value[FIELD_NAME];
  //  FieldMeta 数组
  const Json::Value &field_value = json_value[FIELD_FIELD_NAME];
  if (!name_value.isString()) {
    LOG_ERROR("Index name is not a string. json value=%s", name_value.toStyledString().c_str());
    return RC::INTERNAL;
  }

  if (!field_value.isString()) {
    LOG_ERROR("Field name of index [%s] is not a string. json value=%s",
              name_value.asCString(), field_value.toStyledString().c_str());
//    return RC::INTERNAL;
  }

//  const FieldMeta *field = table.field(field_value.asCString());
//  if (nullptr == field) {
//    LOG_ERROR("Deserialize index [%s]: no such field: %s", name_value.asCString(), field_value.asCString());
//    return RC::SCHEMA_FIELD_MISSING;
//  }
  // 获取  FieldMeta 数组
  vector<FieldMeta > field_metas(field_value.size());
  for (int i = 0; i < field_value.size(); i++) {
    FieldMeta::from_json(field_value[i], field_metas[i]);
  }

  return index.init(name_value.asCString(), field_metas);
}

const char *VectorIndexMeta::name() const { return name_.c_str(); }

const char *VectorIndexMeta::field() const { return field_metas_str_.c_str(); }

void VectorIndexMeta::desc(ostream &os) const { os << "index name=" << name_ << ", field=" << field_metas_str_; }

// 获取 FieldMeta 数组
const vector<FieldMeta> &VectorIndexMeta::getFieldMetas() const {
  return field_metas_;
}

void VectorIndexMeta::setFieldMetas(const vector<FieldMeta> &fieldMetas) {
  field_metas_ = fieldMetas;
}

int VectorIndexMeta::getLists() const {
  return lists_;
}

void VectorIndexMeta::setLists(int lists) {
  lists_ = lists;
}

int VectorIndexMeta::getProbes() const {
  return probes_;
}

void VectorIndexMeta::setProbes(int probes) {
  probes_ = probes;
}
