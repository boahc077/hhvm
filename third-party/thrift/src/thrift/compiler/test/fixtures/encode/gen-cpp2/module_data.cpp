/**
 * Autogenerated by Thrift for src/module.thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated @nocommit
 */

#include "thrift/compiler/test/fixtures/encode/gen-cpp2/module_data.h"

#include <thrift/lib/cpp2/gen/module_data_cpp.h>

namespace apache {
namespace thrift {

const std::array<::facebook::thrift::test::Enum, 2> TEnumDataStorage<::facebook::thrift::test::Enum>::values = {{
  type::first,
  type::second,
}};
const std::array<folly::StringPiece, 2> TEnumDataStorage<::facebook::thrift::test::Enum>::names = {{
  "first",
  "second",
}};

const std::array<folly::StringPiece, 1> TStructDataStorage<::facebook::thrift::test::Foo>::fields_names = {{
  "field",
}};
const std::array<int16_t, 1> TStructDataStorage<::facebook::thrift::test::Foo>::fields_ids = {{
  1,
}};
const std::array<protocol::TType, 1> TStructDataStorage<::facebook::thrift::test::Foo>::fields_types = {{
  TType::T_I32,
}};

const std::array<folly::StringPiece, 1> TStructDataStorage<::facebook::thrift::test::Bar>::fields_names = {{
  "list_field",
}};
const std::array<int16_t, 1> TStructDataStorage<::facebook::thrift::test::Bar>::fields_ids = {{
  1,
}};
const std::array<protocol::TType, 1> TStructDataStorage<::facebook::thrift::test::Bar>::fields_types = {{
  TType::T_LIST,
}};

const std::array<folly::StringPiece, 12> TStructDataStorage<::facebook::thrift::test::OpEncodeStruct>::fields_names = {{
  "int_field",
  "enum_field",
  "foo_field",
  "adapted_field",
  "list_field",
  "list_shared_ptr_field",
  "list_cpp_type_field",
  "set_field",
  "map_field",
  "nested_field",
  "bar_field",
  "adapted_list_field",
}};
const std::array<int16_t, 12> TStructDataStorage<::facebook::thrift::test::OpEncodeStruct>::fields_ids = {{
  1,
  2,
  3,
  4,
  5,
  6,
  7,
  8,
  9,
  10,
  11,
  12,
}};
const std::array<protocol::TType, 12> TStructDataStorage<::facebook::thrift::test::OpEncodeStruct>::fields_types = {{
  TType::T_I32,
  TType::T_I32,
  TType::T_STRUCT,
  TType::T_STRUCT,
  TType::T_LIST,
  TType::T_LIST,
  TType::T_LIST,
  TType::T_SET,
  TType::T_MAP,
  TType::T_MAP,
  TType::T_STRUCT,
  TType::T_LIST,
}};

} // namespace thrift
} // namespace apache
