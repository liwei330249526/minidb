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

#pragma once

#include <string>
#include <src/observer/storage/index/vector_index_meta.h>

#include "sql/stmt/stmt.h"

struct CreateVectorIndexSqlNode;
class Table;
class FieldMeta;

/**
 * @brief 创建索引的语句
 * @ingroup Statement
 */
class CreateVectorIndexStmt : public Stmt
{
public:
    CreateVectorIndexStmt(Table *table, const FieldMeta *field_meta, const std::string &index_name)
            : table_(table), field_meta_(field_meta), index_name_(index_name)
    {}
    // 携带了 Vector index 的所有信息
    CreateVectorIndexStmt(Table *table, vector<FieldMeta*> field_metas , const std::string &index_name, VectorIndexType type, DistanceType distance, int lists, int probes)
            : table_(table), field_metas_(field_metas), index_name_(index_name), type_(type), distance_(distance), lists_(lists), probes_(probes)
    {}

    virtual ~CreateVectorIndexStmt() = default;

    StmtType type() const override { return StmtType::CREATE_VECTOR_INDEX; }

    Table             *table() const { return table_; }
    const FieldMeta   *field_meta() const { return field_meta_; }
    const vector<FieldMeta*> &field_metas() const { return field_metas_; }
    const std::string &index_name() const { return index_name_; }

public:
    static RC create(Db *db, const CreateVectorIndexSqlNode &create_index, Stmt *&stmt);

    VectorIndexType getType() const {
      return type_;
    }

    void setType(VectorIndexType type) {
      type_ = type;
    }

    DistanceType getDistance() const {
      return distance_;
    }

    void setDistance(DistanceType distance) {
      distance_ = distance;
    }

    int getLists() const {
      return lists_;
    }

    void setLists(int lists) {
      lists_ = lists;
    }

    int getProbes() const {
      return probes_;
    }

    void setProbes(int probes) {
      probes_ = probes;
    }

private:
    Table           *table_      = nullptr;  // 表
    const FieldMeta *field_meta_ = nullptr; // 弃用
    vector<FieldMeta*> field_metas_; // 索引列表
    std::string      index_name_;

//    std::pair<std::string, std::string> params_[4]; // 4 个参数; TYPE=IVFFLAT, DISTANCE=L2_DISTANCE, LISTS=3, PROBES=3
    VectorIndexType type_;
    // Vector index type， 例如 ivf-flat
    DistanceType distance_;  // 距离公式
    // LISTS参数：用于指定在建立索引时将向量集划分成子集的数量。比如LISTS = 3，就是把向量数据分成 3 个子集。该参数值越大，意味着划分的子集越多，
    // 每个子集内的向量数量相对越少，这样在查询时可以缩小搜索范围，从而加快查询速度，但同时会消耗更多的内存，索引构建时间也会更长，并且可能会导致召回率降低。
    int lists_;
    // PROBES参数：是一个查询时参数，用于指定在查询过程中要考虑的子集数量。例如PROBES = 3，表示在查询时会搜索 3 个子集。默认值是 1，即只搜索与查询向量最近的那个子集。
    // 增加PROBES的值，可以扩大搜索范围，提高召回率，也就是更有可能找到真正与查询向量相似的向量，但代价是查询速度会变慢。
    int probes_;
};
