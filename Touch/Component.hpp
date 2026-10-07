// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_TOUCH_HMICOMPONENT_DECL_DECL
#define CRABSTK_TOUCH_HMICOMPONENT_DECL_DECL
#include <_Config.h>
#if SEAM >= CRABSTK_TOUCH_CORE
namespace _ {

typedef enum {
  HMITypeMIDI = 0,
  HMITypeDMX,

} HMIComponentType;

//< The non-alpha-numeric characters allowed in a label.
const CHA* GetAllowedSymbols();

/* Sets the allowed symbols to the given AString. */
void SetAllowedSymbols(const CHA* s);

/* Checks the given CHA c to see if any of the chars contain it. */
inline BOL ContainsChar(const CHA* chars, CHA c);

/* Checks the given AString s to see if any of the chars are in it. */
inline BOL ContainsChar(const CHA* chars, const CHA* s);

/* Verifies if thisLabel is a valid label.
@return Returns 0 if the label is valid.
@return Returns 1 if the label is too uint32_t.
@return Returns -1 thisLabel is "".
*/
ISC IsValidLabel(const CHA* label);

/* All unnamed labels share the same CHA* pointer. */
const CHA* GetUnnamedLabel();

/* A list of the most common Parameter types. */
typedef enum ParameterTypes {
  ControlDMX = 0,
  ControlMidi,
  OSCControl,
  DMXButton,
  TButtonMIDI,
  OSCButton,
  ButtonMacro
} ParameterType;

/* A component in a Human-Machine Interface that may be referenced by name or
IUD. A HMIComponent has the following rules: 1.) Labels must be 255
characters or less for a normal label, and 16 characters or less for a short
label. 2.) Labels must contain printable ASCII characters that are HUI
compatible.
*/
class LIB_MEMBER HMIComponent {
 public:
  enum {
    MaxLabelLength = 20,  //< The max label length of a label.
  };

  /* Default constructor. */
  HMIComponent(const CHA* newLabel = "") {
    if (SetLabel (label)) label = GetUnnamedLabel ();
  }

  /* Copy constructor. */
  HMIComponent(const HMIComponent& other);

  /* Virtual destructor. */
  virtual ~HMIComponent() {}

  /* Gets the unique ID for the object. */
  IUD Uid () { return uid_;  }

  /* Gets the unique AString of the component type. */
  const CHA* GetHMIComponentType();

  /* Gets the label. */
  const CHA* Label() const { return label_; }

  /* Sets the label to the given AString.
  @return Returns 0 upon success and 1 if the new label is too uint32_t.
  @see    HMIComponent::IsValid (CHA). */
  virtual ISC SetLabel(const CHA* label) {
    if (label == "") return -1;
    SIW length = StringLength (label);
    if (length > MaxLabelLength) return 1;

    if (!IsValidLabel (label)) return 1;

    label = label;
    return 0;
  }

  /* Compares this label to the given AString.
  @return Returns 0 if they are identical and1 if the other CHA* is too
  uint32_t. */
  ISC Compare(const CHA* s) const {
    return label_.Compare (s);
  }

  /* Prints this object to a AString. */
  template<typename Printer>
  Printer& Print (Printer& o) const { return label_->Print (o); }

 private:
  IUD uid_,          //< Unique id.
      type_;           //< Type of HMI component.
  const CHA* label_;  //< This object's text label.
};                     //< class HMIComponent
}  //< namespace _
#endif 
#endif
