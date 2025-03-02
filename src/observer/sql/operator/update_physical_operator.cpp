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
// Created by liwei on 2025/3/2.
//

#include "sql/operator/update_physical_operator.h"
#include "common/log/log.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"

// 更新操作，物理算子
RC UpdatePhysicalOperator::open(Trx *trx)
{
	if (children_.empty()) {
		return RC::SUCCESS;
	}

	std::unique_ptr<PhysicalOperator> &child = children_[0];

	RC rc = child->open(trx);
	if (rc != RC::SUCCESS) {
		LOG_WARN("failed to open child operator: %s", strrc(rc));
		return rc;
	}

	trx_ = trx;
  // 获取要 update 的所有记录
	while (OB_SUCC(rc = child->next())) {
		Tuple *tuple = child->current_tuple();
		if (nullptr == tuple) {
			LOG_WARN("failed to get current record: %s", strrc(rc));
			return rc;
		}

		RowTuple *row_tuple = static_cast<RowTuple *>(tuple);
		Record   &record    = row_tuple->record();
		records_.emplace_back(std::move(record));
	}

	child->close();

	// 先收集记录再删除
	// 记录的有效性由事务来保证，如果事务不保证删除的有效性，那说明此事务类型不支持并发控制，比如VacuousTrx
	// 构造record
	// 通过 attribuate_name_ 找到 filed； 将 value_ 数据插入指定位置; 然后写入存储
	const FieldMeta *field = table_->table_meta().field(attribuate_name_.c_str());
	if (nullptr == field) {
		LOG_WARN("no such field in table: table %s, field %s", table_->name(), attribuate_name_.c_str());
//		table = nullptr;
		return RC::SCHEMA_FIELD_NOT_EXIST;
	}

	for (Record &record : records_) {
		// 更新记录
		Record newRecord;
		newRecord.copy_data(record.data(), record.len());
		newRecord.set_rid(record.rid());

		size_t       copy_len = field->len();
		const size_t data_len = value_.length();
		if (field->type() == AttrType::CHARS) {
			// 如果record 空间该字段的空间大小更大， 则复制 value 数据长度+1
			// 如果record 空间该字段的空间大小更小，则复制空间长度
			if (copy_len > data_len) {
				copy_len = data_len + 1;
			}
		}
		newRecord.set_field(field->offset(), copy_len, const_cast<char *>(value_.data()));
		// 写入记录
		rc = trx_->update_record(table_, newRecord);
		if (rc != RC::SUCCESS) {
			LOG_WARN("failed to delete record: %s", strrc(rc));
			return rc;
		}
	}

	return RC::SUCCESS;
}

RC UpdatePhysicalOperator::next()
{
	return RC::RECORD_EOF;
}

RC UpdatePhysicalOperator::close()
{
	return RC::SUCCESS;
}
