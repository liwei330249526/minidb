/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#pragma once

#include "storage/index/index.h"
#include "storage/index/vector_index_meta.h"
#include <cmath>
#include <src/observer/sql/expr/tuple.h>
#include "src/observer/storage/table/table.h"

class IvVector{
public:
    IvVector(RID &rid, vector<float> &vt): rid_(rid), vt_(vt){}

    RID rid_;
    vector<float> vt_;
};


// 定义簇结构体
struct Cluster {
    vector<float> centroid; // 质心
    std::vector<RID> vector_indices; // 质心对应的 rid 集合
};

// IVF-Flat 索引处理类
class IVFFlatIndexHandler {
public:
    // 创建索引, 读取向量数据，构建向量簇质心，将 rid 加入到指定向量簇中
    RC create(Table *table, Trx *trx, const VectorIndexMeta &index_meta) {
      // 质心个数
      num_clusters_ = index_meta.getLists();
      // 获取向量数据
      std::vector<IvVector> all_vectors; // 所有向量
      get_vectors_from_table(table, trx, index_meta, all_vectors);

      // 使用 K-Means 算法进行向量分组
      clusters_ = kmeans(all_vectors, num_clusters_, max_iterations_);

      // 构建倒排文件索引，将向量的 RID 存储到对应簇的列表中
      for (size_t i = 0; i < all_vectors.size(); ++i) {
        insert_entry(all_vectors[i].vt_, &(all_vectors[i].rid_));
//        int nearest_cluster = find_nearest_cluster(all_vectors[i].vt_);
//        clusters_[nearest_cluster].vector_indices.push_back(all_vectors[i].rid_);
      }

      return RC::SUCCESS;
    }

    // 打开索引
    RC open(/* 相关参数 */) {
      // 实现打开已存在的 IVF-Flat 索引的逻辑
      return RC::SUCCESS;
    }

    // 关闭索引
    RC close() {
      // 实现关闭索引的逻辑，例如释放资源
      return RC::SUCCESS;
    }

    // 插入向量及其对应的记录标识符
    RC insert_entry(const vector<float> &vctor, const RID *rid) {
      int nearest_cluster = find_nearest_cluster(vctor); // 最近的质心
      clusters_[nearest_cluster].vector_indices.push_back(*rid);  // 加入到该质心的， 维护的一个 rid 集合
      return RC::SUCCESS;
    }

    // 删除向量及其对应的记录标识符
    RC delete_entry(const vector<float> &vctor, const RID *rid) {
      int nearest_cluster = find_nearest_cluster(vctor); // 最近的质心
      auto &index_list = clusters_[nearest_cluster].vector_indices;
      // std::remove 的工作原理是遍历从 index_list.begin() 到 index_list.end() 范围内的元素，将不等于 *rid 的元素依次移动到容器的前面，而等于 *rid 的元素会被移动到容器的末尾部分。
      // 最后，它会返回一个指向新的逻辑末尾的迭代器，这个迭代器之后的元素就是要被移除的元素。
      // . index_list.erase(..., index_list.end())
      // erase 是容器的成员函数，用于从容器中删除指定范围内的元素。
      index_list.erase(std::remove(index_list.begin(), index_list.end(), *rid), index_list.end()); // 这行代码的主要目的是从 index_list 中移除所有等于 *rid 的元素
      return RC::SUCCESS;
    }

