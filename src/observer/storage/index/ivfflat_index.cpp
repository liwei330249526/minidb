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
// Created by liwei on 2025/4/13.
//

#include "storage/index/ivfflat_index.h"
#include "common/log/log.h"
#include "storage/table/table.h"
#include "storage/db/db.h"


RC IvfflatIndex::create(Table *table, Trx *trx, const char *file_name, const VectorIndexMeta &index_meta, const vector<FieldMeta*> &field_meta) {
  if (inited_) {
    LOG_WARN("Failed to create index due to the index has been created before. file_name:%s, index:%s, field:%s",
             file_name, index_meta.name(), index_meta.field());
    return RC::RECORD_OPENNED;
  }

  // 这里可以进行一些初始化操作，如设置索引元数据等
//  Index::init_index_meta(index_meta);
  index_meta_ = index_meta;

  RC rc = index_handler_.create(table, trx, index_meta);
  if (RC::SUCCESS != rc) {
    LOG_WARN("Failed to create index_handler, file_name:%s, index:%s, field:%s, rc:%s",
             file_name, index_meta.name(), index_meta.field(), strrc(rc));
    return rc;
  }

  inited_ = true;
  table_ = table;
  LOG_INFO("Successfully create index, file_name:%s, index:%s, field:%s",
           file_name, index_meta.name(), index_meta.field());
  return RC::SUCCESS;
}

RC IvfflatIndex::open(Table *table, const char *file_name, const IndexMeta &index_meta, const FieldMeta &field_meta) {
  if (inited_) {
    LOG_WARN("Failed to open index due to the index has been initedd before. file_name:%s, index:%s, field:%s",
             file_name, index_meta.name(), index_meta.field());
    return RC::RECORD_OPENNED;
  }

  // 初始化索引元数据等
  vector<FieldMeta*> field_metas;
  field_metas.push_back((FieldMeta*)(&field_meta));
  Index::init(index_meta, field_metas);

//  BufferPoolManager &bpm = table->db()->buffer_pool_manager();
  RC rc = index_handler_.open(/* 相关参数 */);
  if (RC::SUCCESS != rc) {
    LOG_WARN("Failed to open index_handler, file_name:%s, index:%s, field:%s, rc:%s",
             file_name, index_meta.name(), index_meta.field(), strrc(rc));
    return rc;
  }

  inited_ = true;
  table_ = table;
  LOG_INFO("Successfully open index, file_name:%s, index:%s, field:%s",
           file_name, index_meta.name(), index_meta.field());
  return RC::SUCCESS;
}

//RC IvfflatIndex::init(VectorIndexType type, DistanceType distance, int lists, int probes) {
//  type_ = type;
//  distance_ = distance;
//  lists_ = lists;
//  probes_ = probes;
//  return RC::SUCCESS;
//}
vector<RID> IvfflatIndex::ann_search(const vector<float> &base_vector, size_t limit) {
  return vector<RID>();
}

RC IvfflatIndex::close() {
  if (inited_) {
    LOG_INFO("Begin to close index, index:%s, field:%s", index_meta_.name(), index_meta_.field());
    index_handler_.close();
    inited_ = false;
  }
  LOG_INFO("Successfully close index.");
  return RC::SUCCESS;
}

// 从行数据获取
RC IvfflatIndex::insert_entry(const char *recordData, const RID *rid) {
  // 从 record 获取 vector，
  // insert  vector, rid
  RowTuple tuple;
  Value val;
  Record record;
  record.set_data((char *)recordData);
  record.set_rid(*rid);

  // ok
//  int offset = index_meta_.getFieldMetas().front().offset();
//  int dataLen = index_meta_.getFieldMetas().front().len();
//  vector<float> vctor(dataLen / sizeof(float));
//  memcpy(vctor.data(), record + offset, dataLen);

//  attr_type_ = AttrType::VECTORS;
//  value_.vector_value_ = new vector<float>(other.length() / sizeof(float)); // 一个float 的数组指针, 数据长度为 lenght_字节
//  memcpy(this->value_.vector_value_->data(), other.value_.vector_value_->data(), other.length());

  // 一行数据， 数据位置传入，插入索引
  tuple.set_record(&record);
  tuple.set_schema(table_, table_->table_meta().field_metas());
  tuple.cell_at(index_meta_.getFieldMetas().front().field_id(), val);
  vector<float> vctor = val.get_vector();
  insert_entry(vctor, rid);

  return RC::INTERNAL;
}


RC IvfflatIndex::insert_entry(Record &record, const RID *rid) {
  // 从 record 获取 vector，
  // insert  vector, rid
  RowTuple tuple;
  Value val;

  // 一行数据， 数据位置传入，插入索引
  tuple.set_record(&record);
  tuple.set_schema(table_, table_->table_meta().field_metas());
  tuple.cell_at(index_meta_.getFieldMetas().front().field_id(), val);
  vector<float> vctor = val.get_vector();
  return insert_entry(vctor,&(record.rid()));
}

RC IvfflatIndex::insert_entry(const vector<float> &vctor, const RID *rid) {
  // 从 record 获取 vector，
  // insert  vector, rid
  return index_handler_.insert_entry(vctor, rid);
}


RC IvfflatIndex::delete_entry(const char *recordData, const RID *rid) {
  // 从 record 获取 vector，
  // insert  vector, rid
  RowTuple tuple;
  Value val;
  Record record;
  record.set_data((char *)recordData);
  record.set_rid(*rid);

  // ok
//  int offset = index_meta_.getFieldMetas().front().offset();
//  int dataLen = index_meta_.getFieldMetas().front().len();
//  vector<float> vctor(dataLen / sizeof(float));
//  memcpy(vctor.data(), record + offset, dataLen);

//  attr_type_ = AttrType::VECTORS;
//  value_.vector_value_ = new vector<float>(other.length() / sizeof(float)); // 一个float 的数组指针, 数据长度为 lenght_字节
//  memcpy(this->value_.vector_value_->data(), other.value_.vector_value_->data(), other.length());

  // 一行数据， 数据位置传入，插入索引
  tuple.set_record(&record);
  tuple.set_schema(table_, table_->table_meta().field_metas());
  tuple.cell_at(index_meta_.getFieldMetas().front().field_id(), val);
  vector<float> vctor = val.get_vector();
  return delete_entry(vctor, rid);
}

RC IvfflatIndex::delete_entry(const vector<float> &vctor, const RID *rid) {
  return index_handler_.delete_entry(vctor, rid);
}

RC IvfflatIndex::sync() {
  return RC::RECORD_EOF;
}

IndexScanner *IvfflatIndex::create_scanner(vector<Value> &left_key, bool left_inclusive, vector<Value> &right_key,
                                           bool right_inclusive) {
  return nullptr;
}

