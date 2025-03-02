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
// Created by Wangyunlai on 2022/5/22.
//

#include "sql/stmt/update_stmt.h"
#include "common/log/log.h"
#include "sql/stmt/filter_stmt.h"
#include "storage/db/db.h"
#include "storage/table/table.h"

UpdateStmt::UpdateStmt(Table *table, string attribute_name, Value value, FilterStmt *filter_stmt)
				: table_(table), attribute_name_(attribute_name), value_(value), filter_stmt_(filter_stmt)
{
}


UpdateStmt::~UpdateStmt(){
	if (nullptr != filter_stmt_) {
		delete filter_stmt_;
		filter_stmt_ = nullptr;
	}
}

//  update t1 set a = 3 where a = 2;
RC UpdateStmt::create(Db *db, const UpdateSqlNode &update, Stmt *&stmt)
{
  // TODO
  // 表名
	const char *table_name = update.relation_name.c_str();
	if (nullptr == db || nullptr == table_name) {
		LOG_WARN("invalid argument. db=%p, table_name=%p", db, table_name);
		return RC::INVALID_ARGUMENT;
	}

	// check whether the table exists
	Table *table = db->find_table(table_name);
	if (nullptr == table) {
		LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
		return RC::SCHEMA_TABLE_NOT_EXIST;
	}
	// table name -- > table
	std::unordered_map<std::string, Table *> table_map;
	table_map.insert(std::pair<std::string, Table *>(std::string(table_name), table));
  // where 条件
	FilterStmt *filter_stmt = nullptr;
	RC          rc          = FilterStmt::create(
					db, table, &table_map, update.conditions.data(), static_cast<int>(update.conditions.size()), filter_stmt);
	if (rc != RC::SUCCESS) {
		LOG_WARN("failed to create filter statement. rc=%d:%s", rc, strrc(rc));
		return rc;
	}

	// set a = 3
	// 根据列名获取列元数据
//	const FieldMeta *field = table->table_meta().field(update.attribute_name.c_str());
//	if (nullptr == field) {
//		LOG_WARN("no such field in table: table %s, field %s", table->name(), update.attribute_name.c_str());
//		table = nullptr;
//		return RC::SCHEMA_FIELD_NOT_EXIST;
//	}

	stmt = new UpdateStmt(table, update.attribute_name, update.value, filter_stmt);
	return rc;
}
const string & UpdateStmt::getAttributeName() const{
	return attribute_name_;
}
void UpdateStmt::setAttributeName(const string & attributeName){
	attribute_name_ = attributeName;
}

