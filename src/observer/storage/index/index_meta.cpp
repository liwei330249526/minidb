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

#include "storage/index/index_meta.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "storage/field/field_meta.h"
#include "storage/table/table_meta.h"
#include "json/json.h"

const static Json::StaticString FIELD_NAME("name");
const static Json::StaticString FIELD_FIELD_NAME("field_name");

RC IndexMeta::init(const char *name, const vector<FieldMeta*> &field_metas)
{
  if (common::is_blank(name)) {
    LOG_ERROR("Failed to init index, name is empty.");
    return RC::INVALID_ARGUMENT;
  }

  name_  = name;
  for (size_t i = 0; i < field_metas.size(); i++) {
    FieldMeta *fm = field_metas[i];
    field_metas_.push_back(*fm);
    field_metas_str_ += fm->name();
    if (i != field_metas.size()) {
      field_metas_str_ += '-';
    }
  }
//  for (auto &fm : field_metas) {
//    field_.push_back(fm->name());
//    field_mame_ = field_mame_ + fm->name() + "_";
//  }
//  field_ = field.name();
  return RC::SUCCESS;
}

RC IndexMeta::init(const char *name, vector<FieldMeta> &field_metas) {
  if (common::is_blank(name)) {
    LOG_ERROR("Failed to init index, name is empty.");
    return RC::INVALID_ARGUMENT;
  }

  name_  = name;
  for (auto &fm : field_metas) {
    field_metas_.push_back(fm);
  }
  return RC::SUCCESS;
}

void IndexMeta::to_json(Json::Value &json_value) const
{
  json_value[FIELD_NAME]       = name_;
  for (auto &fm : field_metas_) {
    Json::Value v;
    fm.to_json(v);
    json_value[FIELD_FIELD_NAME].append(v);
  }
}

RC IndexMeta::from_json(const TableMeta &table, const Json::Value &json_value, IndexMeta &index)
{
  const Json::Value &name_value  = json_value[FIELD_NAME];
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
  vector<FieldMeta > field_metas(field_value.size());
  for (int i = 0; i < field_value.size(); i++) {
    FieldMeta::from_json(field_value[i], field_metas[i]);
  }

  return index.init(name_value.asCString(), field_metas);
}

const char *IndexMeta::name() const { return name_.c_str(); }

const char *IndexMeta::field() const { return field_metas_str_.c_str(); }

void IndexMeta::desc(ostream &os) const { os << "index name=" << name_ << ", field=" << field_metas_str_; }

const vector<FieldMeta> &IndexMeta::getFieldMetas() const {
  return field_metas_;
}

void IndexMeta::setFieldMetas(const vector<FieldMeta> &fieldMetas) {
  field_metas_ = fieldMetas;
}


