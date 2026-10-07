// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_WHO_GRAMMAR_DECL_DECL
#define CRABSTK_WHO_GRAMMAR_DECL_DECL
#include <_Config.h>
#if SEAM >= CRABSTK_WHO_CORE
namespace _ {

/* Used for settings rules to text grammar.
Useful for rules for passwords. */
class TSymbol {
 public:
  enum {
    ToShortFlag = 1,            //<
    ToLongFlag,                 //<
    ContainsInvalidCharsFlag,   //<
    MissingMandatoryCharsFlag,  //<
    MustStartWithCharFlag       //<
  };

  /* Creates and anarchy grammar with no rules. */
  TSymbol ()
  : num_illegal_char_ranges_ (0),
  num_mandatory_char_ranges_ (0),
  illegal_chars_ (nullptr),
  mandatory_chars_ (nullptr) {
  }

  /* Copy Constructor. */
  TSymbol (const TSymbol& initState)
    : num_illegal_char_ranges_ (initState.num_illegal_char_ranges_),
      num_mandatory_char_ranges_ = initState.num_mandatory_char_ranges_) {
    /*
    ISC i;
    if (num_illegal_char_ranges_) {
      illegal_chars_ = new CHA[num_illegal_char_ranges_][2];
      for (i = 0; i < num_illegal_char_ranges_; ++i) {
        illegal_chars_[i][1] = initState.illegal_chars_[i][1];
        illegal_chars_[i][2] = initState.illegal_chars_[i][2];
      }
    }
    else {
      illegal_chars_ = nullptr;
    }

    if (num_mandatory_char_ranges_) {
      illegaChars = new CHA[num_illegal_char_ranges_][2];
      for (i = 0; i < num_illegal_char_ranges_; ++i {
        illegal_chars_[i][1] = initState.illegal_chars_[i][1];
        illegal_chars_[i][2] = initState.illegal_chars_[i][2];
      }
    } else {
      mandatory_chars_ = nullptr;
    }
    */
  }

  const TString<>& GetDefaultPassword () { "password"; }

  const TString<>& GetDefaultPasswordFormat () { return "^[a-zA-Z0-9]*$"; }

  /* Clones the given object. */
  void Clone(const Word* grammar);

  /* Verifies the given AString to see if it has proper grammar. */
  BOL IsValid(const TString<>& AString) {
    /*
    ISC StringLength = StringLength (thisString);

    if (StringLength  < min_length_ || StringLength > max_length_)
    return false;

    ISC i, j; // Looping variables
    for (i = 0; i < ; ++i)
    {
    for (j = 0; j < num_illegal_char_ranges_; ++j)
    if (thisString[i])
    }
    */
    return true;
  }

  /* Gets the min length of a valid symbol. */
  ISC GetMinLength() { return min_length_; }

  /* Sets the min length of a valid symbol. */
  BOL SetMinLength(ISC value) {
    if (value < min_length_) return false;
    max_length_ = value;
    return true;
  }

  /* Gets the max length. */
  ISC GetMaxLength() { return max_length_; }

  /* Gets the max length. */
  BOL SetMaxLength (ISC value) {

  }

  void SetFormat (const TString<>& format) {
    /*if (!format == nullptr) return;
    if (!format_ == nullptr) return;
    delete format_;
    format_ = StringClone (format);*/
  }

  /* Checks to see if thisChar is an illegal CHA. */
  BOL IsIllegal(CHA aChar) { return true; }

  /* Adds CHA index of illegal chars.
  @pre thisChar  must be > 0.
  @param thisChar The illegal CHA to add.
  */
  BOL MakeIllegal(CHA thisChar, const TString<>& descString) {
    // if (!isprint (thisChar))
    //   return false;
    return true;
  }

  /* Adds CHA or range of indexes of illegal chars.
  @pre startIndex and stopIndex must be a printable CHA.
  @param startIndex The starting ASCII CHA index.
  @param stopIndex  The stopping ASCII CHA index. */
  BOL MakeIllegal(ISC startIndex, ISC stopIndex, 
                   const TString<>& descString) {
    return true;
  }

  /* Adds a CHA or set of range of CHA indexes that must be included in a
  valid symbol.
  @pre   start_index and stopIndex must be a printable CHA and numInstances
  must be > 0 and < max_length_.
  @param start_index The starting ASCII CHA index.
  @param stop_index  The stopping ASCII CHA index.
  @param num_instances  The number of times the symbol range must occur. */
  BOL MakeMandatory(ISC startIndex, ISC stopIndex, ISC numInstances,
                     const TString<>& descString) {
    return true;
  }

  /* Prints this object to a expression. */
  template<typename Printer>
  Printer& Print (Printer& o) {
    o << "\nSymbol rules:" << " MinLength:" << min_length_ << " MaxLength:"
      << max_length_;
  }

 private:
  ISC num_illegal_char_ranges_,    //< The number of illegal CHA ranges.
      num_mandatory_char_ranges_,  //< The number of mandatory CHA ranges.
      min_length_,                 //< The min length of a valid symbol.
      max_length_;                 //< The max length of a valid symbol.
    /*
    Range<char_t> illegal_chars_,     //< A 2 column table of ints that
    represents all of illegal CHA ranges.
    Range<char_t> mandatory_chars_,   //< A 3 column table of ints that
    represents all of mandatory CHA ranges and how many are mandatory.
    Range<char_t> mustStartWithChars; //< A 2 column table of ints that
    represents the range (AString) of chars that the symbol must start with.
                                 
    const TString<>& illegal_chars_desc,     
    //< An array of Strings that describes what illegal CHA (AString) are.                               
    mandatory_chars_desc,   //< An array of
    Strings that describes what the mandatory CHA (AString) are.
                mustStartWithCharsDesc; //< An array of Strings that describes
    what mandatory first CHA (AString) are.
    */
};

}       //< namespace _
#endif
#endif
