#pragma once

#include <iostream>
#include <string>    // STRING, CSTR
#include <cstring>   // LEN(char*)
#include <cstdlib>   // RND, RANDOMIZE_TIMER, RUN, VAL
#include <ctime>     // TIMER, RANDOMIZE_TIMER
#include <cctype>    // UCASE$, LCASE$
#include <cmath>     // PLAY (note frequencies)
#include <cstdio>    // FLIP (frame assembly), KILL
#include <sstream>   // STR$, HEX$, OCT$
#include <fstream>   // OPEN/PRINT_FILE/LINE_INPUT_FILE/...
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

// ===========================================================================
// VGA in your terminal. SCREEN(13) allocates a real 256-colour framebuffer
// with the classic VGA DAC palette; FLIP() renders it as ANSI 24-bit
// half-blocks (one character cell = two stacked pixels). Works in any modern
// terminal - conhost, Windows Terminal, anything over ssh. Mode 13h forever.
// ===========================================================================
inline std::vector<BYTE> __vga_framebuffer;
inline int __vga_width = 0;
inline int __vga_height = 0;
inline int __vga_color_mask = 255;          // colours-1 of the active mode; PSET masks with it
inline unsigned __vga_video_base = 0xA0000; // physical base of the video window (mode-dependent)
inline bool __vga_unchained = false;        // mode X/Y/Z: the A000 window is planar, 4 pixels per address
inline int __vga_plane_mask = 0xF;          // sequencer register 2 (map mask) - which planes POKE writes
inline int __vga_read_plane = 0;            // graphics controller register 4 (read map select) - which plane PEEK reads
inline int __vga_bank = 0;                  // SVGA bank: which 64KB slice the A000 window shows
inline unsigned char __vga_palette[256][3]; // 6-bit DAC values, like mother nature intended

inline void __VGA_DEFAULT_PALETTE() {
  // 0-15: the EGA sixteen
  static const unsigned char ega[16][3] = {
    {0,0,0},{0,0,42},{0,42,0},{0,42,42},{42,0,0},{42,0,42},{42,21,0},{42,42,42},
    {21,21,21},{21,21,63},{21,63,21},{21,63,63},{63,21,21},{63,21,63},{63,63,21},{63,63,63}
  };
  // 16-31: the gray ramp
  static const unsigned char grays[16] = {0,5,8,11,14,17,20,24,28,32,36,40,45,50,56,63};
  for (auto i = 0; i < 16; ++i)
    for (auto channel = 0; channel < 3; ++channel)
      __vga_palette[i][channel] = ega[i][channel];
  for (auto i = 0; i < 16; ++i)
    __vga_palette[16 + i][0] = __vga_palette[16 + i][1] = __vga_palette[16 + i][2] = grays[i];
  // 32-247: 3 brightness blocks x 3 saturation rings x 24-step hue wheel,
  // blue -> magenta -> red -> yellow -> green -> cyan -> blue (close
  // approximation of the factory DAC values)
  static const unsigned char top[3] = {63, 45, 28};
  auto index = 32;
  for (auto brightness = 0; brightness < 3; ++brightness) {
    const int hi = top[brightness];
    const int los[3] = {0, hi / 2, hi * 3 / 4};
    for (auto saturation = 0; saturation < 3; ++saturation) {
      const auto lo = los[saturation];
      for (auto step = 0; step < 24; ++step) {
        const auto segment = step / 4;
        const auto rise = lo + (hi - lo) * (step % 4) / 4;
        const auto fall = hi - (hi - lo) * (step % 4) / 4;
        auto r = lo, g = lo, b = lo;
        switch (segment) {
          case 0: b = hi; r = rise; break; // blue -> magenta
          case 1: r = hi; b = fall; break; // magenta -> red
          case 2: r = hi; g = rise; break; // red -> yellow
          case 3: g = hi; r = fall; break; // yellow -> green
          case 4: g = hi; b = rise; break; // green -> cyan
          case 5: b = hi; g = fall; break; // cyan -> blue
        }
        __vga_palette[index][0] = static_cast<unsigned char>(r);
        __vga_palette[index][1] = static_cast<unsigned char>(g);
        __vga_palette[index][2] = static_cast<unsigned char>(b);
        ++index;
      }
    }
  }
  for (; index < 256; ++index) // 248-255: black, as the factory shipped it
    __vga_palette[index][0] = __vga_palette[index][1] = __vga_palette[index][2] = 0;
}

