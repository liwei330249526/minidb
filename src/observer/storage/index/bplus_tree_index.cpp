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

#include "storage/index/bplus_tree_index.h"
#include "common/log/log.h"
#include "storage/table/table.h"
#include "storage/db/db.h"

BplusTreeIndex::~BplusTreeIndex() noexcept { close(); }

RC BplusTreeIndex::create(Table *table, const char *file_name, const IndexMeta &index_meta, const vector<FieldMeta*> &field_metas)
{
  if (inited_) {
    LOG_WARN("Failed to create index due to the index has been created before. file_name:%s, index:%s, field:%s",
        file_name, index_meta.name(), index_meta.field());
    return RC::RECORD_OPENNED;
  }

  Index::init(index_meta, field_metas);

  BufferPoolManager &bpm = table->db()->buffer_pool_manager();
  vector<AttrType> field_meta_types;
  vector<int> field_meta_lens;
  for (auto &fm : field_metas) {
    field_meta_types.push_back(fm->type());
    field_meta_lens.push_back(fm->len());
  }
  RC rc = index_handler_.create(table->db()->log_handler(), bpm, file_name, field_meta_types, field_meta_lens);
  if (RC::SUCCESS != rc) {
    LOG_WARN("Failed to create index_handler, file_name:%s, index:%s, field:%s, rc:%s",
        file_name, index_meta.name(), index_meta.field(), strrc(rc));
    return rc;
  }

  inited_ = true;
  table_  = table;
  LOG_INFO("Successfully create index, file_name:%s, index:%s, field:%s",
    file_name, index_meta.name(), index_meta.field());
  return RC::SUCCESS;
}

RC BplusTreeIndex::drop(Table *table, const char *file_name, const IndexMeta &index_meta)
{
	RC rc = RC::SUCCESS;
	rc = this->close();
	if(rc != RC::SUCCESS) {
		return RC::FILE_CLOSE;
	}

	BufferPoolManager &bpm = table->db()->buffer_pool_manager();
	rc = bpm.drop_file(file_name);
	if(rc != RC::SUCCESS) {
		return RC::FILE_CLOSE;
	}
	return rc;
}

RC BplusTreeIndex::open(Table *table, const char *file_name, const IndexMeta &index_meta, vector<FieldMeta*> &field_metas)
{
  if (inited_) {
    LOG_WARN("Failed to open index due to the index has been initedd before. file_name:%s, index:%s, field:%s",
        file_name, index_meta.name(), index_meta.field());
    return RC::RECORD_OPENNED;
  }

  Index::init(index_meta, field_metas);

  BufferPoolManager &bpm = table->db()->buffer_pool_manager();
  RC rc = index_handler_.open(table->db()->log_handler(), bpm, file_name);
  if (RC::SUCCESS != rc) {
    LOG_WARN("Failed to open index_handler, file_name:%s, index:%s, field:%s, rc:%s",
        file_name, index_meta.name(), index_meta.field(), strrc(rc));
    return rc;
  }

  inited_ = true;
  table_  = table;
  LOG_INFO("Successfully open index, file_name:%s, index:%s, field:%s",
    file_name, index_meta.name(), index_meta.field());
  return RC::SUCCESS;
}

RC BplusTreeIndex::close()
{
  if (inited_) {
    LOG_INFO("Begin to close index, index:%s, field:%s", index_meta_.name(), index_meta_.field());
    index_handler_.close();
    inited_ = false;
  }
  LOG_INFO("Successfully close index.");
  return RC::SUCCESS;
}

RC BplusTreeIndex::insert_entry(const char *record, const RID *rid)
{
  // 行数据，和数据位置 rid
  // 插入 data， len 的 Vector
  vector<DataWrapper> datas;
  for (auto &fm : index_meta_.getFieldMetas()) {
    datas.push_back({record + fm.offset(), fm.len()});
  }
  return index_handler_.insert_entry(datas, rid);
}

RC BplusTreeIndex::delete_entry(const char *record, const RID *rid)
{
  // 行数据，和数据位置 rid
  // 插入 data， len 的 Vector
  vector<DataWrapper> datas;
  for (auto &fm : index_meta_.getFieldMetas()) {
    datas.push_back({record + fm.offset(), fm.len()});
  }
  return index_handler_.delete_entry(datas, rid);
}

IndexScanner *BplusTreeIndex::create_scanner(
        vector<Value> &left_key, bool left_inclusive, vector<Value> &right_key, bool right_inclusive)
{
  BplusTreeIndexScanner *index_scanner = new BplusTreeIndexScanner(index_handler_);
  RC rc = index_scanner->open(left_key, left_inclusive, right_key, right_inclusive);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open index scanner. rc=%d:%s", rc, strrc(rc));
    delete index_scanner;
    return nullptr;
  }
  return index_scanner;
}

RC BplusTreeIndex::sync() { return index_handler_.sync(); }

////////////////////////////////////////////////////////////////////////////////
BplusTreeIndexScanner::BplusTreeIndexScanner(BplusTreeHandler &tree_handler) : tree_scanner_(tree_handler) {}

BplusTreeIndexScanner::~BplusTreeIndexScanner() noexcept { tree_scanner_.close(); }

RC BplusTreeIndexScanner::open(
        vector<Value> &left_key, bool left_inclusive, vector<Value> &right_key, bool right_inclusive)
{
  return tree_scanner_.open(left_key, left_inclusive, right_key, right_inclusive);
}

RC BplusTreeIndexScanner::next_entry(RID *rid) { return tree_scanner_.next_entry(*rid); }

RC BplusTreeIndexScanner::destroy()
{
  delete this;
  return RC::SUCCESS;
}
