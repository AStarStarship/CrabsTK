// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_TOOLKIT_UPDATE_JSX_H
#define CRABS_TOOLKIT_UPDATE_JSX_H

#include <initializer_list>
#include <span>
#include <string>
#include <string_view>

namespace CT::JSX {

struct Attribute {
  std::string_view name;
  std::string_view value;
  bool boolean = false;
};

inline bool NameIsValid(std::string_view name) {
  if (name.empty()) return false;
  for (char item : name) {
    bool valid = (item >= 'a' && item <= 'z') ||
                 (item >= 'A' && item <= 'Z') ||
                 (item >= '0' && item <= '9') || item == '-' ||
                 item == '_' || item == ':';
    if (!valid) return false;
  }
  return true;
}

inline void EscapeText(std::string& output, std::string_view text) {
  for (char item : text) {
    switch (item) {
      case '&': output += "&amp;"; break;
      case '<': output += "&lt;"; break;
      case '>': output += "&gt;"; break;
      default: output.push_back(item); break;
    }
  }
}

inline void EscapeAttribute(std::string& output, std::string_view text) {
  for (char item : text) {
    switch (item) {
      case '&': output += "&amp;"; break;
      case '<': output += "&lt;"; break;
      case '>': output += "&gt;"; break;
      case '"': output += "&quot;"; break;
      case '\'': output += "&#39;"; break;
      default: output.push_back(item); break;
    }
  }
}

class Element {
 public:
  explicit Element(std::string_view name) : name_(name) {}

  Element& AttributeSet(std::string_view name, std::string_view value) {
    if (NameIsValid(name)) attributes_.append(" ").append(name).append("=\"");
    else return *this;
    EscapeAttribute(attributes_, value);
    attributes_.push_back('"');
    return *this;
  }

  Element& BooleanAttribute(std::string_view name, bool asserted = true) {
    if (asserted && NameIsValid(name)) attributes_.append(" ").append(name);
    return *this;
  }

  Element& Text(std::string_view text) {
    EscapeText(children_, text);
    return *this;
  }

  Element& Raw(std::string_view trusted_markup) {
    children_.append(trusted_markup);
    return *this;
  }

  std::string Render() const {
    if (!NameIsValid(name_)) return {};
    std::string output;
    output.reserve(name_.size() * 2 + attributes_.size() + children_.size() + 5);
    output.push_back('<');
    output.append(name_).append(attributes_).push_back('>');
    output.append(children_).append("</").append(name_).push_back('>');
    return output;
  }

 private:
  std::string name_;
  std::string attributes_;
  std::string children_;
};

inline std::string Fragment(std::initializer_list<std::string_view> children) {
  std::size_t bytes = 0;
  for (std::string_view child : children) bytes += child.size();
  std::string output;
  output.reserve(bytes);
  for (std::string_view child : children) output.append(child);
  return output;
}

}  // namespace CT::JSX
#endif