    // 同步索引数据
    RC sync() {
      // 实现同步索引数据的逻辑
      return RC::SUCCESS;
    }

private:
    // 从表中获取向量数据的示例方法，实际需要根据具体逻辑实现
    RC get_vectors_from_table(Table *table, Trx *trx, const VectorIndexMeta &index_meta, std::vector<IvVector> &all_vectors) {
      RC rc = RC::SUCCESS;
      // 遍历当前的所有数据，插入这个索引
      RecordFileScanner scanner;
      rc = table->get_record_scanner(scanner, trx, ReadWriteMode::READ_ONLY);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to create scanner while creating index. table=%s, index=%s, rc=%s",
                 table->name(), index_meta.name(), strrc(rc));
        return rc;
      }

      // 遍历获取所有向量
      Record record;
      RowTuple tuple;
      Value val;
      while (OB_SUCC(rc = scanner.next(record))) {
        // 一行数据， 数据位置传入，插入索引
        tuple.set_record(&record);
        tuple.set_schema(table, table->table_meta().field_metas());
        tuple.cell_at(index_meta.getFieldMetas().front().field_id(), val);
        vector<float> vctor = val.get_vector();
        all_vectors.emplace_back(IvVector(record.rid(), vctor));
//        rc = index->insert_entry(vctor, &record.rid());
//        if (rc != RC::SUCCESS) {
//          LOG_WARN("failed to insert record into index while creating index. table=%s, index=%s, rc=%s",
//                   name(), index_name, strrc(rc));
//          return rc;
//        }
      }
      if (RC::RECORD_EOF == rc) {
        rc = RC::SUCCESS;
      }
      scanner.close_scan();
      LOG_INFO("inserted all records into new index. table=%s, index=%s", table->name(), index_meta.name());
      return RC::SUCCESS;
    }

    // 假设的获取向量对应的 RID 的方法
    RID get_rid_for_vector(int vector_index) {
      return {vector_index, vector_index};
    }

    int find_nearest_cluster(const vector<float> &vctor) {
      int nearest_index = 0;
      // 计算距离 vector 最近的质心的 index
      // 用probe ， 需要获取3个簇的数据，找到一个距离vctor 最近的质心
      float min_distance = l2_distance(vctor, clusters_[0].centroid);
      for (int i = 1; i < num_clusters_; ++i) {
        float distance = l2_distance(vctor, clusters_[i].centroid);
        if (distance < min_distance) {
          min_distance = distance;
          nearest_index = i;
        }
      }
      return nearest_index;
    }
    // 计算两个向量的欧氏距离， 差的平方和， 开方
    float l2_distance(const vector<float> &v1, const vector<float> &v2) {
      float sum = 0.0;
      for (size_t i = 0; i < v1.size(); ++i) {
        float diff = v1[i] - v2[i];
        sum += diff * diff;
      }

      return std::sqrt(sum);
    }

    // K-Means 聚类算法
    // K-Means 算法是一个迭代的聚类算法，它会不断地更新聚类中心和样本的归属，直到满足一定的停止条件。
    // 数据规模较小且聚类结构较简单：可以设置相对较小的值，如 50 到 100。例如，对于一些只有几百个样本，且聚类特征明显的数据，较小的迭代次数可能就足以让算法收敛到较好的结果。
    std::vector<Cluster> kmeans(vector<IvVector> &data, int num_clusters, int max_iterations) {
      // 多少个向量数据
      int num_vectors = data.size();
      // 几维
      // int dim = data[0].vt_.size();

      // 随机初始化质心, num_clusters 个质心
      std::vector<Cluster> centroids;
      srand(static_cast<unsigned int>(time(nullptr)));
      for (int i = 0; i < num_clusters; ++i) {
        // 随机数字
        int random_index = rand() % num_vectors;
        Cluster cluster;
        // 选一个随机向量作为质心，加入到集合
        cluster.centroid = data[random_index].vt_;
        centroids.push_back(cluster);
      }
      // 迭代次数
      for (int iteration = 0; iteration < max_iterations; ++iteration) {
        // 每个质心有一个 向量数组, 即簇;  [10] [3][vector]， 第10个质心， 有3个向量
        vector<vector<vector<float>>> clusters(num_clusters);

        // 将每个向量分配到最近的质心，将向量给这个质心
        for (IvVector &dit : data) {
          vector<float> &vt = dit.vt_;
          int nearest_cluster = 0;
          float min_distance = l2_distance(vt, centroids[0].centroid);
          // 找到这个向量 vector 最近的质心 nearest_cluster
          for (int i = 1; i < num_clusters; ++i) {
            float distance = l2_distance(vt, centroids[i].centroid);
            if (distance < min_distance) {
              min_distance = distance;
              nearest_cluster = i;
            }
          }
          // 将向量加入到最近的质心的对应的 idnex 的数组中
          clusters[nearest_cluster].push_back(vt);
        }

        // 更新质心
        bool centroids_changed = false;
        for (int i = 0; i < num_clusters; ++i) {
          // 重新计算每个质心
          vector<float> new_centroid = compute_centroid(clusters[i]);
          // 判断新质心和旧质心发生变化, 则更新质心; 更新质心后可以重新迭代一次，重新计算 向量簇
          // 如果前后质心的欧式距离发生变化, 则更新
          if (l2_distance(new_centroid, centroids[i].centroid) > 0.0001) {
            centroids[i].centroid = new_centroid;
            centroids_changed = true;
          }
        }

        // 如果质心不再变化，提前结束循环
        if (!centroids_changed) {
          break;
        }
      }

      return centroids;
    }

    // 计算向量列表的质心
    vector<float> compute_centroid(const std::vector<vector<float>> &vectors) {
      // 几个向量
      int num_vectors = vectors.size();
      // 维度
      int dim = vectors[0].size();
      vector<float> centroid(dim, 0.0);

      // 所有向量相加
      for (const auto &vector : vectors) {
        for (int i = 0; i < dim; ++i) {
          centroid[i] += vector[i];
        }
      }

      // 每个维度的平均值
      for (int i = 0; i < dim; ++i) {
        centroid[i] /= num_vectors;
      }
      // 得出心的质心
      return centroid;
    }

    int num_clusters_;
    int max_iterations_ = 50; // 默认为50
    std::vector<Cluster> clusters_;  // 假设10个质心，每个质心维护 rid 集合
};



/**
 * @brief ivfflat 向量索引
 * @ingroup Index
 */
class IvfflatIndex : public Index {
public:
    IvfflatIndex(Table *table) : table_(table){};
    virtual ~IvfflatIndex() noexcept {};

    RC create(Table *table, Trx *trx, const char *file_name, const VectorIndexMeta &index_meta, const vector<FieldMeta*> &field_meta);
    RC init(VectorIndexType type, DistanceType distance, int lists, int probes);
    RC open(Table *table, const char *file_name, const IndexMeta &index_meta, const FieldMeta &field_meta);
    vector<RID> ann_search(const vector<float> &base_vector, size_t limit);

    RC close();

    RC insert_entry(const char *record, const RID *rid) override ;
    RC insert_entry(Record &record, const RID *rid);
    RC insert_entry(const vector<float> &vctor, const RID *rid);
    RC delete_entry(const char *record, const RID *rid) override ;
    RC delete_entry(const vector<float> &vctor, const RID *rid);

    RC sync() override;
    IndexScanner *create_scanner(vector<Value> &left_key, bool left_inclusive, vector<Value> &right_key, bool right_inclusive) override;


private:
    Table *table_ = nullptr;;
    VectorIndexMeta index_meta_; // 索引元数据
    IVFFlatIndexHandler index_handler_;
    bool   inited_ = false;
    int    lists_  = 1;
    int    probes_ = 1;
};

