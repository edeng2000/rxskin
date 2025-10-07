#ifndef JSON_WRITER_H_INCLUDED
#define JSON_WRITER_H_INCLUDED

#if !defined(JSON_IS_AMALGAMATION)
#include "json/value.h"
#endif 
#include <vector>
#include <string>

// Disable warning C4251: <data member>: <type> needs to have dll-interface to
#if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)
#pragma warning(push)
#pragma warning(disable : 4251)
#endif

namespace Json {

class Value;

/** \brief Abstract class for writers.
 */
class JSON_API Writer {
public:
  virtual ~Writer();

  // just return reference of the string
  virtual std::string &write(const Value& root) = 0;
};

class JSON_API FastWriter : public Writer {
public:
  FastWriter();
  virtual ~FastWriter() {}

  void enableYAMLCompatibility();
  void dropNullPlaceholders();
  void omitEndingLineFeed();

public: // overridden from Writer
  virtual std::string &write(const Value& root);

private:
  void writeValue(const Value& value);

  std::string document_;
  bool yamlCompatiblityEnabled_;
  bool dropNullPlaceholders_;
  bool omitEndingLineFeed_;
};

class JSON_API StyledWriter : public Writer {
public:
  StyledWriter();
  virtual ~StyledWriter() {}

public:
  virtual std::string &write(const Value& root);

private:
  void writeValue(const Value& value);
  void writeArrayValue(const Value& value);
  bool isMultineArray(const Value& value);
  void pushValue(const std::string& value);
  void writeIndent();
  void writeWithIndent(const std::string& value);
  void indent();
  void unindent();
  void writeCommentBeforeValue(const Value& root);
  void writeCommentAfterValueOnSameLine(const Value& root);
  bool hasCommentForValue(const Value& value);
  static std::string normalizeEOL(const std::string& text);

  typedef std::vector<std::string> ChildValues;

  ChildValues childValues_;
  std::string document_;
  std::string indentString_;
  int rightMargin_;
  int indentSize_;
  bool addChildValues_;
};

class JSON_API StyledStreamWriter {
public:
  StyledStreamWriter(std::string indentation = "\t");
  ~StyledStreamWriter() {}

public:
  void write(std::ostream& out, const Value& root);

private:
  void writeValue(const Value& value);
  void writeArrayValue(const Value& value);
  bool isMultineArray(const Value& value);
  void pushValue(const std::string& value);
  void writeIndent();
  void writeWithIndent(const std::string& value);
  void indent();
  void unindent();
  void writeCommentBeforeValue(const Value& root);
  void writeCommentAfterValueOnSameLine(const Value& root);
  bool hasCommentForValue(const Value& value);
  static std::string normalizeEOL(const std::string& text);

  typedef std::vector<std::string> ChildValues;

  ChildValues childValues_;
  std::ostream* document_;
  std::string indentString_;
  int rightMargin_;
  std::string indentation_;
  bool addChildValues_;
};

#if defined(JSON_HAS_INT64)
std::string JSON_API valueToString(Int value);
std::string JSON_API valueToString(UInt value);
#endif // if defined(JSON_HAS_INT64)
std::string JSON_API valueToString(LargestInt value);
std::string JSON_API valueToString(LargestUInt value);
std::string JSON_API valueToString(double value);
std::string JSON_API valueToString(bool value);
std::string JSON_API valueToQuotedString(const char* value);

/// \brief Output using the StyledStreamWriter.
/// \see Json::operator>>()
JSON_API std::ostream& operator<<(std::ostream&, const Value& root);

} // namespace Json

#if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)
#pragma warning(pop)
#endif // if defined(JSONCPP_DISABLE_DLL_INTERFACE_WARNING)

#endif // JSON_WRITER_H_INCLUDED
