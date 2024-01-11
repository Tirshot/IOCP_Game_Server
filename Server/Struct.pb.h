// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Struct.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_Struct_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_Struct_2eproto

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
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
#include "Enum.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_Struct_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_Struct_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_Struct_2eproto;
namespace Protocol {
class BuffData;
struct BuffDataDefaultTypeInternal;
extern BuffDataDefaultTypeInternal _BuffData_default_instance_;
class ObjectInfo;
struct ObjectInfoDefaultTypeInternal;
extern ObjectInfoDefaultTypeInternal _ObjectInfo_default_instance_;
class Text;
struct TextDefaultTypeInternal;
extern TextDefaultTypeInternal _Text_default_instance_;
}  // namespace Protocol
PROTOBUF_NAMESPACE_OPEN
template<> ::Protocol::BuffData* Arena::CreateMaybeMessage<::Protocol::BuffData>(Arena*);
template<> ::Protocol::ObjectInfo* Arena::CreateMaybeMessage<::Protocol::ObjectInfo>(Arena*);
template<> ::Protocol::Text* Arena::CreateMaybeMessage<::Protocol::Text>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace Protocol {

// ===================================================================

class BuffData final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:Protocol.BuffData) */ {
 public:
  inline BuffData() : BuffData(nullptr) {}
  ~BuffData() override;
  explicit PROTOBUF_CONSTEXPR BuffData(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  BuffData(const BuffData& from);
  BuffData(BuffData&& from) noexcept
    : BuffData() {
    *this = ::std::move(from);
  }

  inline BuffData& operator=(const BuffData& from) {
    CopyFrom(from);
    return *this;
  }
  inline BuffData& operator=(BuffData&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const BuffData& default_instance() {
    return *internal_default_instance();
  }
  static inline const BuffData* internal_default_instance() {
    return reinterpret_cast<const BuffData*>(
               &_BuffData_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(BuffData& a, BuffData& b) {
    a.Swap(&b);
  }
  inline void Swap(BuffData* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(BuffData* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  BuffData* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<BuffData>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const BuffData& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const BuffData& from) {
    BuffData::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(BuffData* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "Protocol.BuffData";
  }
  protected:
  explicit BuffData(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kVictimsFieldNumber = 3,
    kBuffIdFieldNumber = 1,
    kRemainTimeFieldNumber = 2,
  };
  // repeated uint64 victims = 3;
  int victims_size() const;
  private:
  int _internal_victims_size() const;
  public:
  void clear_victims();
  private:
  uint64_t _internal_victims(int index) const;
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< uint64_t >&
      _internal_victims() const;
  void _internal_add_victims(uint64_t value);
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< uint64_t >*
      _internal_mutable_victims();
  public:
  uint64_t victims(int index) const;
  void set_victims(int index, uint64_t value);
  void add_victims(uint64_t value);
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< uint64_t >&
      victims() const;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< uint64_t >*
      mutable_victims();

  // uint64 buffId = 1;
  void clear_buffid();
  uint64_t buffid() const;
  void set_buffid(uint64_t value);
  private:
  uint64_t _internal_buffid() const;
  void _internal_set_buffid(uint64_t value);
  public:

  // float remainTime = 2;
  void clear_remaintime();
  float remaintime() const;
  void set_remaintime(float value);
  private:
  float _internal_remaintime() const;
  void _internal_set_remaintime(float value);
  public:

  // @@protoc_insertion_point(class_scope:Protocol.BuffData)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::RepeatedField< uint64_t > victims_;
    mutable std::atomic<int> _victims_cached_byte_size_;
    uint64_t buffid_;
    float remaintime_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_Struct_2eproto;
};
// -------------------------------------------------------------------

class ObjectInfo final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:Protocol.ObjectInfo) */ {
 public:
  inline ObjectInfo() : ObjectInfo(nullptr) {}
  ~ObjectInfo() override;
  explicit PROTOBUF_CONSTEXPR ObjectInfo(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  ObjectInfo(const ObjectInfo& from);
  ObjectInfo(ObjectInfo&& from) noexcept
    : ObjectInfo() {
    *this = ::std::move(from);
  }

  inline ObjectInfo& operator=(const ObjectInfo& from) {
    CopyFrom(from);
    return *this;
  }
  inline ObjectInfo& operator=(ObjectInfo&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const ObjectInfo& default_instance() {
    return *internal_default_instance();
  }
  static inline const ObjectInfo* internal_default_instance() {
    return reinterpret_cast<const ObjectInfo*>(
               &_ObjectInfo_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(ObjectInfo& a, ObjectInfo& b) {
    a.Swap(&b);
  }
  inline void Swap(ObjectInfo* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(ObjectInfo* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  ObjectInfo* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<ObjectInfo>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const ObjectInfo& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const ObjectInfo& from) {
    ObjectInfo::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(ObjectInfo* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "Protocol.ObjectInfo";
  }
  protected:
  explicit ObjectInfo(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kNameFieldNumber = 5,
    kObjectIdFieldNumber = 1,
    kObjectTypeFieldNumber = 2,
    kStateFieldNumber = 3,
    kDirFieldNumber = 4,
    kHpFieldNumber = 6,
    kMaxHpFieldNumber = 7,
    kAttackFieldNumber = 8,
    kDefenceFieldNumber = 9,
    kPosXFieldNumber = 10,
    kPosYFieldNumber = 11,
    kDamageFieldNumber = 12,
    kGoldFieldNumber = 14,
    kWeaponTypeFieldNumber = 13,
    kItemTypeFieldNumber = 15,
  };
  // string name = 5;
  void clear_name();
  const std::string& name() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_name(ArgT0&& arg0, ArgT... args);
  std::string* mutable_name();
  PROTOBUF_NODISCARD std::string* release_name();
  void set_allocated_name(std::string* name);
  private:
  const std::string& _internal_name() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_name(const std::string& value);
  std::string* _internal_mutable_name();
  public:

  // uint64 objectId = 1;
  void clear_objectid();
  uint64_t objectid() const;
  void set_objectid(uint64_t value);
  private:
  uint64_t _internal_objectid() const;
  void _internal_set_objectid(uint64_t value);
  public:

  // .Protocol.OBJECT_TYPE objectType = 2;
  void clear_objecttype();
  ::Protocol::OBJECT_TYPE objecttype() const;
  void set_objecttype(::Protocol::OBJECT_TYPE value);
  private:
  ::Protocol::OBJECT_TYPE _internal_objecttype() const;
  void _internal_set_objecttype(::Protocol::OBJECT_TYPE value);
  public:

  // .Protocol.OBJECT_STATE_TYPE state = 3;
  void clear_state();
  ::Protocol::OBJECT_STATE_TYPE state() const;
  void set_state(::Protocol::OBJECT_STATE_TYPE value);
  private:
  ::Protocol::OBJECT_STATE_TYPE _internal_state() const;
  void _internal_set_state(::Protocol::OBJECT_STATE_TYPE value);
  public:

  // .Protocol.DIR_TYPE dir = 4;
  void clear_dir();
  ::Protocol::DIR_TYPE dir() const;
  void set_dir(::Protocol::DIR_TYPE value);
  private:
  ::Protocol::DIR_TYPE _internal_dir() const;
  void _internal_set_dir(::Protocol::DIR_TYPE value);
  public:

  // int32 hp = 6;
  void clear_hp();
  int32_t hp() const;
  void set_hp(int32_t value);
  private:
  int32_t _internal_hp() const;
  void _internal_set_hp(int32_t value);
  public:

  // int32 maxHp = 7;
  void clear_maxhp();
  int32_t maxhp() const;
  void set_maxhp(int32_t value);
  private:
  int32_t _internal_maxhp() const;
  void _internal_set_maxhp(int32_t value);
  public:

  // int32 attack = 8;
  void clear_attack();
  int32_t attack() const;
  void set_attack(int32_t value);
  private:
  int32_t _internal_attack() const;
  void _internal_set_attack(int32_t value);
  public:

  // int32 defence = 9;
  void clear_defence();
  int32_t defence() const;
  void set_defence(int32_t value);
  private:
  int32_t _internal_defence() const;
  void _internal_set_defence(int32_t value);
  public:

  // int32 posX = 10;
  void clear_posx();
  int32_t posx() const;
  void set_posx(int32_t value);
  private:
  int32_t _internal_posx() const;
  void _internal_set_posx(int32_t value);
  public:

  // int32 posY = 11;
  void clear_posy();
  int32_t posy() const;
  void set_posy(int32_t value);
  private:
  int32_t _internal_posy() const;
  void _internal_set_posy(int32_t value);
  public:

  // int32 damage = 12;
  void clear_damage();
  int32_t damage() const;
  void set_damage(int32_t value);
  private:
  int32_t _internal_damage() const;
  void _internal_set_damage(int32_t value);
  public:

  // uint64 gold = 14;
  void clear_gold();
  uint64_t gold() const;
  void set_gold(uint64_t value);
  private:
  uint64_t _internal_gold() const;
  void _internal_set_gold(uint64_t value);
  public:

  // .Protocol.WEAPON_TYPE weaponType = 13;
  void clear_weapontype();
  ::Protocol::WEAPON_TYPE weapontype() const;
  void set_weapontype(::Protocol::WEAPON_TYPE value);
  private:
  ::Protocol::WEAPON_TYPE _internal_weapontype() const;
  void _internal_set_weapontype(::Protocol::WEAPON_TYPE value);
  public:

  // .Protocol.ITEM_TYPE itemType = 15;
  void clear_itemtype();
  ::Protocol::ITEM_TYPE itemtype() const;
  void set_itemtype(::Protocol::ITEM_TYPE value);
  private:
  ::Protocol::ITEM_TYPE _internal_itemtype() const;
  void _internal_set_itemtype(::Protocol::ITEM_TYPE value);
  public:

  // @@protoc_insertion_point(class_scope:Protocol.ObjectInfo)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr name_;
    uint64_t objectid_;
    int objecttype_;
    int state_;
    int dir_;
    int32_t hp_;
    int32_t maxhp_;
    int32_t attack_;
    int32_t defence_;
    int32_t posx_;
    int32_t posy_;
    int32_t damage_;
    uint64_t gold_;
    int weapontype_;
    int itemtype_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_Struct_2eproto;
};
// -------------------------------------------------------------------

class Text final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:Protocol.Text) */ {
 public:
  inline Text() : Text(nullptr) {}
  ~Text() override;
  explicit PROTOBUF_CONSTEXPR Text(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  Text(const Text& from);
  Text(Text&& from) noexcept
    : Text() {
    *this = ::std::move(from);
  }

  inline Text& operator=(const Text& from) {
    CopyFrom(from);
    return *this;
  }
  inline Text& operator=(Text&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const Text& default_instance() {
    return *internal_default_instance();
  }
  static inline const Text* internal_default_instance() {
    return reinterpret_cast<const Text*>(
               &_Text_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(Text& a, Text& b) {
    a.Swap(&b);
  }
  inline void Swap(Text* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(Text* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  Text* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<Text>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const Text& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const Text& from) {
    Text::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(Text* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "Protocol.Text";
  }
  protected:
  explicit Text(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kStrFieldNumber = 3,
    kObjectIdFieldNumber = 1,
    kTimeFieldNumber = 2,
  };
  // string str = 3;
  void clear_str();
  const std::string& str() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_str(ArgT0&& arg0, ArgT... args);
  std::string* mutable_str();
  PROTOBUF_NODISCARD std::string* release_str();
  void set_allocated_str(std::string* str);
  private:
  const std::string& _internal_str() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_str(const std::string& value);
  std::string* _internal_mutable_str();
  public:

  // uint64 objectId = 1;
  void clear_objectid();
  uint64_t objectid() const;
  void set_objectid(uint64_t value);
  private:
  uint64_t _internal_objectid() const;
  void _internal_set_objectid(uint64_t value);
  public:

  // uint64 time = 2;
  void clear_time();
  uint64_t time() const;
  void set_time(uint64_t value);
  private:
  uint64_t _internal_time() const;
  void _internal_set_time(uint64_t value);
  public:

  // @@protoc_insertion_point(class_scope:Protocol.Text)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr str_;
    uint64_t objectid_;
    uint64_t time_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_Struct_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// BuffData

// uint64 buffId = 1;
inline void BuffData::clear_buffid() {
  _impl_.buffid_ = uint64_t{0u};
}
inline uint64_t BuffData::_internal_buffid() const {
  return _impl_.buffid_;
}
inline uint64_t BuffData::buffid() const {
  // @@protoc_insertion_point(field_get:Protocol.BuffData.buffId)
  return _internal_buffid();
}
inline void BuffData::_internal_set_buffid(uint64_t value) {
  
  _impl_.buffid_ = value;
}
inline void BuffData::set_buffid(uint64_t value) {
  _internal_set_buffid(value);
  // @@protoc_insertion_point(field_set:Protocol.BuffData.buffId)
}

// float remainTime = 2;
inline void BuffData::clear_remaintime() {
  _impl_.remaintime_ = 0;
}
inline float BuffData::_internal_remaintime() const {
  return _impl_.remaintime_;
}
inline float BuffData::remaintime() const {
  // @@protoc_insertion_point(field_get:Protocol.BuffData.remainTime)
  return _internal_remaintime();
}
inline void BuffData::_internal_set_remaintime(float value) {
  
  _impl_.remaintime_ = value;
}
inline void BuffData::set_remaintime(float value) {
  _internal_set_remaintime(value);
  // @@protoc_insertion_point(field_set:Protocol.BuffData.remainTime)
}

// repeated uint64 victims = 3;
inline int BuffData::_internal_victims_size() const {
  return _impl_.victims_.size();
}
inline int BuffData::victims_size() const {
  return _internal_victims_size();
}
inline void BuffData::clear_victims() {
  _impl_.victims_.Clear();
}
inline uint64_t BuffData::_internal_victims(int index) const {
  return _impl_.victims_.Get(index);
}
inline uint64_t BuffData::victims(int index) const {
  // @@protoc_insertion_point(field_get:Protocol.BuffData.victims)
  return _internal_victims(index);
}
inline void BuffData::set_victims(int index, uint64_t value) {
  _impl_.victims_.Set(index, value);
  // @@protoc_insertion_point(field_set:Protocol.BuffData.victims)
}
inline void BuffData::_internal_add_victims(uint64_t value) {
  _impl_.victims_.Add(value);
}
inline void BuffData::add_victims(uint64_t value) {
  _internal_add_victims(value);
  // @@protoc_insertion_point(field_add:Protocol.BuffData.victims)
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< uint64_t >&
BuffData::_internal_victims() const {
  return _impl_.victims_;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< uint64_t >&
BuffData::victims() const {
  // @@protoc_insertion_point(field_list:Protocol.BuffData.victims)
  return _internal_victims();
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< uint64_t >*
BuffData::_internal_mutable_victims() {
  return &_impl_.victims_;
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< uint64_t >*
BuffData::mutable_victims() {
  // @@protoc_insertion_point(field_mutable_list:Protocol.BuffData.victims)
  return _internal_mutable_victims();
}

// -------------------------------------------------------------------

// ObjectInfo

// uint64 objectId = 1;
inline void ObjectInfo::clear_objectid() {
  _impl_.objectid_ = uint64_t{0u};
}
inline uint64_t ObjectInfo::_internal_objectid() const {
  return _impl_.objectid_;
}
inline uint64_t ObjectInfo::objectid() const {
  // @@protoc_insertion_point(field_get:Protocol.ObjectInfo.objectId)
  return _internal_objectid();
}
inline void ObjectInfo::_internal_set_objectid(uint64_t value) {
  
  _impl_.objectid_ = value;
}
inline void ObjectInfo::set_objectid(uint64_t value) {
  _internal_set_objectid(value);
  // @@protoc_insertion_point(field_set:Protocol.ObjectInfo.objectId)
}

// .Protocol.OBJECT_TYPE objectType = 2;
inline void ObjectInfo::clear_objecttype() {
  _impl_.objecttype_ = 0;
}
inline ::Protocol::OBJECT_TYPE ObjectInfo::_internal_objecttype() const {
  return static_cast< ::Protocol::OBJECT_TYPE >(_impl_.objecttype_);
}
inline ::Protocol::OBJECT_TYPE ObjectInfo::objecttype() const {
  // @@protoc_insertion_point(field_get:Protocol.ObjectInfo.objectType)
  return _internal_objecttype();
}
inline void ObjectInfo::_internal_set_objecttype(::Protocol::OBJECT_TYPE value) {
  
  _impl_.objecttype_ = value;
}
inline void ObjectInfo::set_objecttype(::Protocol::OBJECT_TYPE value) {
  _internal_set_objecttype(value);
  // @@protoc_insertion_point(field_set:Protocol.ObjectInfo.objectType)
}

// .Protocol.OBJECT_STATE_TYPE state = 3;
inline void ObjectInfo::clear_state() {
  _impl_.state_ = 0;
}
inline ::Protocol::OBJECT_STATE_TYPE ObjectInfo::_internal_state() const {
  return static_cast< ::Protocol::OBJECT_STATE_TYPE >(_impl_.state_);
}
inline ::Protocol::OBJECT_STATE_TYPE ObjectInfo::state() const {
  // @@protoc_insertion_point(field_get:Protocol.ObjectInfo.state)
  return _internal_state();
}
inline void ObjectInfo::_internal_set_state(::Protocol::OBJECT_STATE_TYPE value) {
  
  _impl_.state_ = value;
}
inline void ObjectInfo::set_state(::Protocol::OBJECT_STATE_TYPE value) {
  _internal_set_state(value);
  // @@protoc_insertion_point(field_set:Protocol.ObjectInfo.state)
}

// .Protocol.DIR_TYPE dir = 4;
inline void ObjectInfo::clear_dir() {
  _impl_.dir_ = 0;
}
inline ::Protocol::DIR_TYPE ObjectInfo::_internal_dir() const {
  return static_cast< ::Protocol::DIR_TYPE >(_impl_.dir_);
}
inline ::Protocol::DIR_TYPE ObjectInfo::dir() const {
  // @@protoc_insertion_point(field_get:Protocol.ObjectInfo.dir)
  return _internal_dir();
}
inline void ObjectInfo::_internal_set_dir(::Protocol::DIR_TYPE value) {
  
  _impl_.dir_ = value;
}
inline void ObjectInfo::set_dir(::Protocol::DIR_TYPE value) {
  _internal_set_dir(value);
  // @@protoc_insertion_point(field_set:Protocol.ObjectInfo.dir)
}

// string name = 5;
inline void ObjectInfo::clear_name() {
  _impl_.name_.ClearToEmpty();
}
inline const std::string& ObjectInfo::name() const {
  // @@protoc_insertion_point(field_get:Protocol.ObjectInfo.name)
  return _internal_name();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void ObjectInfo::set_name(ArgT0&& arg0, ArgT... args) {
 
 _impl_.name_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:Protocol.ObjectInfo.name)
}
inline std::string* ObjectInfo::mutable_name() {
  std::string* _s = _internal_mutable_name();
  // @@protoc_insertion_point(field_mutable:Protocol.ObjectInfo.name)
  return _s;
}
inline const std::string& ObjectInfo::_internal_name() const {
  return _impl_.name_.Get();
}
inline void ObjectInfo::_internal_set_name(const std::string& value) {
  
  _impl_.name_.Set(value, GetArenaForAllocation());
}
inline std::string* ObjectInfo::_internal_mutable_name() {
  
  return _impl_.name_.Mutable(GetArenaForAllocation());
}
inline std::string* ObjectInfo::release_name() {
  // @@protoc_insertion_point(field_release:Protocol.ObjectInfo.name)
  return _impl_.name_.Release();
}
inline void ObjectInfo::set_allocated_name(std::string* name) {
  if (name != nullptr) {
    
  } else {
    
  }
  _impl_.name_.SetAllocated(name, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.name_.IsDefault()) {
    _impl_.name_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:Protocol.ObjectInfo.name)
}

// int32 hp = 6;
inline void ObjectInfo::clear_hp() {
  _impl_.hp_ = 0;
}
inline int32_t ObjectInfo::_internal_hp() const {
  return _impl_.hp_;
}
inline int32_t ObjectInfo::hp() const {
  // @@protoc_insertion_point(field_get:Protocol.ObjectInfo.hp)
  return _internal_hp();
}
inline void ObjectInfo::_internal_set_hp(int32_t value) {
  
  _impl_.hp_ = value;
}
inline void ObjectInfo::set_hp(int32_t value) {
  _internal_set_hp(value);
  // @@protoc_insertion_point(field_set:Protocol.ObjectInfo.hp)
}

// int32 maxHp = 7;
inline void ObjectInfo::clear_maxhp() {
  _impl_.maxhp_ = 0;
}
inline int32_t ObjectInfo::_internal_maxhp() const {
  return _impl_.maxhp_;
}
inline int32_t ObjectInfo::maxhp() const {
  // @@protoc_insertion_point(field_get:Protocol.ObjectInfo.maxHp)
  return _internal_maxhp();
}
inline void ObjectInfo::_internal_set_maxhp(int32_t value) {
  
  _impl_.maxhp_ = value;
}
inline void ObjectInfo::set_maxhp(int32_t value) {
  _internal_set_maxhp(value);
  // @@protoc_insertion_point(field_set:Protocol.ObjectInfo.maxHp)
}

// int32 attack = 8;
inline void ObjectInfo::clear_attack() {
  _impl_.attack_ = 0;
}
inline int32_t ObjectInfo::_internal_attack() const {
  return _impl_.attack_;
}
inline int32_t ObjectInfo::attack() const {
  // @@protoc_insertion_point(field_get:Protocol.ObjectInfo.attack)
  return _internal_attack();
}
inline void ObjectInfo::_internal_set_attack(int32_t value) {
  
  _impl_.attack_ = value;
}
inline void ObjectInfo::set_attack(int32_t value) {
  _internal_set_attack(value);
  // @@protoc_insertion_point(field_set:Protocol.ObjectInfo.attack)
}

// int32 defence = 9;
inline void ObjectInfo::clear_defence() {
  _impl_.defence_ = 0;
}
inline int32_t ObjectInfo::_internal_defence() const {
  return _impl_.defence_;
}
inline int32_t ObjectInfo::defence() const {
  // @@protoc_insertion_point(field_get:Protocol.ObjectInfo.defence)
  return _internal_defence();
}
inline void ObjectInfo::_internal_set_defence(int32_t value) {
  
  _impl_.defence_ = value;
}
inline void ObjectInfo::set_defence(int32_t value) {
  _internal_set_defence(value);
  // @@protoc_insertion_point(field_set:Protocol.ObjectInfo.defence)
}

// int32 posX = 10;
inline void ObjectInfo::clear_posx() {
  _impl_.posx_ = 0;
}
inline int32_t ObjectInfo::_internal_posx() const {
  return _impl_.posx_;
}
inline int32_t ObjectInfo::posx() const {
  // @@protoc_insertion_point(field_get:Protocol.ObjectInfo.posX)
  return _internal_posx();
}
inline void ObjectInfo::_internal_set_posx(int32_t value) {
  
  _impl_.posx_ = value;
}
inline void ObjectInfo::set_posx(int32_t value) {
  _internal_set_posx(value);
  // @@protoc_insertion_point(field_set:Protocol.ObjectInfo.posX)
}

// int32 posY = 11;
inline void ObjectInfo::clear_posy() {
  _impl_.posy_ = 0;
}
inline int32_t ObjectInfo::_internal_posy() const {
  return _impl_.posy_;
}
inline int32_t ObjectInfo::posy() const {
  // @@protoc_insertion_point(field_get:Protocol.ObjectInfo.posY)
  return _internal_posy();
}
inline void ObjectInfo::_internal_set_posy(int32_t value) {
  
  _impl_.posy_ = value;
}
inline void ObjectInfo::set_posy(int32_t value) {
  _internal_set_posy(value);
  // @@protoc_insertion_point(field_set:Protocol.ObjectInfo.posY)
}

// int32 damage = 12;
inline void ObjectInfo::clear_damage() {
  _impl_.damage_ = 0;
}
inline int32_t ObjectInfo::_internal_damage() const {
  return _impl_.damage_;
}
inline int32_t ObjectInfo::damage() const {
  // @@protoc_insertion_point(field_get:Protocol.ObjectInfo.damage)
  return _internal_damage();
}
inline void ObjectInfo::_internal_set_damage(int32_t value) {
  
  _impl_.damage_ = value;
}
inline void ObjectInfo::set_damage(int32_t value) {
  _internal_set_damage(value);
  // @@protoc_insertion_point(field_set:Protocol.ObjectInfo.damage)
}

// .Protocol.WEAPON_TYPE weaponType = 13;
inline void ObjectInfo::clear_weapontype() {
  _impl_.weapontype_ = 0;
}
inline ::Protocol::WEAPON_TYPE ObjectInfo::_internal_weapontype() const {
  return static_cast< ::Protocol::WEAPON_TYPE >(_impl_.weapontype_);
}
inline ::Protocol::WEAPON_TYPE ObjectInfo::weapontype() const {
  // @@protoc_insertion_point(field_get:Protocol.ObjectInfo.weaponType)
  return _internal_weapontype();
}
inline void ObjectInfo::_internal_set_weapontype(::Protocol::WEAPON_TYPE value) {
  
  _impl_.weapontype_ = value;
}
inline void ObjectInfo::set_weapontype(::Protocol::WEAPON_TYPE value) {
  _internal_set_weapontype(value);
  // @@protoc_insertion_point(field_set:Protocol.ObjectInfo.weaponType)
}

// uint64 gold = 14;
inline void ObjectInfo::clear_gold() {
  _impl_.gold_ = uint64_t{0u};
}
inline uint64_t ObjectInfo::_internal_gold() const {
  return _impl_.gold_;
}
inline uint64_t ObjectInfo::gold() const {
  // @@protoc_insertion_point(field_get:Protocol.ObjectInfo.gold)
  return _internal_gold();
}
inline void ObjectInfo::_internal_set_gold(uint64_t value) {
  
  _impl_.gold_ = value;
}
inline void ObjectInfo::set_gold(uint64_t value) {
  _internal_set_gold(value);
  // @@protoc_insertion_point(field_set:Protocol.ObjectInfo.gold)
}

// .Protocol.ITEM_TYPE itemType = 15;
inline void ObjectInfo::clear_itemtype() {
  _impl_.itemtype_ = 0;
}
inline ::Protocol::ITEM_TYPE ObjectInfo::_internal_itemtype() const {
  return static_cast< ::Protocol::ITEM_TYPE >(_impl_.itemtype_);
}
inline ::Protocol::ITEM_TYPE ObjectInfo::itemtype() const {
  // @@protoc_insertion_point(field_get:Protocol.ObjectInfo.itemType)
  return _internal_itemtype();
}
inline void ObjectInfo::_internal_set_itemtype(::Protocol::ITEM_TYPE value) {
  
  _impl_.itemtype_ = value;
}
inline void ObjectInfo::set_itemtype(::Protocol::ITEM_TYPE value) {
  _internal_set_itemtype(value);
  // @@protoc_insertion_point(field_set:Protocol.ObjectInfo.itemType)
}

// -------------------------------------------------------------------

// Text

// uint64 objectId = 1;
inline void Text::clear_objectid() {
  _impl_.objectid_ = uint64_t{0u};
}
inline uint64_t Text::_internal_objectid() const {
  return _impl_.objectid_;
}
inline uint64_t Text::objectid() const {
  // @@protoc_insertion_point(field_get:Protocol.Text.objectId)
  return _internal_objectid();
}
inline void Text::_internal_set_objectid(uint64_t value) {
  
  _impl_.objectid_ = value;
}
inline void Text::set_objectid(uint64_t value) {
  _internal_set_objectid(value);
  // @@protoc_insertion_point(field_set:Protocol.Text.objectId)
}

// uint64 time = 2;
inline void Text::clear_time() {
  _impl_.time_ = uint64_t{0u};
}
inline uint64_t Text::_internal_time() const {
  return _impl_.time_;
}
inline uint64_t Text::time() const {
  // @@protoc_insertion_point(field_get:Protocol.Text.time)
  return _internal_time();
}
inline void Text::_internal_set_time(uint64_t value) {
  
  _impl_.time_ = value;
}
inline void Text::set_time(uint64_t value) {
  _internal_set_time(value);
  // @@protoc_insertion_point(field_set:Protocol.Text.time)
}

// string str = 3;
inline void Text::clear_str() {
  _impl_.str_.ClearToEmpty();
}
inline const std::string& Text::str() const {
  // @@protoc_insertion_point(field_get:Protocol.Text.str)
  return _internal_str();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void Text::set_str(ArgT0&& arg0, ArgT... args) {
 
 _impl_.str_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:Protocol.Text.str)
}
inline std::string* Text::mutable_str() {
  std::string* _s = _internal_mutable_str();
  // @@protoc_insertion_point(field_mutable:Protocol.Text.str)
  return _s;
}
inline const std::string& Text::_internal_str() const {
  return _impl_.str_.Get();
}
inline void Text::_internal_set_str(const std::string& value) {
  
  _impl_.str_.Set(value, GetArenaForAllocation());
}
inline std::string* Text::_internal_mutable_str() {
  
  return _impl_.str_.Mutable(GetArenaForAllocation());
}
inline std::string* Text::release_str() {
  // @@protoc_insertion_point(field_release:Protocol.Text.str)
  return _impl_.str_.Release();
}
inline void Text::set_allocated_str(std::string* str) {
  if (str != nullptr) {
    
  } else {
    
  }
  _impl_.str_.SetAllocated(str, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.str_.IsDefault()) {
    _impl_.str_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:Protocol.Text.str)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace Protocol

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_Struct_2eproto
