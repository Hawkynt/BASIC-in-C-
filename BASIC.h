#pragma once

#include <iostream>
#include <string>    // STRING, CSTR
#include <cstring>   // LEN(char*)
#include <cstdlib>   // RND, RANDOMIZE_TIMER, RUN, VAL
#include <ctime>     // TIMER, RANDOMIZE_TIMER
#include <cctype>    // UCASE$, LCASE$
#include <sstream>   // STR$, HEX$, OCT$
#include <utility>   // SWAP
#include <vector>    // LIST_OF
#include <map>       // DICTIONARY_OF
#include <algorithm> // LIST_SORT, LIST_REVERSE, LIST_CONTAINS
#include <windows.h>
#include <conio.h>

// macro hacks
#define CONCAT2(a, b) a##b
#define CONCAT(a, b) CONCAT2(a, b)
#define UNIQUE(name) CONCAT(name, __LINE__)
#define EXPAND(x) x

// Assignment
// Type mapping
#define INTEGER int
#define LONG    long
#define SINGLE  float
#define DOUBLE  double
#define STRING  std::string
#define BYTE    unsigned char
#define BOOLEAN bool
#define ASCIIZ  char // PowerBASIC's zero-terminated buffer: DIM_ARRAY(buffer, 32 AS ASCIIZ)

// Collection types (yes, your DOS-era BASIC never had these - consider them
// a gift from the future). Both compose: DICTIONARY_OF(STRING, LIST_OF(INTEGER))
#define LIST_OF(type) std::vector<type>
#define DICTIONARY_OF(keytype, valuetype) std::map<keytype, valuetype>

// Variable declarations
// The type slot is variadic so templated types whose expansion contains
// commas - e.g. DICTIONARY_OF(STRING, INTEGER) - survive the macro grinder.
#define AS ,
#define __DIM_IMPL(var , ...) __VA_ARGS__ var;
#define DIM(...) EXPAND(__DIM_IMPL(__VA_ARGS__))
#define __DIM_ARRAY_IMPL(var , size , ...) __VA_ARGS__ var[size];
#define DIM_ARRAY(...) EXPAND(__DIM_ARRAY_IMPL(__VA_ARGS__))

#define SET(x) x;
#define LET(x) auto x;
#undef CONST // windows.h defines a plain CONST; ours is function-like so winnt's usage stays untouched
#define CONST(x) const constexpr auto x;

// Logic & conditionals
#define IF(cond) if(cond)
#define THEN {
#define ELSE } else {
#define ELSEIF(cond) } else if(cond) {
#define ENDIF }

#define SELECT(var) switch(var) {
#define CASE(val) case val: {
#define CASE_ELSE default: {
#define END_CASE } break;
#define END_SELECT }

#define AND &&
#define OR ||
#define NOT !

// Structured Paradigm
#define DO_WHILE(cond) while(cond) {
#define DO_UNTIL(cond) while(!(cond)) {

#define WHILE(cond) while(cond) {
#define WEND }

#define DO do {
#define LOOP_WHILE(cond) } while(cond);
#define LOOP_UNTIL(cond) } while(!(cond));
#define LOOP }
#define LOOP_FOREVER } while(true);

// For-loops
#define FOR(var, start, end) { auto UNIQUE(__end) = (end); for (auto var = start; var <= UNIQUE(__end); ++var) {
#define FOR_STEP(var, start, end, step) { auto UNIQUE(__end) = (end); auto UNIQUE(__step) = (step); for (auto var = start; UNIQUE(__step) > 0 ? var <= UNIQUE(__end) : var >= UNIQUE(__end); var += UNIQUE(__step)) {
#define NEXT } }

// FOR EACH item IN container ... NEXT (VB junior joins the party)
#ifdef IN
#undef IN // some windows headers claim IN as a SAL annotation; BASIC needs it more
#endif
#define IN ,
#define __FOR_EACH_IMPL(var , container) { for (auto && var : (container)) {
#define FOR_EACH(...) EXPAND(__FOR_EACH_IMPL(__VA_ARGS__))

// Breaks and flow
#define BREAK break
#define CONTINUE continue
#define GOTO(label) goto label
#define LABEL(name) name:

// PowerBASIC flow statements
#define EXIT_DO break;
#define EXIT_FOR break;
#define EXIT_WHILE break;
#define EXIT_SUB return;
#define ITERATE_DO continue;
#define ITERATE_FOR continue;

// PowerBASIC INCR/DECR statements: INCR(x) or INCR(x, amount)
template<typename T> void __INCR_IMPL(T & value) { ++value; }
template<typename T, typename TAmount> void __INCR_IMPL(T & value, TAmount amount) { value += amount; }
template<typename T> void __DECR_IMPL(T & value) { --value; }
template<typename T, typename TAmount> void __DECR_IMPL(T & value, TAmount amount) { value -= amount; }
#define INCR(...) EXPAND(__INCR_IMPL(__VA_ARGS__));
#define DECR(...) EXPAND(__DECR_IMPL(__VA_ARGS__));

