// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_TOOLKIT_UPDATE_ATYPE_H
#define CRABS_TOOLKIT_UPDATE_ATYPE_H

#include <cstdint>

namespace CT {

using TypeWord = std::uint16_t;
using TypePayload = std::uint16_t;

enum class TypeMod : TypeWord {
  Unsigned = 0,
  Pointer = 1,
  Signed = 2,
  Context = 3,
  Constant = 4,
  ConstantPointer = 5,
  ConstantSigned = 6,
  ConstantContext = 7,
};

inline constexpr TypeWord TypePayloadBits = 12;
inline constexpr TypeWord TypePayloadMask = 0x0fff;
inline constexpr TypeWord TypeModMask = 0xf000;
inline constexpr TypeWord TypeModBit0 = TypePayloadBits;

/* Packs a 12-bit ASCII payload and four modifier bits into one word. */
constexpr TypeWord TypePack(TypePayload payload, TypeMod mod) {
  return TypeWord((payload & TypePayloadMask) |
                  (TypeWord(mod) << TypeModBit0));
}

constexpr TypePayload TypePayloadOf(TypeWord type) {
  return TypePayload(type & TypePayloadMask);
}

constexpr TypeMod TypeModOf(TypeWord type) {
  return TypeMod((type & TypeModMask) >> TypeModBit0);
}

constexpr TypeWord TypeWithMod(TypeWord type, TypeMod mod) {
  return TypePack(TypePayloadOf(type), mod);
}

constexpr bool TypeIsSigned(TypeWord type) {
  TypeMod mod = TypeModOf(type);
  return mod == TypeMod::Signed || mod == TypeMod::ConstantSigned;
}

constexpr bool TypeIsContext(TypeWord type) {
  TypeMod mod = TypeModOf(type);
  return mod == TypeMod::Context || mod == TypeMod::ConstantContext;
}

/* C0 values remain available as machine controls in the MOD-0 payload. */
enum class Control : TypePayload {
  Nil = 0x00,
  StartOfHeading = 0x01,
  StartOfText = 0x02,
  EndOfText = 0x03,
  EndOfTransmission = 0x04,
  Enquiry = 0x05,
  Acknowledge = 0x06,
  Bell = 0x07,
  Backspace = 0x08,
  HorizontalTab = 0x09,
  LineFeed = 0x0a,
  VerticalTab = 0x0b,
  FormFeed = 0x0c,
  CarriageReturn = 0x0d,
  ShiftOut = 0x0e,
  ShiftIn = 0x0f,
  DataLinkEscape = 0x10,
  DeviceControl1 = 0x11,
  DeviceControl2 = 0x12,
  DeviceControl3 = 0x13,
  NegativeAcknowledge = 0x15,
  SynchronousIdle = 0x16,
  EndTransmissionBlock = 0x17,
  Cancel = 0x18,
  EndOfMedium = 0x19,
  Substitute = 0x1a,
  Escape = 0x1b,
  FileSeparator = 0x1c,
  GroupSeparator = 0x1d,
  RecordSeparator = 0x1e,
  UnitSeparator = 0x1f,
};

constexpr TypeWord ControlType(Control control) {
  return TypePack(TypePayload(control), TypeMod::Unsigned);
}

}  // namespace CT
#endif
