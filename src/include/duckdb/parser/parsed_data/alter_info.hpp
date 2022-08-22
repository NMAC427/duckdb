//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/parsed_data/alter_info.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/parser/parsed_data/parse_info.hpp"
#include "duckdb/parser/column_definition.hpp"
#include "duckdb/common/enums/catalog_type.hpp"
#include "duckdb/parser/constraint.hpp"

namespace duckdb {

enum class AlterType : uint8_t {
	INVALID = 0,
	ALTER_SCHEMA = 1,
	ALTER_TABLE = 2,
	ALTER_VIEW = 3,
	ALTER_SEQUENCE = 4,
	CHANGE_OWNERSHIP = 5
};

struct AlterInfo : public ParseInfo {
	AlterInfo(AlterType type, string schema_p, string name_p);
	~AlterInfo() override;

	AlterType type;
	//! if exists
	bool if_exists;
	//! Schema name to alter
	string schema;
	//! Entry name to alter
	string name;

public:
	virtual CatalogType GetCatalogType() const = 0;
	virtual unique_ptr<AlterInfo> Copy() const = 0;
	void Serialize(Serializer &serializer) const;
	virtual void Serialize(FieldWriter &writer) const = 0;
	static unique_ptr<AlterInfo> Deserialize(Deserializer &source);
};

} // namespace duckdb