inline void __GRAPHICS_IMPL(int width, int height, int colors = 256, unsigned videoBase = 0xA0000) {
  __vga_width = width;
  __vga_height = height;
  __vga_color_mask = colors - 1;
  __vga_video_base = videoBase;
  __vga_unchained = false;
  __vga_plane_mask = 0xF;
  __vga_read_plane = 0;
  __vga_bank = 0;
  __vga_framebuffer.assign(width > 0 ? static_cast<size_t>(width) * height : 0, 0);
  __VGA_DEFAULT_PALETTE();
  if (colors == 4) {
    // CGA palette 1, high intensity: the cyan/magenta/white of childhood memories
    __vga_palette[1][0] = 21; __vga_palette[1][1] = 63; __vga_palette[1][2] = 63;
    __vga_palette[2][0] = 63; __vga_palette[2][1] = 21; __vga_palette[2][2] = 63;
    __vga_palette[3][0] = 63; __vga_palette[3][1] = 63; __vga_palette[3][2] = 63;
  } else if (colors == 2) {
    if (videoBase == 0xB0000) { // Hercules: green phosphor, as nature intended
      __vga_palette[1][0] = 0;  __vga_palette[1][1] = 63; __vga_palette[1][2] = 0;
    } else {                    // CGA mode 2: plain white on black
      __vga_palette[1][0] = 63; __vga_palette[1][1] = 63; __vga_palette[1][2] = 63;
    }
  }
  if (width <= 0) return;
  // best effort: VT processing + UTF-8 + hidden cursor (fails harmlessly when piped)
  const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD mode = 0;
  if (GetConsoleMode(handle, &mode))
    SetConsoleMode(handle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
  SetConsoleOutputCP(CP_UTF8);
  std::cout << "\x1b[2J\x1b[?25l" << std::flush;
}

// The QB mode table, plus a Hercules cameo. Each mode parks its video window
// at the historically correct segment.
inline void __SCREEN_IMPL(int mode) {
  switch (mode) {
    case 1:  __GRAPHICS_IMPL(320, 200, 4,   0xB8000); break; // CGA
    case 2:  __GRAPHICS_IMPL(640, 200, 2,   0xB8000); break; // CGA mono
    case 3:  __GRAPHICS_IMPL(720, 348, 2,   0xB0000); break; // Hercules
    case 7:  __GRAPHICS_IMPL(320, 200, 16,  0xA0000); break; // EGA
    case 8:  __GRAPHICS_IMPL(640, 200, 16,  0xA0000); break; // EGA
    case 9:  __GRAPHICS_IMPL(640, 350, 16,  0xA0000); break; // EGA
    case 12: __GRAPHICS_IMPL(640, 480, 16,  0xA0000); break; // VGA hi-res
    case 13: __GRAPHICS_IMPL(320, 200, 256, 0xA0000); break; // the one true mode
    // VESA mode numbers, faithfully fake. SVGA you couldn't afford in 1992;
    // anything over 64KB is bank-switched through the A000 window (VESA_BANK).
    case 0x100: __GRAPHICS_IMPL(640, 400, 256);  break;
    case 0x101: __GRAPHICS_IMPL(640, 480, 256);  break;
    case 0x103: __GRAPHICS_IMPL(800, 600, 256);  break;
    case 0x105: __GRAPHICS_IMPL(1024, 768, 256); break;
    case 0x107: __GRAPHICS_IMPL(1280, 1024, 256); break; // 24-bit modes left as an exercise for the therapist
    default: __GRAPHICS_IMPL(0, 0); std::cout << "\x1b[?25h" << std::flush; break; // SCREEN(0): back to text
  }
}

// Mode X / Y / Z: the unchained 256-colour tweaks of demo-scene legend. The
// A000 window goes planar - each address is FOUR pixels, one per plane, and
// POKE writes whichever planes the sequencer map mask (OUT 3C4/3C5 index 2)
// has enabled. PSET/POINT stay linear because they never lied to you.
inline void __MODE_UNCHAINED(int width, int height) {
  __GRAPHICS_IMPL(width, height, 256, 0xA0000);
  __vga_unchained = true;
}

#define SCREEN(mode) __SCREEN_IMPL(mode);
#define GRAPHICS(width, height) __GRAPHICS_IMPL(width, height);
#define MODE_X() __MODE_UNCHAINED(320, 240);
#define MODE_Y() __MODE_UNCHAINED(320, 200);
#define MODE_Z() __MODE_UNCHAINED(320, 400);
#define VESA_BANK(bank) __vga_bank = (bank);
#define SCREEN_WIDTH() (__vga_width)
#define SCREEN_HEIGHT() (__vga_height)
#define SCREEN_COLORS() (__vga_color_mask + 1)

// CGA palette select, like COLOR bg, p in SCREEN 1 - palette 0 is the muddy
// green/red/brown one, palette 1 the cyan/magenta/white of childhood memories
inline void __CGA_PALETTE_IMPL(int palette) {
  if (__vga_color_mask != 3) return;
  if (palette == 0) {
    __vga_palette[1][0] = 0;  __vga_palette[1][1] = 42; __vga_palette[1][2] = 0;
    __vga_palette[2][0] = 42; __vga_palette[2][1] = 0;  __vga_palette[2][2] = 0;
    __vga_palette[3][0] = 42; __vga_palette[3][1] = 21; __vga_palette[3][2] = 0;
  } else {
    __vga_palette[1][0] = 21; __vga_palette[1][1] = 63; __vga_palette[1][2] = 63;
    __vga_palette[2][0] = 63; __vga_palette[2][1] = 21; __vga_palette[2][2] = 63;
    __vga_palette[3][0] = 63; __vga_palette[3][1] = 63; __vga_palette[3][2] = 63;
  }
}
#define CGA_PALETTE(palette) __CGA_PALETTE_IMPL(palette);

inline void __PSET_IMPL(int x, int y, int color) {
  if (x < 0 || y < 0 || x >= __vga_width || y >= __vga_height) return;
  __vga_framebuffer[static_cast<size_t>(y) * __vga_width + x] = static_cast<BYTE>(color & __vga_color_mask);
}

inline int __POINT_IMPL(int x, int y) {
  if (x < 0 || y < 0 || x >= __vga_width || y >= __vga_height) return -1;
  return __vga_framebuffer[static_cast<size_t>(y) * __vga_width + x];
}

#define PSET(x, y, color) __PSET_IMPL(x, y, color);
#define POINT(x, y) __POINT_IMPL(x, y)

inline void __PALETTE_IMPL(int index, int r, int g, int b) {
  if (index < 0 || index > 255) return;
  const auto clamp = [](int value) { return static_cast<unsigned char>(value < 0 ? 0 : value > 63 ? 63 : value); };
  __vga_palette[index][0] = clamp(r);
  __vga_palette[index][1] = clamp(g);
  __vga_palette[index][2] = clamp(b);
}
#define PALETTE(index, r, g, b) __PALETTE_IMPL(index, r, g, b);

inline void __LINE_IMPL(int x1, int y1, int x2, int y2, int color) {
  // Bresenham, like everyone before us
  const auto dx = std::abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
  const auto dy = -std::abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
  auto err = dx + dy;
  for (;;) {
    __PSET_IMPL(x1, y1, color);
    if (x1 == x2 && y1 == y2) break;
    const auto doubled = 2 * err;
    if (doubled >= dy) { err += dy; x1 += sx; }
    if (doubled <= dx) { err += dx; y1 += sy; }
  }
}

inline void __LINE_BOX_IMPL(int x1, int y1, int x2, int y2, int color, bool filled) {
  if (x1 > x2) std::swap(x1, x2);
  if (y1 > y2) std::swap(y1, y2);
  for (auto y = y1; y <= y2; ++y)
    for (auto x = x1; x <= x2; ++x)
      if (filled || x == x1 || x == x2 || y == y1 || y == y2)
        __PSET_IMPL(x, y, color);
}

#define LINE(x1, y1, x2, y2, color) __LINE_IMPL(x1, y1, x2, y2, color);
#define LINE_B(x1, y1, x2, y2, color) __LINE_BOX_IMPL(x1, y1, x2, y2, color, false);
#define LINE_BF(x1, y1, x2, y2, color) __LINE_BOX_IMPL(x1, y1, x2, y2, color, true);

inline void __CIRCLE_IMPL(int cx, int cy, int radius, int color) {
  // midpoint circle, eight octants at a time
  if (radius < 0) return;
  auto x = radius, y = 0, err = 1 - radius;
  while (x >= y) {
    __PSET_IMPL(cx + x, cy + y, color); __PSET_IMPL(cx - x, cy + y, color);
    __PSET_IMPL(cx + x, cy - y, color); __PSET_IMPL(cx - x, cy - y, color);
    __PSET_IMPL(cx + y, cy + x, color); __PSET_IMPL(cx - y, cy + x, color);
    __PSET_IMPL(cx + y, cy - x, color); __PSET_IMPL(cx - y, cy - x, color);
    ++y;
    if (err < 0) err += 2 * y + 1;
    else { --x; err += 2 * (y - x) + 1; }
  }
}
#define CIRCLE(x, y, radius, color) __CIRCLE_IMPL(x, y, radius, color);

inline void __PAINT_IMPL(int x, int y, int color) {
  // bucket fill of the uniformly-coloured area under the cursor
  const auto target = __POINT_IMPL(x, y);
  if (target < 0 || target == color) return;
  std::vector<std::pair<int, int>> stack{{x, y}};
  while (!stack.empty()) {
    const auto [px, py] = stack.back();
    stack.pop_back();
    if (__POINT_IMPL(px, py) != target) continue;
    __PSET_IMPL(px, py, color);
    stack.push_back({px + 1, py});
    stack.push_back({px - 1, py});
    stack.push_back({px, py + 1});
    stack.push_back({px, py - 1});
  }
}
#define PAINT(x, y, color) __PAINT_IMPL(x, y, color);

// assembles the ANSI frame: ESC[H, then per cell one upper-half-block whose
// foreground is the top pixel and background the bottom pixel
inline STRING __VGA_RENDER() {
  STRING frame;
  if (__vga_width <= 0) return frame;
  frame.reserve(static_cast<size_t>(__vga_width) * __vga_height * 8);
  frame += "\x1b[H";
  char code[32];
  for (auto y = 0; y < __vga_height; y += 2) {
    auto lastTop = -1, lastBottom = -1;
    for (auto x = 0; x < __vga_width; ++x) {
      const int topPixel = __vga_framebuffer[static_cast<size_t>(y) * __vga_width + x];
      const int bottomPixel = y + 1 < __vga_height ? __vga_framebuffer[static_cast<size_t>(y + 1) * __vga_width + x] : 0;
      if (topPixel != lastTop) {
        std::snprintf(code, sizeof(code), "\x1b[38;2;%d;%d;%dm", __vga_palette[topPixel][0] * 255 / 63, __vga_palette[topPixel][1] * 255 / 63, __vga_palette[topPixel][2] * 255 / 63);
        frame += code;
        lastTop = topPixel;
      }
      if (bottomPixel != lastBottom) {
        std::snprintf(code, sizeof(code), "\x1b[48;2;%d;%d;%dm", __vga_palette[bottomPixel][0] * 255 / 63, __vga_palette[bottomPixel][1] * 255 / 63, __vga_palette[bottomPixel][2] * 255 / 63);
        frame += code;
        lastBottom = bottomPixel;
      }
      frame += "\xE2\x96\x80"; // the mighty upper half block
    }
    frame += "\x1b[0m\n";
  }
  return frame;
}

inline void __FLIP_IMPL() {
  const auto frame = __VGA_RENDER();
  std::cout.write(frame.data(), static_cast<std::streamsize>(frame.size()));
  std::cout.flush();
}
#define FLIP() __FLIP_IMPL();

// ===========================================================================
// PEEK / POKE / DEF SEG. One flat megabyte of perfectly fake RAM with honest
// real-mode address arithmetic: physical = segment * 16 + offset, wrapping at
// 1MB because the A20 line is off in this house. So A001:0000 and A000:0010
// and 9FFF:0020 are all the same byte - just like grandpa's 8086.
// The active video mode maps its framebuffer at the historically correct
// window (&HA000 for EGA/VGA, &HB800 for CGA, &HB000 for Hercules - spelled
// 0xA000 etc, C++ has feelings too), one linear byte per pixel (the chain-4
// fantasy; planar masochism not included). Yes, POKE(y * 320 + x, colour)
// draws a pixel in SCREEN 13. You're welcome and we're sorry.
// ===========================================================================
inline unsigned __basic_segment = 0;
inline std::vector<BYTE> __basic_memory; // lazily allocated megabyte of nostalgia

#define DEF_SEG(segment) __basic_segment = (segment);

inline unsigned __BASIC_PHYSICAL(unsigned offset) {
  return (__basic_segment * 16 + offset) & 0xFFFFF; // real mode, A20 disabled
}

inline bool __VGA_BANKED() { // SVGA: more pixels than a segment can hold -> 64KB window + bank switching
  return !__vga_unchained && __vga_video_base == 0xA0000 && __vga_framebuffer.size() > 0x10000;
}

inline size_t __VGA_WINDOW_SIZE() {
  if (__vga_width <= 0) return 0;
  if (__vga_unchained || __VGA_BANKED()) return 0x10000;
  return __vga_framebuffer.size();
}

inline int __PEEK_IMPL(unsigned offset) {
  const auto physical = __BASIC_PHYSICAL(offset);
  const auto window = __VGA_WINDOW_SIZE();
  if (window > 0 && physical >= __vga_video_base && physical - __vga_video_base < window) {
    const size_t rel = physical - __vga_video_base;
    if (__vga_unchained) { // planar read: read map select picks the plane
      const auto stride = static_cast<size_t>(__vga_width) / 4;
      const auto y = rel / stride, x = (rel % stride) * 4 + __vga_read_plane;
      return y < static_cast<size_t>(__vga_height) ? __vga_framebuffer[y * __vga_width + x] : 0;
    }
    const auto target = (__VGA_BANKED() ? static_cast<size_t>(__vga_bank) * 0x10000 : 0) + rel;
    return target < __vga_framebuffer.size() ? __vga_framebuffer[target] : 0;
  }
  if (__basic_memory.empty()) __basic_memory.assign(0x100000, 0);
  return __basic_memory[physical];
}

inline void __POKE_IMPL(unsigned offset, int value) {
  const auto physical = __BASIC_PHYSICAL(offset);
  const auto window = __VGA_WINDOW_SIZE();
  if (window > 0 && physical >= __vga_video_base && physical - __vga_video_base < window) {
    const size_t rel = physical - __vga_video_base;
    if (__vga_unchained) { // planar write: one address, up to four pixels, map mask decides
      const auto stride = static_cast<size_t>(__vga_width) / 4;
      const auto y = rel / stride, baseX = (rel % stride) * 4;
      if (y < static_cast<size_t>(__vga_height))
        for (auto plane = 0; plane < 4; ++plane)
          if (__vga_plane_mask & (1 << plane))
            __vga_framebuffer[y * __vga_width + baseX + plane] = static_cast<BYTE>(value & 0xFF);
      return;
    }
    const auto target = (__VGA_BANKED() ? static_cast<size_t>(__vga_bank) * 0x10000 : 0) + rel;
    if (target < __vga_framebuffer.size())
      __vga_framebuffer[target] = static_cast<BYTE>(value & 0xFF);
    return; // video region writes never fall through to RAM, just like the real bus
  }
  if (__basic_memory.empty()) __basic_memory.assign(0x100000, 0);
  __basic_memory[physical] = static_cast<BYTE>(value & 0xFF);
}

#define PEEK(offset) __PEEK_IMPL(offset)
#define POKE(offset, value) __POKE_IMPL(offset, value);

// ===========================================================================
// OUT / INP / WAIT. The VGA register file, lovingly faked:
//   3C8/3C9  DAC write index + data (three writes per entry, auto-increment)
//   3C7/3C9  DAC read index + data - yes, palette animation via OUT works
//   3C4/3C5  sequencer: index 2 = map mask (mode X/Y/Z plane writes),
//            index 0Eh = Trident bank register, XOR 2 quirk faithfully
//            reproduced because somebody out there still has nightmares
//   3CE/3CF  graphics controller: index 4 = read map select
//   3DA      input status: the retrace bit toggles every read, making this
//            the world's most cooperative vertical retrace
// Every other port is a junk drawer: INP returns what OUT last shouted at it.
// ===========================================================================
inline int __vga_dac_write_index = 0, __vga_dac_write_channel = 0;
inline int __vga_dac_read_index = 0, __vga_dac_read_channel = 0;
inline int __vga_sequencer_index = 0, __vga_gc_index = 0;
inline int __vga_retrace_status = 0;
inline std::map<int, BYTE> __basic_ports;

inline void __OUT_IMPL(int port, int value) {
  value &= 0xFF;
  switch (port) {
    case 0x3C8: __vga_dac_write_index = value; __vga_dac_write_channel = 0; break;
    case 0x3C9:
      __vga_palette[__vga_dac_write_index][__vga_dac_write_channel] = static_cast<unsigned char>(value & 0x3F);
      if (++__vga_dac_write_channel == 3) {
        __vga_dac_write_channel = 0;
        __vga_dac_write_index = (__vga_dac_write_index + 1) & 0xFF;
      }
      break;
    case 0x3C7: __vga_dac_read_index = value; __vga_dac_read_channel = 0; break;
    case 0x3C4: __vga_sequencer_index = value; break;
    case 0x3C5:
      if (__vga_sequencer_index == 2) __vga_plane_mask = value & 0xF;
      else if (__vga_sequencer_index == 0x0E) __vga_bank = value ^ 2; // thanks, Trident
      else __basic_ports[(0x3C5 << 8) | __vga_sequencer_index] = static_cast<BYTE>(value);
      break;
    case 0x3CE: __vga_gc_index = value; break;
    case 0x3CF:
      if (__vga_gc_index == 4) __vga_read_plane = value & 3;
      else __basic_ports[(0x3CF << 8) | __vga_gc_index] = static_cast<BYTE>(value);
      break;
    default: __basic_ports[port] = static_cast<BYTE>(value); break;
  }
}

inline int __INP_IMPL(int port) {
  switch (port) {
    case 0x3C9: {
      const int value = __vga_palette[__vga_dac_read_index][__vga_dac_read_channel];
      if (++__vga_dac_read_channel == 3) {
        __vga_dac_read_channel = 0;
        __vga_dac_read_index = (__vga_dac_read_index + 1) & 0xFF;
      }
      return value;
    }
    case 0x3C5:
      if (__vga_sequencer_index == 2) return __vga_plane_mask;
      if (__vga_sequencer_index == 0x0E) return __vga_bank ^ 2; // still thanks, Trident
      return __basic_ports.count((0x3C5 << 8) | __vga_sequencer_index) ? __basic_ports[(0x3C5 << 8) | __vga_sequencer_index] : 0;
    case 0x3CF:
      if (__vga_gc_index == 4) return __vga_read_plane;
      return __basic_ports.count((0x3CF << 8) | __vga_gc_index) ? __basic_ports[(0x3CF << 8) | __vga_gc_index] : 0;
    case 0x3DA: return __vga_retrace_status ^= 8;
    default: return __basic_ports.count(port) ? __basic_ports[port] : 0;
  }
}

#ifdef OUT
#undef OUT // like IN, some windows headers claim OUT as a SAL annotation; the bus needs it more
#endif
#define OUT(port, value) __OUT_IMPL(port, value);
#define INP(port) __INP_IMPL(port)
#define WAIT(port, mask) while ((__INP_IMPL(port) & (mask)) == 0) {}

// ===========================================================================
// GET / PUT sprites, QB style. GET grabs a rectangle into a SPRITE, PUT blits
// it back with PSET, PRESET, AND, OR or XOR (default XOR, obviously - how
// else would you erase by drawing twice). The verb arrives via the
// stringizer, which is the only reason AND and OR survive the preprocessor.
// ===========================================================================
struct __BASIC_SPRITE {
  int width = 0;
  int height = 0;
  std::vector<BYTE> pixels;
};
#define SPRITE __BASIC_SPRITE
#define SPRITE_WIDTH(sprite) ((sprite).width)
#define SPRITE_HEIGHT(sprite) ((sprite).height)

inline void __GET_SPRITE_IMPL(int x1, int y1, int x2, int y2, SPRITE & sprite) {
  if (x1 > x2) std::swap(x1, x2);
  if (y1 > y2) std::swap(y1, y2);
  sprite.width = x2 - x1 + 1;
  sprite.height = y2 - y1 + 1;
  sprite.pixels.assign(static_cast<size_t>(sprite.width) * sprite.height, 0);
  for (auto y = 0; y < sprite.height; ++y)
    for (auto x = 0; x < sprite.width; ++x) {
      const auto pixel = __POINT_IMPL(x1 + x, y1 + y);
      sprite.pixels[static_cast<size_t>(y) * sprite.width + x] = static_cast<BYTE>(pixel < 0 ? 0 : pixel);
    }
}

inline void __PUT_SPRITE_IMPL(int x, int y, const SPRITE & sprite, const STRING & verb) {
  for (auto sy = 0; sy < sprite.height; ++sy)
    for (auto sx = 0; sx < sprite.width; ++sx) {
      const auto existing = __POINT_IMPL(x + sx, y + sy);
      if (existing < 0) continue; // off screen
      const int value = sprite.pixels[static_cast<size_t>(sy) * sprite.width + sx];
      auto result = existing ^ value; // XOR is the default, ask any sprite
      if (verb == "PSET") result = value;
      else if (verb == "PRESET") result = ~value;
      else if (verb == "AND") result = existing & value;
      else if (verb == "OR") result = existing | value;
      __PSET_IMPL(x + sx, y + sy, result);
    }
}

#define GET_SPRITE(x1, y1, x2, y2, sprite) __GET_SPRITE_IMPL(x1, y1, x2, y2, sprite);
#define PUT_SPRITE(x, y, sprite, verb) __PUT_SPRITE_IMPL(x, y, sprite, #verb);
// the true believers' aliases
#define GET(x1, y1, x2, y2, sprite) __GET_SPRITE_IMPL(x1, y1, x2, y2, sprite);
#define PUT(x, y, sprite, verb) __PUT_SPRITE_IMPL(x, y, sprite, #verb);

// ===========================================================================
// PAINT, extended family. PAINT_BORDER floods until it hits the border
// colour (QB's PAINT (x,y), c, border). PAINT_PATTERN floods the same way
// but fills with a tile: a string where each character is one pixel colour,
// tileWidth pixels per row - build it with CHR$ like it's 1991.
// ===========================================================================
inline void __PAINT_FLOOD(int x, int y, int borderColor, const STRING & tile, int tileWidth) {
  if (__vga_width <= 0 || tile.empty() || tileWidth < 1) return;
  const auto start = __POINT_IMPL(x, y);
  if (start < 0 || start == borderColor) return;
  const auto tileHeight = (static_cast<int>(tile.length()) + tileWidth - 1) / tileWidth;
  std::vector<bool> visited(static_cast<size_t>(__vga_width) * __vga_height, false);
  std::vector<std::pair<int, int>> stack{{x, y}};
  while (!stack.empty()) {
    const auto [px, py] = stack.back();
    stack.pop_back();
    if (px < 0 || py < 0 || px >= __vga_width || py >= __vga_height) continue;
    const auto index = static_cast<size_t>(py) * __vga_width + px;
    if (visited[index]) continue;
    visited[index] = true;
    if (__POINT_IMPL(px, py) == borderColor) continue;
    const auto cell = (py % tileHeight) * tileWidth + (px % tileWidth);
    if (cell < static_cast<int>(tile.length()))
      __PSET_IMPL(px, py, static_cast<unsigned char>(tile[cell]));
    stack.push_back({px + 1, py});
    stack.push_back({px - 1, py});
    stack.push_back({px, py + 1});
    stack.push_back({px, py - 1});
  }
}

inline void __PAINT_BORDER_IMPL(int x, int y, int color, int borderColor) {
  __PAINT_FLOOD(x, y, borderColor, STRING(1, static_cast<char>(color)), 1);
}

#define PAINT_BORDER(x, y, color, borderColor) __PAINT_BORDER_IMPL(x, y, color, borderColor);
#define PAINT_PATTERN(x, y, tile, tileWidth, borderColor) __PAINT_FLOOD(x, y, borderColor, tile, tileWidth);

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
  if (__vga_width > 0) // graphics mode: CLS wipes the framebuffer
    std::fill(__vga_framebuffer.begin(), __vga_framebuffer.end(), static_cast<BYTE>(0));

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

// ===========================================================================
// SOUND and PLAY. SOUND(frequency, ticks) like QB (18.2 ticks per second),
// PLAY("T120 O4 L8 CDEFGAB") with a real Music Macro Language parser:
// notes A-G with #/+/- accidentals, O/</> octaves, L lengths, dots, T tempo,
// P/R rests, N note numbers (N37 = middle C), MN/ML/MS articulation.
// Beep() reroutes to the sound card on modern Windows - no PC speaker needed.
// ===========================================================================
struct __MML_NOTE {
  double frequency;          // Hz, 0 = rest
  double totalMilliseconds;  // note slot duration
  double soundMilliseconds;  // audible part (articulation)
};

inline int __MML_NUMBER(const STRING & tune, size_t & i) { // digits at i, -1 if none
  if (i >= tune.length() || !std::isdigit(static_cast<unsigned char>(tune[i]))) return -1;
  auto value = 0;
  while (i < tune.length() && std::isdigit(static_cast<unsigned char>(tune[i])))
    value = value * 10 + (tune[i++] - '0');
  return value;
}

inline std::vector<__MML_NOTE> __MML_PARSE(const STRING & tune) {
  std::vector<__MML_NOTE> notes;
  auto octave = 4;              // QB default; octave 3 holds middle C
  auto defaultLength = 4;
  auto tempo = 120.0;
  auto articulation = 7.0 / 8.0; // MN: 7/8 sound, 1/8 silence
  static const int semitoneOf[7] = {9, 11, 0, 2, 4, 5, 7}; // A..G

  const auto emit = [&](double frequency, int length, int dots) {
    auto milliseconds = (60000.0 / tempo) * 4.0 / length;
    auto extension = milliseconds / 2;
    for (auto dot = 0; dot < dots; ++dot, extension /= 2)
      milliseconds += extension;
    notes.push_back({frequency, milliseconds, milliseconds * articulation});
  };
  const auto frequencyOfMidi = [](int midi) { return 440.0 * std::pow(2.0, (midi - 69) / 12.0); };

  for (size_t i = 0; i < tune.length();) {
    const auto command = std::toupper(static_cast<unsigned char>(tune[i]));
    ++i;
    if (command >= 'A' && command <= 'G') {
      auto semitone = semitoneOf[command - 'A'];
      while (i < tune.length() && (tune[i] == '#' || tune[i] == '+' || tune[i] == '-')) {
        semitone += tune[i] == '-' ? -1 : +1;
        ++i;
      }
      auto length = __MML_NUMBER(tune, i);
      if (length < 1) length = defaultLength;
      auto dots = 0;
      while (i < tune.length() && tune[i] == '.') { ++dots; ++i; }
      emit(frequencyOfMidi((octave + 2) * 12 + semitone), length, dots);
      continue;
    }
    switch (command) {
      case 'O': { const auto o = __MML_NUMBER(tune, i); if (o >= 0) octave = o > 6 ? 6 : o; break; }
      case '>': if (octave < 6) ++octave; break;
      case '<': if (octave > 0) --octave; break;
      case 'L': { const auto l = __MML_NUMBER(tune, i); if (l >= 1) defaultLength = l; break; }
      case 'T': { const auto t = __MML_NUMBER(tune, i); if (t >= 32 && t <= 255) tempo = t; break; }
      case 'P': case 'R': {
        auto length = __MML_NUMBER(tune, i);
        if (length < 1) length = defaultLength;
        auto dots = 0;
        while (i < tune.length() && tune[i] == '.') { ++dots; ++i; }
        emit(0, length, dots);
        break;
      }
      case 'N': {
        const auto n = __MML_NUMBER(tune, i);
        if (n == 0) emit(0, defaultLength, 0);
        else if (n >= 1 && n <= 84) emit(frequencyOfMidi(n + 23), defaultLength, 0);
        break;
      }
      case 'M':
        if (i < tune.length()) {
          const auto style = std::toupper(static_cast<unsigned char>(tune[i]));
          ++i;
          if (style == 'N') articulation = 7.0 / 8.0;
          else if (style == 'L') articulation = 1.0;
          else if (style == 'S') articulation = 3.0 / 4.0;
          // MB/MF: everything here is foreground music anyway
        }
        break;
      default: break; // spaces and anything exotic: graciously ignored
    }
  }
  return notes;
}

inline void __SOUND_IMPL(double frequency, double ticks) {
  const auto milliseconds = static_cast<DWORD>(ticks * 1000.0 / 18.2);
  if (frequency < 37 || frequency > 32767) { Sleep(milliseconds); return; }
  Beep(static_cast<DWORD>(frequency), milliseconds);
}

inline void __PLAY_IMPL(const STRING & tune) {
  for (const auto & note : __MML_PARSE(tune)) {
    if (note.frequency < 37) { Sleep(static_cast<DWORD>(note.totalMilliseconds)); continue; }
    Beep(static_cast<DWORD>(note.frequency), static_cast<DWORD>(note.soundMilliseconds));
    const auto gap = note.totalMilliseconds - note.soundMilliseconds;
    if (gap >= 1) Sleep(static_cast<DWORD>(gap));
  }
}

#define SOUND(frequency, ticks) __SOUND_IMPL(frequency, ticks);
#define PLAY(tune) __PLAY_IMPL(tune);

// ===========================================================================
// DATA / READ / RESTORE. DATA(...) at file scope queues values in source
// order; READ(var) pops the next one converted to var's type; RESTORE
// rewinds. Reading past the end yields zero/empty (mercy, not "Out of DATA").
// ===========================================================================
inline std::vector<STRING> __data_store;
inline size_t __data_cursor = 0;

template<typename T>
STRING __DATA_STRINGIFY(const T & value) {
  std::ostringstream stream;
  stream << value;
  return stream.str();
}

template<typename... TValues>
bool __DATA_ADD(TValues&&... values) {
  (__data_store.push_back(__DATA_STRINGIFY(values)), ...);
  return true;
}

template<typename T>
void __READ_IMPL(T & target) {
  if (__data_cursor >= __data_store.size()) { target = T(); return; }
  std::istringstream stream(__data_store[__data_cursor++]);
  stream >> target;
}

inline void __READ_IMPL(STRING & target) {
  target = __data_cursor < __data_store.size() ? __data_store[__data_cursor++] : "";
}

#define DATA(...) [[maybe_unused]] static const bool UNIQUE(__data_line) = __DATA_ADD(__VA_ARGS__);
#define READ(var) __READ_IMPL(var);
#define RESTORE __data_cursor = 0;
#define DATA_REMAINING() static_cast<int>(__data_store.size() - __data_cursor)

// ===========================================================================
// File I/O channels: OPEN("file" FOR_OUTPUT AS 1), PRINT_FILE(1, ...),
// LINE_INPUT_FILE(1, s), INPUT_FILE(1, x), EOF_FILE(1), CLOSE_FILE(1),
// KILL("file"). The AS is the same AS as everywhere else. Channel numbers
// are yours to manage, exactly like 1989.
// ===========================================================================
inline std::map<int, std::fstream> __file_channels;

#define FOR_INPUT  , (std::ios::in)
#define FOR_OUTPUT , (std::ios::out | std::ios::trunc)
#define FOR_APPEND , (std::ios::out | std::ios::app)

inline void __OPEN_IMPL(const STRING & filename, std::ios::openmode mode, int channel) {
  auto & file = __file_channels[channel];
  if (file.is_open()) file.close();
  file.clear();
  file.open(filename, mode);
}
#define OPEN(...) EXPAND(__OPEN_IMPL(__VA_ARGS__));

template<typename... TValues>
void __PRINT_FILE_IMPL(int channel, TValues&&... values) {
  (__file_channels[channel] << ... << values) << '\n';
}
#define PRINT_FILE(...) __PRINT_FILE_IMPL(__VA_ARGS__);

template<typename T>
void __INPUT_FILE_IMPL(int channel, T & target) {
  auto & file = __file_channels[channel];
  file >> target;
  // swallow the delimiter after the item: spaces, one comma, or the line break
  while (file.peek() == ' ' || file.peek() == '\t') file.get();
  if (file.peek() == ',') file.get();
  else {
    if (file.peek() == '\r') file.get();
    if (file.peek() == '\n') file.get();
  }
}
#define INPUT_FILE(channel, var) __INPUT_FILE_IMPL(channel, var);

inline void __LINE_INPUT_FILE_IMPL(int channel, STRING & target) {
  std::getline(__file_channels[channel], target);
  if (!target.empty() && target.back() == '\r') target.pop_back();
}
#define LINE_INPUT_FILE(channel, var) __LINE_INPUT_FILE_IMPL(channel, var);

#define EOF_FILE(channel) (__file_channels[channel].peek() == std::char_traits<char>::eof())

inline void __CLOSE_FILE_IMPL(int channel) {
  const auto found = __file_channels.find(channel);
  if (found == __file_channels.end()) return;
  found->second.close();
  __file_channels.erase(found);
}
#define CLOSE_FILE(channel) __CLOSE_FILE_IMPL(channel);

#define KILL(filename) std::remove(STRING(filename).c_str());

inline BOOLEAN FILE_EXISTS(const STRING & filename) {
  return std::ifstream(filename).good();
}

