//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/parsed_data/alter_schema_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/parser/parsed_data/alter_info.hpp"
#include "duckdb/parser/column_definition.hpp"
#include "duckdb/common/enums/catalog_type.hpp"
#include "duckdb/parser/constraint.hpp"

namespace duckdb {

//===--------------------------------------------------------------------===//
// Alter Schema
//===--------------------------------------------------------------------===//
enum class AlterSchemaType : uint8_t { INVALID = 0, RENAME_SCHEMA = 1 };

struct AlterSchemaInfo : public AlterInfo {
	AlterSchemaInfo(AlterSchemaType type, string schema_p);
	~AlterSchemaInfo() override;

	AlterSchemaType alter_schema_type;

public:
	CatalogType GetCatalogType() const override;
	void Serialize(FieldWriter &writer) const override;
	virtual void SerializeAlterSchema(FieldWriter &writer) const = 0;
	static unique_ptr<AlterInfo> Deserialize(FieldReader &reader);
};

struct RenameSchemaInfo : public AlterSchemaInfo {
	RenameSchemaInfo(string schema, string new_name);
	~RenameSchemaInfo() override;

	//! Schema new name
	string new_schema_name;

public:
	unique_ptr<AlterInfo> Copy() const override;
	void SerializeAlterSchema(FieldWriter &writer) const override;
	static unique_ptr<AlterInfo> Deserialize(FieldReader &reader, string schema);
};

} // namespace duckdb
