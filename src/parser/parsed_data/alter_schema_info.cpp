#include "duckdb/parser/parsed_data/alter_schema_info.hpp"
#include "duckdb/common/field_writer.hpp"
#include "duckdb/parser/constraint.hpp"

namespace duckdb {

//===--------------------------------------------------------------------===//
// AlterSchemaInfo
//===--------------------------------------------------------------------===//

AlterSchemaInfo::AlterSchemaInfo(AlterSchemaType type, string schema_p)
    : AlterInfo(AlterType::ALTER_SCHEMA, move(schema_p), move(schema_p)), alter_schema_type(type) {
}
AlterSchemaInfo::~AlterSchemaInfo() {
}

CatalogType AlterSchemaInfo::GetCatalogType() const {
	return CatalogType::SCHEMA_ENTRY;
}

void AlterSchemaInfo::Serialize(FieldWriter &writer) const {
	writer.WriteField<AlterSchemaType>(alter_schema_type);
	writer.WriteString(schema);
	writer.WriteString(name);
	SerializeAlterSchema(writer);
}

unique_ptr<AlterInfo> AlterSchemaInfo::Deserialize(FieldReader &reader) {
	auto type = reader.ReadRequired<AlterSchemaType>();
	auto schema = reader.ReadRequired<string>();
	auto name = reader.ReadRequired<string>();
	unique_ptr<AlterSchemaInfo> info;
	switch (type) {
	case AlterSchemaType::RENAME_SCHEMA:
		return RenameSchemaInfo::Deserialize(reader, schema);
	default:
		throw SerializationException("Unknown alter table type for deserialization!");
	}
}

//===--------------------------------------------------------------------===//
// RenameSchemaInfo
//===--------------------------------------------------------------------===//

RenameSchemaInfo::RenameSchemaInfo(string schema_p, string new_name_p)
    : AlterSchemaInfo(AlterSchemaType::RENAME_SCHEMA, move(schema_p)), new_schema_name(move(new_name_p)) {
}
RenameSchemaInfo::~RenameSchemaInfo() {
}

unique_ptr<AlterInfo> RenameSchemaInfo::Copy() const {
	return make_unique_base<AlterInfo, RenameSchemaInfo>(schema, new_schema_name);
}

void RenameSchemaInfo::SerializeAlterSchema(FieldWriter &writer) const {
	writer.WriteString(new_schema_name);
}

unique_ptr<AlterInfo> RenameSchemaInfo::Deserialize(FieldReader &reader, string schema) {
	auto new_name = reader.ReadRequired<string>();
	return make_unique<RenameSchemaInfo>(move(schema), new_name);
}

} // namespace duckdb
