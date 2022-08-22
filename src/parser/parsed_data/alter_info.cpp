#include "duckdb/parser/parsed_data/alter_info.hpp"
#include "duckdb/parser/parsed_data/alter_schema_info.hpp"
#include "duckdb/parser/parsed_data/alter_table_info.hpp"
#include "duckdb/common/field_writer.hpp"
#include "duckdb/parser/constraint.hpp"

namespace duckdb {

AlterInfo::AlterInfo(AlterType type, string schema_p, string name_p)
    : type(type), if_exists(false), schema(move(schema_p)), name(move(name_p)) {
}

AlterInfo::~AlterInfo() {
}

void AlterInfo::Serialize(Serializer &serializer) const {
	FieldWriter writer(serializer);
	writer.WriteField<AlterType>(type);
	Serialize(writer);
	writer.Finalize();
}

unique_ptr<AlterInfo> AlterInfo::Deserialize(Deserializer &source) {
	FieldReader reader(source);
	auto type = reader.ReadRequired<AlterType>();

	unique_ptr<AlterInfo> result;
	switch (type) {
	case AlterType::ALTER_SCHEMA:
		result = AlterSchemaInfo::Deserialize(reader);
		break;
	case AlterType::ALTER_TABLE:
		result = AlterTableInfo::Deserialize(reader);
		break;
	case AlterType::ALTER_VIEW:
		result = AlterViewInfo::Deserialize(reader);
		break;
	default:
		throw SerializationException("Unknown alter type for deserialization!");
	}
	reader.Finalize();

	return result;
}
} // namespace duckdb