#define SWAP(a, b) std::swap(a, b);
#define MOD %

// Modular Paradigm
#define __FUNCTION_IMPL(signature , rettype) rettype signature
#define FUNCTION(...) EXPAND(__FUNCTION_IMPL(__VA_ARGS__)) {
#define END_FUNCTION }

#define __SUB_IMPL(signature) void signature
#define SUB(...) EXPAND(__SUB_IMPL(__VA_ARGS__)) {
#define END_SUB }

#define RETURN(val) return val
#define END return 0;

// Core Functions
inline int __LEN_IMPL(const std::string & s) {
  return static_cast<int>(s.length());
}

inline int __LEN_IMPL(const char* s) {
  return static_cast<int>(std::strlen(s));
}

template<typename T>
int LEN(const T & s) {
  return __LEN_IMPL(s); // Calls correct overload based on type
}
#define CINT(x) static_cast<int>(x)
#define CLNG(x) static_cast<long>(x)
#define CSNG(x) static_cast<float>(x)
#define CDBL(x) static_cast<double>(x)
#define CSTR(x) std::to_string(x)

// String functions - and yes, the $ is part of the identifier. Both MSVC and
// GCC accept $ in identifiers, so LEFT$ really is called LEFT$. The 80s won.
// Positions are 1-based like the good lord Kemeny intended; out-of-range
// arguments clamp instead of crashing because home computers were forgiving.
inline STRING LEFT$(const STRING & s, int count) {
  if (count <= 0) return "";
  if (static_cast<size_t>(count) >= s.length()) return s;
  return s.substr(0, count);
}

inline STRING RIGHT$(const STRING & s, int count) {
  if (count <= 0) return "";
  if (static_cast<size_t>(count) >= s.length()) return s;
  return s.substr(s.length() - count);
}

inline STRING MID$(const STRING & s, int start) {
  if (start < 1) start = 1;
  if (static_cast<size_t>(start) > s.length()) return "";
  return s.substr(start - 1);
}

inline STRING MID$(const STRING & s, int start, int count) {
  if (start < 1) start = 1;
  if (count <= 0 || static_cast<size_t>(start) > s.length()) return "";
  return s.substr(start - 1, count);
}

// 1-based position of needle in haystack, 0 if absent - INSTR([start,] haystack, needle)
inline int INSTR(int start, const STRING & haystack, const STRING & needle) {
  if (start < 1) start = 1;
  const auto position = haystack.find(needle, start - 1);
  return position == STRING::npos ? 0 : static_cast<int>(position) + 1;
}

inline int INSTR(const STRING & haystack, const STRING & needle) {
  return INSTR(1, haystack, needle);
}

inline STRING UCASE$(STRING s) {
  for (auto & character : s)
    character = static_cast<char>(std::toupper(static_cast<unsigned char>(character)));
  return s;
}

inline STRING LCASE$(STRING s) {
  for (auto & character : s)
    character = static_cast<char>(std::tolower(static_cast<unsigned char>(character)));
  return s;
}

inline STRING LTRIM$(const STRING & s) {
  const auto position = s.find_first_not_of(' ');
  return position == STRING::npos ? "" : s.substr(position);
}

inline STRING RTRIM$(const STRING & s) {
  const auto position = s.find_last_not_of(' ');
  return position == STRING::npos ? "" : s.substr(0, position + 1);
}

inline STRING TRIM$(const STRING & s) {
  return LTRIM$(RTRIM$(s));
}

inline STRING SPACE$(int count) {
  return count <= 0 ? "" : STRING(count, ' ');
}

inline STRING STRING$(int count, int code) {
  return count <= 0 ? "" : STRING(count, static_cast<char>(code));
}

inline STRING STRING$(int count, const STRING & s) {
  return count <= 0 || s.empty() ? "" : STRING(count, s[0]);
}

inline STRING REPEAT$(int count, const STRING & s) {
  STRING result;
  for (auto i = 0; i < count; ++i)
    result += s;
  return result;
}

inline STRING CHR$(int code) {
  return STRING(1, static_cast<char>(code));
}

inline int ASC(const STRING & s) {
  return s.empty() ? 0 : static_cast<unsigned char>(s[0]);
}

// STR$ keeps the classic leading sign slot: " 42" for positives, "-42" for negatives
template<typename T>
STRING STR$(T value) {
  std::ostringstream stream;
  stream << value;
  return value >= 0 ? " " + stream.str() : stream.str();
}

