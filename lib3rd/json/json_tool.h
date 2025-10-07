#ifndef LIB_JSONCPP_JSON_TOOL_H_INCLUDED
#define LIB_JSONCPP_JSON_TOOL_H_INCLUDED

namespace Json {

/// Converts a unicode code-point to UTF-8.
static inline std::string codePointToUTF8(unsigned int cp) {
  std::string result;

  if (cp <= 0x7f) {
    result.resize(1);
    result[0] = static_cast<char>(cp);
  } else if (cp <= 0x7FF) {
    result.resize(2);
    result[1] = static_cast<char>(0x80 | (0x3f & cp));
    result[0] = static_cast<char>(0xC0 | (0x1f & (cp >> 6)));
  } else if (cp <= 0xFFFF) {
    result.resize(3);
    result[2] = static_cast<char>(0x80 | (0x3f & cp));
    result[1] = 0x80 | static_cast<char>((0x3f & (cp >> 6)));
    result[0] = 0xE0 | static_cast<char>((0xf & (cp >> 12)));
  } else if (cp <= 0x10FFFF) {
    result.resize(4);
    result[3] = static_cast<char>(0x80 | (0x3f & cp));
    result[2] = static_cast<char>(0x80 | (0x3f & (cp >> 6)));
    result[1] = static_cast<char>(0x80 | (0x3f & (cp >> 12)));
    result[0] = static_cast<char>(0xF0 | (0x7 & (cp >> 18)));
  }

  return result;
}

/// Returns true if ch is a control character (in range [0,32[).
static inline bool isControlCharacter(char ch) { return ch > 0 && ch <= 0x1F; }

enum {
  uintToStringBufferSize = 3 * sizeof(LargestUInt) + 1
};

// Defines a char buffer for use with uintToString().
typedef char UIntToStringBuffer[uintToStringBufferSize];

// uint to string buffer
static inline void uintToString(LargestUInt value, char*& current) {
  *--current = 0;
  do {
    *--current = char(value % 10) + '0';
    value /= 10;
  } while (value != 0);
}

/** Change ',' to '.' everywhere in buffer.
 *
 * We had a sophisticated way, but it did not work in WinCE.
 * @see https://github.com/open-source-parsers/jsoncpp/pull/9
 */
static inline void fixNumericLocale(char* begin, char* end) {
  while (begin < end) {
    if (*begin == ',') {
      *begin = '.';
    }
    ++begin;
  }
}

} // namespace Json {

#endif // LIB_JSONCPP_JSON_TOOL_H_INCLUDED
