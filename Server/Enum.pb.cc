// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Enum.proto

#include "Enum.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

namespace Protocol {
}  // namespace Protocol
static const ::_pb::EnumDescriptor* file_level_enum_descriptors_Enum_2eproto[7];
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_Enum_2eproto = nullptr;
const uint32_t TableStruct_Enum_2eproto::offsets[1] = {};
static constexpr ::_pbi::MigrationSchema* schemas = nullptr;
static constexpr ::_pb::Message* const* file_default_instances = nullptr;

const char descriptor_table_protodef_Enum_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\nEnum.proto\022\010Protocol*h\n\nPlayerType\022\024\n\020"
  "PLAYER_TYPE_NONE\020\000\022\026\n\022PLAYER_TYPE_KNIGHT"
  "\020\001\022\024\n\020PLAYER_TYPE_MAGE\020\002\022\026\n\022PLAYER_TYPE_"
  "ARCHER\020\003*\265\001\n\013OBJECT_TYPE\022\024\n\020OBJECT_TYPE_"
  "NONE\020\000\022\026\n\022OBJECT_TYPE_PLAYER\020\001\022\027\n\023OBJECT"
  "_TYPE_MONSTER\020\002\022\032\n\026OBJECT_TYPE_PROJECTIL"
  "E\020\003\022\023\n\017OBJECT_TYPE_NPC\020\004\022\030\n\024OBJECT_TYPE_"
  "NPC_SIGN\020\005\022\024\n\020OBJECT_TYPE_ITEM\020\006*\231\002\n\021OBJ"
  "ECT_STATE_TYPE\022\032\n\026OBJECT_STATE_TYPE_IDLE"
  "\020\000\022\032\n\026OBJECT_STATE_TYPE_MOVE\020\001\022\033\n\027OBJECT"
  "_STATE_TYPE_SKILL\020\002\022\031\n\025OBJECT_STATE_TYPE"
  "_HIT\020\003\022\032\n\026OBJECT_STATE_TYPE_STOP\020\004\022\032\n\026OB"
  "JECT_STATE_TYPE_CHAT\020\005\022 \n\034OBJECT_STATE_T"
  "YPE_SPIN_READY\020\006\022\032\n\026OBJECT_STATE_TYPE_SP"
  "IN\020\007\022\036\n\032OBJECT_STATE_TYPE_TELEPORT\020\010*U\n\010"
  "DIR_TYPE\022\017\n\013DIR_TYPE_UP\020\000\022\021\n\rDIR_TYPE_DO"
  "WN\020\001\022\021\n\rDIR_TYPE_LEFT\020\002\022\022\n\016DIR_TYPE_RIGH"
  "T\020\003*P\n\013WEAPON_TYPE\022\025\n\021WEAPON_TYPE_SWORD\020"
  "\000\022\023\n\017WEAPON_TYPE_BOW\020\001\022\025\n\021WEAPON_TYPE_ST"
  "AFF\020\002*z\n\tITEM_TYPE\022\022\n\016ITEM_TYPE_NONE\020\000\022\023"
  "\n\017ITEM_TYPE_HEART\020\001\022\027\n\023ITEM_TYPE_FULLHEA"
  "RT\020\002\022\026\n\022ITEM_TYPE_MAXHEART\020\003\022\023\n\017ITEM_TYP"
  "E_ARROW\020\004*4\n\010NPC_TYPE\022\021\n\rNPC_TYPE_SIGN\020\000"
  "\022\025\n\021NPC_TYPE_MERCHANT\020\001b\006proto3"
  ;
static ::_pbi::once_flag descriptor_table_Enum_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_Enum_2eproto = {
    false, false, 951, descriptor_table_protodef_Enum_2eproto,
    "Enum.proto",
    &descriptor_table_Enum_2eproto_once, nullptr, 0, 0,
    schemas, file_default_instances, TableStruct_Enum_2eproto::offsets,
    nullptr, file_level_enum_descriptors_Enum_2eproto,
    file_level_service_descriptors_Enum_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_Enum_2eproto_getter() {
  return &descriptor_table_Enum_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_Enum_2eproto(&descriptor_table_Enum_2eproto);
namespace Protocol {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* PlayerType_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_Enum_2eproto);
  return file_level_enum_descriptors_Enum_2eproto[0];
}
bool PlayerType_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* OBJECT_TYPE_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_Enum_2eproto);
  return file_level_enum_descriptors_Enum_2eproto[1];
}
bool OBJECT_TYPE_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* OBJECT_STATE_TYPE_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_Enum_2eproto);
  return file_level_enum_descriptors_Enum_2eproto[2];
}
bool OBJECT_STATE_TYPE_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* DIR_TYPE_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_Enum_2eproto);
  return file_level_enum_descriptors_Enum_2eproto[3];
}
bool DIR_TYPE_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* WEAPON_TYPE_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_Enum_2eproto);
  return file_level_enum_descriptors_Enum_2eproto[4];
}
bool WEAPON_TYPE_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* ITEM_TYPE_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_Enum_2eproto);
  return file_level_enum_descriptors_Enum_2eproto[5];
}
bool ITEM_TYPE_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* NPC_TYPE_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_Enum_2eproto);
  return file_level_enum_descriptors_Enum_2eproto[6];
}
bool NPC_TYPE_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace Protocol
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