// parses a leading number, 0 if there is none - VAL(" 3.14cm") = 3.14
inline DOUBLE VAL(const STRING & s) {
  return std::strtod(s.c_str(), nullptr);
}

inline STRING HEX$(long value) {
  std::ostringstream stream;
  stream << std::uppercase << std::hex << value;
  return stream.str();
}

inline STRING OCT$(long value) {
  std::ostringstream stream;
  stream << std::oct << value;
  return stream.str();
}

inline STRING BIN$(long value) {
  auto bits = static_cast<unsigned long>(value);
  if (bits == 0) return "0";
  STRING result;
  for (; bits > 0; bits >>= 1)
    result.insert(result.begin(), static_cast<char>('0' + (bits & 1)));
  return result;
}

// counts non-overlapping occurrences of needle in haystack
inline int TALLY(const STRING & haystack, const STRING & needle) {
  if (needle.empty()) return 0;
  auto count = 0;
  for (auto position = haystack.find(needle); position != STRING::npos; position = haystack.find(needle, position + needle.length()))
    ++count;
  return count;
}

// List operations (1-based, of course - we're not animals)
#define LIST_ADD(list, value) (list).push_back(value);
#define LIST_INSERT(list, position, value) (list).insert((list).begin() + ((position) - 1), value);
#define LIST_REMOVE(list, position) (list).erase((list).begin() + ((position) - 1));
#define LIST_CLEAR(list) (list).clear();
#define LIST_SORT(list) std::sort((list).begin(), (list).end());
#define LIST_REVERSE(list) std::reverse((list).begin(), (list).end());
#define LIST_ITEM(list, position) (list)[(position) - 1]
#define LIST_COUNT(list) static_cast<int>((list).size())

template<typename TList, typename TValue>
BOOLEAN LIST_CONTAINS(const TList & list, const TValue & value) {
  return std::find(list.begin(), list.end(), value) != list.end();
}

// 1-based index of the first match, 0 if absent (INSTR's spiritual cousin)
template<typename TList, typename TValue>
int LIST_INDEX_OF(const TList & list, const TValue & value) {
  const auto found = std::find(list.begin(), list.end(), value);
  return found == list.end() ? 0 : static_cast<int>(std::distance(list.begin(), found)) + 1;
}

// Dictionary operations
#define DICT_SET(dict, key, value) (dict)[key] = value;
#define DICT_ITEM(dict, key) (dict)[key]
#define DICT_HAS(dict, key) ((dict).count(key) > 0)
#define DICT_REMOVE(dict, key) (dict).erase(key);
#define DICT_CLEAR(dict) (dict).clear();
#define DICT_COUNT(dict) static_cast<int>((dict).size())

template<typename TDict>
LIST_OF(typename TDict::key_type) DICT_KEYS(const TDict & dict) {
  LIST_OF(typename TDict::key_type) keys;
  keys.reserve(dict.size());
  for (const auto & pair : dict)
    keys.push_back(pair.first);
  return keys;
}

template<typename TDict>
LIST_OF(typename TDict::mapped_type) DICT_VALUES(const TDict & dict) {
  LIST_OF(typename TDict::mapped_type) values;
  values.reserve(dict.size());
  for (const auto & pair : dict)
    values.push_back(pair.second);
  return values;
}

// I/O
#define SLEEP(ms) Sleep(ms);
#define RND() (static_cast<float>(rand()) / static_cast<float>(RAND_MAX))
#define TIMER() (static_cast<float>(std::clock()) / CLOCKS_PER_SEC)
#define RANDOMIZE_TIMER() srand(static_cast<unsigned int>(time(nullptr)));

template<typename... Args>
void __PRINT_IMPL(Args&&... args) {
  (std::cout << ... << args) << std::endl;
}

#define PRINT(...) do { __PRINT_IMPL(__VA_ARGS__); } while(0);

inline void __CLS_IMPL() {
  const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD count;
  if (!GetConsoleScreenBufferInfo(hOut, &csbi)) return;

  const DWORD cells = csbi.dwSize.X * csbi.dwSize.Y;
  FillConsoleOutputCharacter(hOut, ' ', cells, { 0, 0 }, &count);
  FillConsoleOutputAttribute(hOut, csbi.wAttributes, cells, { 0, 0 }, &count);
  SetConsoleCursorPosition(hOut, { 0, 0 });
}

#define CLS() __CLS_IMPL();

#define LOCATE(r,c) { COORD pos = { static_cast<SHORT>(c-1), static_cast<SHORT>(r-1) }; SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); }
#define COLOR(fg,bg) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ((bg)<<4) | (fg));

#define INPUT(var) std::cin >> var;
#define INKEY() (_kbhit() ? _getch() : 0)

#define RUN(cmd) std::system(cmd);

