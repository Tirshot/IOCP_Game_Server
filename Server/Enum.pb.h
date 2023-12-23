// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Enum.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_Enum_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_Enum_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3021000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3021012 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_Enum_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_Enum_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_Enum_2eproto;
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE
namespace Protocol {

enum PlayerType : int {
  PLAYER_TYPE_NONE = 0,
  PLAYER_TYPE_KNIGHT = 1,
  PLAYER_TYPE_MAGE = 2,
  PLAYER_TYPE_ARCHER = 3,
  PlayerType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  PlayerType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool PlayerType_IsValid(int value);
constexpr PlayerType PlayerType_MIN = PLAYER_TYPE_NONE;
constexpr PlayerType PlayerType_MAX = PLAYER_TYPE_ARCHER;
constexpr int PlayerType_ARRAYSIZE = PlayerType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* PlayerType_descriptor();
template<typename T>
inline const std::string& PlayerType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, PlayerType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function PlayerType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    PlayerType_descriptor(), enum_t_value);
}
inline bool PlayerType_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, PlayerType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<PlayerType>(
    PlayerType_descriptor(), name, value);
}
enum OBJECT_TYPE : int {
  OBJECT_TYPE_NONE = 0,
  OBJECT_TYPE_PLAYER = 1,
  OBJECT_TYPE_MONSTER = 2,
  OBJECT_TYPE_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  OBJECT_TYPE_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool OBJECT_TYPE_IsValid(int value);
constexpr OBJECT_TYPE OBJECT_TYPE_MIN = OBJECT_TYPE_NONE;
constexpr OBJECT_TYPE OBJECT_TYPE_MAX = OBJECT_TYPE_MONSTER;
constexpr int OBJECT_TYPE_ARRAYSIZE = OBJECT_TYPE_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* OBJECT_TYPE_descriptor();
template<typename T>
inline const std::string& OBJECT_TYPE_Name(T enum_t_value) {
  static_assert(::std::is_same<T, OBJECT_TYPE>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function OBJECT_TYPE_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    OBJECT_TYPE_descriptor(), enum_t_value);
}
inline bool OBJECT_TYPE_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, OBJECT_TYPE* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<OBJECT_TYPE>(
    OBJECT_TYPE_descriptor(), name, value);
}
enum OBJECT_STATE_TYPE : int {
  OBJECT_STATE_TYPE_IDLE = 0,
  OBJECT_STATE_TYPE_MOVE = 1,
  OBJECT_STATE_TYPE_SKILL = 2,
  OBJECT_STATE_TYPE_HIT = 3,
  OBJECT_STATE_TYPE_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  OBJECT_STATE_TYPE_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool OBJECT_STATE_TYPE_IsValid(int value);
constexpr OBJECT_STATE_TYPE OBJECT_STATE_TYPE_MIN = OBJECT_STATE_TYPE_IDLE;
constexpr OBJECT_STATE_TYPE OBJECT_STATE_TYPE_MAX = OBJECT_STATE_TYPE_HIT;
constexpr int OBJECT_STATE_TYPE_ARRAYSIZE = OBJECT_STATE_TYPE_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* OBJECT_STATE_TYPE_descriptor();
template<typename T>
inline const std::string& OBJECT_STATE_TYPE_Name(T enum_t_value) {
  static_assert(::std::is_same<T, OBJECT_STATE_TYPE>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function OBJECT_STATE_TYPE_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    OBJECT_STATE_TYPE_descriptor(), enum_t_value);
}
inline bool OBJECT_STATE_TYPE_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, OBJECT_STATE_TYPE* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<OBJECT_STATE_TYPE>(
    OBJECT_STATE_TYPE_descriptor(), name, value);
}
enum DIR_TYPE : int {
  DIR_TYPE_UP = 0,
  DIR_TYPE_DOWN = 1,
  DIR_TYPE_LEFT = 2,
  DIR_TYPE_RIGHT = 3,
  DIR_TYPE_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  DIR_TYPE_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool DIR_TYPE_IsValid(int value);
constexpr DIR_TYPE DIR_TYPE_MIN = DIR_TYPE_UP;
constexpr DIR_TYPE DIR_TYPE_MAX = DIR_TYPE_RIGHT;
constexpr int DIR_TYPE_ARRAYSIZE = DIR_TYPE_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* DIR_TYPE_descriptor();
template<typename T>
inline const std::string& DIR_TYPE_Name(T enum_t_value) {
  static_assert(::std::is_same<T, DIR_TYPE>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function DIR_TYPE_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    DIR_TYPE_descriptor(), enum_t_value);
}
inline bool DIR_TYPE_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, DIR_TYPE* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<DIR_TYPE>(
    DIR_TYPE_descriptor(), name, value);
}
enum WEAPON_TYPE : int {
  WEAPON_TYPE_SWORD = 0,
  WEAPON_TYPE_BOW = 1,
  WEAPON_TYPE_STAFF = 2,
  WEAPON_TYPE_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  WEAPON_TYPE_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool WEAPON_TYPE_IsValid(int value);
constexpr WEAPON_TYPE WEAPON_TYPE_MIN = WEAPON_TYPE_SWORD;
constexpr WEAPON_TYPE WEAPON_TYPE_MAX = WEAPON_TYPE_STAFF;
constexpr int WEAPON_TYPE_ARRAYSIZE = WEAPON_TYPE_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* WEAPON_TYPE_descriptor();
template<typename T>
inline const std::string& WEAPON_TYPE_Name(T enum_t_value) {
  static_assert(::std::is_same<T, WEAPON_TYPE>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function WEAPON_TYPE_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    WEAPON_TYPE_descriptor(), enum_t_value);
}
inline bool WEAPON_TYPE_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, WEAPON_TYPE* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<WEAPON_TYPE>(
    WEAPON_TYPE_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace Protocol

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::Protocol::PlayerType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Protocol::PlayerType>() {
  return ::Protocol::PlayerType_descriptor();
}
template <> struct is_proto_enum< ::Protocol::OBJECT_TYPE> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Protocol::OBJECT_TYPE>() {
  return ::Protocol::OBJECT_TYPE_descriptor();
}
template <> struct is_proto_enum< ::Protocol::OBJECT_STATE_TYPE> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Protocol::OBJECT_STATE_TYPE>() {
  return ::Protocol::OBJECT_STATE_TYPE_descriptor();
}
template <> struct is_proto_enum< ::Protocol::DIR_TYPE> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Protocol::DIR_TYPE>() {
  return ::Protocol::DIR_TYPE_descriptor();
}
template <> struct is_proto_enum< ::Protocol::WEAPON_TYPE> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Protocol::WEAPON_TYPE>() {
  return ::Protocol::WEAPON_TYPE_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_Enum_2eproto
