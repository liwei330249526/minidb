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
// Created by Wangyunlai on 2023/4/25.
//

#include "sql/stmt/create_vector_index_stmt.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "storage/db/db.h"
#include "storage/table/table.h"

using namespace std;
using namespace common;

RC CreateVectorIndexStmt::create(Db *db, const CreateVectorIndexSqlNode &create_index, Stmt *&stmt)
{
  stmt = nullptr;
  // 获取表名
  const char *table_name = create_index.relation_name.c_str();
  if (is_blank(table_name) || is_blank(create_index.index_name.c_str()) ||
      create_index.attribute_name.empty()) {
    LOG_WARN("invalid argument. db=%p, table_name=%p, index name=%s",
             db, table_name, create_index.index_name.c_str());
    return RC::INVALID_ARGUMENT;
  }

  // check whether the table exists
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  // 构造 索引列 元数据数组
  vector<FieldMeta*> field_metas;
  // 遍历每个列名，获取这个列, 加入到field_metas 数组
  FieldMeta *field_meta = const_cast<FieldMeta *>(table->table_meta().field(create_index.attribute_name.c_str()));
  if (nullptr == field_meta) {
    LOG_WARN("no such field in table. db=%s, table=%s, field name=%s",
             db->name(), table_name);
    return RC::SCHEMA_FIELD_NOT_EXIST;
  }
  field_metas.push_back(field_meta);

  // 判断所以是否已经存在
  Index *index = table->find_index(create_index.index_name.c_str());
  if (nullptr != index) {
    LOG_WARN("index with name(%s) already exists. table name=%s", create_index.index_name.c_str(), table_name);
    return RC::SCHEMA_INDEX_NAME_REPEAT;
  }
  // 表，列元数据，索引名字
  //  todo: 4 个参数; TYPE=IVFFLAT, DISTANCE=L2_DISTANCE, LISTS=3, PROBES=3 解析， 对接 vetor index
  DistanceType distance;
  VectorIndexType type;
  int lists;
  int probes;
  for (auto p : create_index.params) {
    string key = common::str_to_upper(p.first);
    string val = common::str_to_upper(p.second);

    if (key == "TYPE") {
      if (val == "IVFFLAT") {
        type = VectorIndexType::IFV_FLAT;
      } else {
        LOG_WARN("vector type is %s, is not IVFFLAT", val.c_str());
        return RC::INTERNAL;
      }
    } else if (key == "DISTANCE") {
      if (val == "L2_DISTANCE") {
        distance = DistanceType::L2_DISTANCE;
      } else if (val == "INNER_PRODUCT") {
        distance = DistanceType::INNER_PRODUCT;
      } else if (val == "COSINE_DISTANCE") {
        distance = DistanceType::COSINE_DISTANCE;
      } else {
        LOG_WARN("DistanceType type is %s,  not  L2_DISTANCE or INNER_PRODUCT or COSINE_DISTANCE", val.c_str());
        return RC::INTERNAL;
      }
    } else if (key == "LISTS") {
      lists = stoi(val);
    } else if (key == "PROBES") {
      probes = stoi(val);
    } else {
      LOG_WARN("param key type is %s,  not  TYPE OR DISTANCE OR LISTS OR PROBES", key.c_str());
      return RC::INTERNAL;
    }
  }
  stmt = new CreateVectorIndexStmt(table, field_metas, create_index.index_name, type, distance, lists, probes);
  return RC::SUCCESS;
}
